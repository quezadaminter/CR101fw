#include <atmel_start.h>
#include <avr/sfr_defs.h>
#include <util/atomic.h>
#include <util/delay.h>
#include "i2c_simple_master.h"
#include "Messages.h"
#include "tone.h"
#include "Registers.h"

uint16_t SWITCH_PAST_STATE = 0;
extern volatile bool touchEventDetected;

uint32_t now = 0;
uint32_t rebootTimer = 0;
uint32_t batteryLevelNextRead = 0;

#define BAT_LEVEL_FULL 860// (4200 * 1024) / 5000;;
#define BAT_LEVEL_DRAINED 614//(3000 * 1024) / 5000;

void CheckSwitchStates()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		uint16_t change = SWITCH_PAST_STATE ^ SWITCH_STATES;

		if(change)
		{
         DDRD |= _BV(5);
         PORTD ^= _BV(5);
			tone_play(F(5), 400);
			//// Send event register
			////uint8_t event = _BV(SWITCH_EVENT);
         //// Send the PI the switch event.
			//I2C_0_write2ByteRegister(PI_I2C_ADDRESS, PI_EVENT_REGISTER, SWITCH_STATES);
         //// Tell the T84 to light up the white LEDs.
			//I2C_0_write1ByteRegister(T84_I2C_SLAVE_ADDRESS, T84_REG_WHITE_PWM_VALUE, 255);
			
			//if(change & _BV(i))
			//{
			//// Send i2c switch event.
			//if(_BV(SWITCH_STATES) == PRESSED)
			//{
			//}
			tone_play(A(5), 400);
		}
		SWITCH_PAST_STATE = SWITCH_STATES;
	}
}

void CheckScrollClicks()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if(touchEventDetected == true)
		{
			touchEventDetected = false;
			// Call touch controller chip on i2c
			// and get the touch event
			// Read both status bytes to clear interrupt
			uint8_t clicks = I2C_0_read1ByteRegister(T84_I2C_SLAVE_ADDRESS, T84_REG_SCROLL_CLICKS);
         int8_t dir = clicks - 128;
			I2C_0_write1ByteRegister(PI_I2C_ADDRESS, dir > 0 ? SCROLL_RIGHT_EVENT : SCROLL_LEFT_EVENT, abs(dir));
		}
	}
}

typedef enum { eCharging, eEndOfCharge, eNotConnected } ChargingStateT;

ChargingStateT chargeState =  eNotConnected;

void CheckChargerStatus()
{
   // OUT high and IN low -> Charging.
   // OUT high and IN high -> Charge current past end of charge threshold.
   // OUT low and IN high -> Charger disconnected / no charging taking place.
   DDRD |= _BV(3);
   PORTD |= _BV(3);
   if(bit_is_clear(PIND, 4))
   {
      if(chargeState != eCharging)
      {
	      I2C_0_write1ByteRegister(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, (1 << T84_ORANGE_BIT));
	      I2C_0_write1ByteRegister(PI_I2C_ADDRESS, PI_EVENT_REGISTER, (1 << PI_EVENT_CHARGING_BIT));
         chargeState = eCharging;
      }
   }
   else
   {
      if(chargeState == eCharging)
      {
	      I2C_0_write1ByteRegister(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, (1 << T84_GREEN_BIT));
	      I2C_0_write1ByteRegister(PI_I2C_ADDRESS, PI_EVENT_REGISTER, (0 << PI_EVENT_CHARGING_BIT));
      }
      chargeState = eEndOfCharge;
      PORTD &= ~(_BV(3));
      DDRD &= ~(_BV(3));
      if(bit_is_set(PIND, 4))
      {
         if(chargeState == eEndOfCharge)
         {
   	      I2C_0_write1ByteRegister(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, 0);
	         I2C_0_write1ByteRegister(PI_I2C_ADDRESS, PI_EVENT_REGISTER, (0 << PI_EVENT_CHARGING_BIT));
         }
         chargeState = eNotConnected;
      }
   }
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void CheckBatteryLevel(uint32_t now)
{
   if(now > batteryLevelNextRead)
   {
       uint16_t battLevel = ADC_0_get_conversion(1);
       uint8_t pct = map(battLevel, BAT_LEVEL_DRAINED, BAT_LEVEL_FULL, 0, 100);
       I2C_0_write1ByteRegister(PI_I2C_ADDRESS, PI_BATTERY_LEVEL_REGISTER, pct);
       batteryLevelNextRead = now + 60000;
       
       if(chargeState == eNotConnected && pct < 75)
       {
         I2C_0_write1ByteRegister(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, (1 << T84_REG_LED_STATE));
       }
    }
}

void CheckToneState(uint32_t now)
{
	tone_update(now);
}

void CheckKeyCombinations(uint32_t now)
{
	if(SWITCH_STATES & REBOOT_KEY_MASK)
	{
		if(rebootTimer < now)
		{
			rebootTimer = now + 4000;
		}
		else if(now > rebootTimer)
		{
			// REBOOT!
		}
	}
}

// V1.0 PWM pin9 PD5
// Orange pin31 PD1
// RG pin30 PD0
// Buzz p10 PD6

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	//atmel_start_init();
   TIMER_1_init();
	sei();

   //DDRD |= (_BV(3)); // Charger status OUT
   //DDRD &= ~(_BV(4)); // Charger status IN

	// Initialize T84
	//I2C_0_write1ByteRegister(T84_I2C_SLAVE_ADDRESS, T84_REG_SYSTEM, T84_BOOT_UP_BIT);
	//_delay_ms(100);

	//tone_setup();
	//tone_play_block(F(5), 250);
	//tone_play_block(A(5), 250);

   //for (uint16_t e = 0; e < 1024; ++e)
   //{
      //uint8_t b = eeprom_read_byte((uint8_t*)&e);
      //if(b != 0xFF)
         //tone_play_block(F(5), 10);
   //}
   	
	// Wait for PI and T84 to come online

   DDRD |= _BV(0) | _BV(1) | _BV(5) | _BV(6);	
	/* Replace with your application code */
   uint8_t s = 0;
	while (1)
	{
		now = millis();
      if(now % 1000 == 0)
      {
         PORTD ^= _BV(1);
         if(s == 0)
         {
            s = 1;
            DDRD |= (_BV(0));
            PORTD |= (_BV(0));
            //tone_play(F(5), 250);
         }
         else if(s == 1)
         {
            s = 2;
            PORTD &= ~(_BV(0));
         }
         else if(s == 2)
         {
            s = 0;
            PORTD &= ~(_BV(0));
            DDRD &= ~(_BV(0));
         }
      }
      //_delay_ms(1000);
		//CheckSwitchStates();
		//CheckScrollClicks();
		//CheckChargerStatus();
		//CheckBatteryLevel(now);
		//CheckKeyCombinations(now);
//		CheckToneState(now);
      //CheckSleepTimer(now);
	}
}
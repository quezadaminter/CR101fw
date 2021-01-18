/*
 * Test328PB.cpp
 *
 * Created: 5/9/2020 3:20:37 PM
 * Author : MQUEZADA
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include <string.h>
#include "TWI.h"
#include "Timer1.h"
#include "Tones.h"
#include "PCInterrupts.h"
#include "Registers.h"
#include "Messages.h"
#include "PinMap.h"
#include "ADC.h"
#include "Timeout.h"
#include "OPT3001.h"

uint8_t PI_EVENTS_REGISTER = 0x00;

uint32_t now = 0;
uint8_t T84_LED_STATES = 0x00;

#define SLEEP_TIMEOUT (60000 * 2) // LCD off, idle sleep
#define DEEP_SLEEP_TIMEOUT (60000 * 3) // LCD off, Rpi Standby, deep sleep, should be 10 minutes, we'll see.
#define SLEEP_MASK (1 << 0)
#define DEEP_SLEEP_MASK (1 << 1)
uint8_t sleepState = 0;

void WDTInit()
{
   // Clear WDT reset flag if it was set
   // is possible previous WDT reset
   // since we can not clear WDE if WDRF is set.
   MCUSR &= ~(1<<WDRF);

   cli();

   wdt_reset();
   // Enable a change on the WD control register.
   // Must be completed within 4 clock cycles so
   // we do it on the same instruction.
   WDTCSR |= (1<<WDCE)|(1<<WDE); // Enable changes to the WDT
   // Enable the system reset and the interrupt mode also and set prescaling
   // for 2 second timeout.
   WDTCSR = (1<<WDE)|(1<< WDIE)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0);

   sei();
}

ISR(WDT_vect)
{
   WDTCSR |= (1<<WDIF); // Clear interrupt flag (should be done by hw)
}

#define BDDR_ON 0B00000000
#define BPOR_ON 0B00111111
#define CDDR_ON 0B00000000
#define CPOR_ON 0B00001101
#define DDDR_ON 0B01100000
#define DPOR_ON 0B10100010
#define EDDR_ON 0B00000000
#define EPOR_ON 0B00001101

#define BDDR_SL 0B00000000
#define BPOR_SL 0B00000000
#define CDDR_SL 0B00000000
#define CPOR_SL 0B00000000
#define DDDR_SL 0B00000000
#define DPOR_SL 0B00000000
#define EDDR_SL 0B00000000
#define EPOR_SL 0B00000000

void InitializeIO()
{
   //WDTInit();
   // Initialize the pullups
   // on the switch input ports.
   //DDRB  = BDDR_ON;   //0B00000000;
   //PORTB = BPOR_ON;   //0B00111111;
   //DDRC  = CDDR_ON;   //0B00000000;
   //PORTC = CPOR_ON;   //0B11001101;
   //DDRD  = DDDR_ON;   //0B00100010;
   //PORTD = DPOR_ON;   //0B10010000;
   //DDRE  = EDDR_ON;
   //PORTE = EPOR_ON;
   
   PI_RES_TOG_DIR_IN;
   CH_STAT_OUT_DIR_IN;
   CH_STAT_OUT_HIGH;
   CH_STAT_IN_DIR_IN;
   CH_STAT_IN_LOW;
   // Stay high to keep power to the PI
   PI_PWR_SW_DIR_OUT;PI_PWR_SW_HIGH;

   ints.Init();
   twi.Init(TWI::e100000);
   t1.Init();
   tones.Init();
   adc.Init();
   opt3001.Init();

   tones.PlayBlock(F(5), 200);
   tones.PlayBlock(F(4), 200);
}

void Sleep()
{
   //// Set pins to high impedance inputs (no pullup)
	//// to save power and hold the line
	//// so there are no fake button presses.
   //BYTE &= ~((1 << 7) | (1 << 6))
   //DDRB  = BDDR_SL;   //0B00000000;
   //PORTB = BPOR_SL;   //0B00111111;
   //DDRC  = CDDR_SL;   //0B00000000;
   //PORTC = CPOR_SL;   //0B11001101;
   //DDRD  = DDDR_SL;   //0B00100010;
   //PORTD = DPOR_SL;   //0B10010000;
   //DDRE  = EDDR_SL;   //0B00100010;
   //PORTE = EPOR_SL;   //0B10010000;

   PI_RES_TOG_DIR_IN;
   CH_STAT_OUT_DIR_IN;
   CH_STAT_OUT_LOW;
   CH_STAT_IN_DIR_IN;
   CH_STAT_IN_LOW;
   // Stay high to keep power to the PI
   PI_PWR_SW_DIR_OUT;PI_PWR_SW_HIGH;

   ints.Sleep();
   twi.Sleep();
   t1.Sleep();
   tones.Sleep();
   adc.Sleep();
   wdt_disable();

	ACSR |= _BV(ACD);                       // Disable comparator
	MCUCR |= _BV(BODS) | _BV(BODSE);          //turn off the brown-out detector

   set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement
	sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
	sleep_mode();                            // sleep
	sleep_disable();                        // Clear SE bit

	InitializeIO();
	sei();
}

typedef enum { eCharging, eEndOfCharge, eNotConnected } ChargingStateT;

ChargingStateT chargeState =  eNotConnected;

void CheckChargerStatus()
{
   // For LTC4002, from datasheet:
   // When  a  charge  cycle  starts,  the  CHRG  pin  is  pulled  to
   // ground by an internal N-channel MOSFET which is capable
   // of driving an LED. When the charge current drops below
   // the  End-of-Charge  threshold  for  more  than  120?s,  the
   // N-channel  MOSFET  turns  off  and  a  weak  25?A  current
   // source to ground is connected to the CHRG pin. This weak
   // 25?A pull-down remains until the timer ends the charge
   // cycle, or the charger is in manual shutdown or sleep mode.
   // After a time out occurs (charge cycle ends), the pin will
   // become high impedance. By using two different value resistors,
   // a microprocessor can detect three states from this
   // pin (charging, end-of-charge and charging stopped).

   // To detect the charge mode, force the digital output pin,
   // OUT, high and measure the voltage at the CHRG pin. The
   // N-channel MOSFET will pull the pin low even with a 2k
   // pull-up resistor. Once the charge current drops below the
   // End-of-Charge threshold, the N-channel MOSFET is turned
   // off and a 25uA current source is connected to the CHRG
   // pin. The IN pin will then be pulled high by the 2k resistor
   // connected  to  OUT.  Now  force  the  OUT  pin  into  a  high
   // impedance state, the current source will pull the pin low
   // through  the  390k  resistor.  When  the  internal  timer  has
   // expired, the CHRG pin changes to a high impedance state
   // and the 390k resistor will then pull the pin high to indicate
   // charging has stopped.

   // OUT high and IN low -> Charging.
   // OUT high and IN high -> Charge current past end of charge threshold.
   // OUT low and IN high -> Charger disconnected / no charging taking place.
   CH_STAT_OUT_DIR_OUT;
   CH_STAT_OUT_HIGH;
   _delay_ms(5);
   //if(CH_STAT_IN_IS_LOW)// bit_is_clear(PIND, 4))
   if(bit_is_clear(PIND, 4))
   {
      if(chargeState != eCharging)
      {
         PI_EVENT_SET_CHARGING(PI_EVENTS_REGISTER);
         //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_ORANGE_BIT_MASK);
         twi.SendByte(PI_I2C_ADDRESS, PI_EVENT_REGISTER, PI_EVENTS_REGISTER);
         chargeState = eCharging;
      }
   }
   else
   {
      CH_STAT_OUT_LOW;
      CH_STAT_OUT_DIR_IN;
      _delay_ms(5);
      //if(CH_STAT_IN_IS_HIGH)// bit_is_set(PIND, 4))
      if(bit_is_set(PIND, 4))
      {
         if(chargeState != eNotConnected)
         {
            PI_EVENT_SET_CHARGER_DISCONNECTED(PI_EVENTS_REGISTER);
            //T84_LED_STATES &= ~T84_GREEN_BIT_MASK;
            //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
            twi.SendByte(PI_I2C_ADDRESS, PI_EVENT_REGISTER, PI_EVENT_REGISTER);
         }
         chargeState = eNotConnected;
      }
      else if(chargeState != eEndOfCharge)
      {
         chargeState = eEndOfCharge;
         PI_EVENT_SET_END_OF_CHARGE(PI_EVENTS_REGISTER);

         T84_LED_STATES |= T84_GREEN_BIT_MASK;
         //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
         twi.SendByte(PI_I2C_ADDRESS, PI_EVENT_REGISTER, PI_EVENTS_REGISTER);
      }
   }
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//#define BAT_LEVEL_FULL 860// (4200 * 1024) / 5000;
#define BAT_LEVEL_FULL 757// (3700 * 1024) / 5000;
#define BAT_LEVEL_DRAINED 614//(3000 * 1024) / 5000;
uint16_t battPct = 0;
void CheckBatteryLevel()
{
   uint16_t bl = adc.Read();//ADC_0_get_conversion(1);
   uint8_t pct = map(bl, BAT_LEVEL_DRAINED, BAT_LEVEL_FULL, 0, 100);
   
   twi.SendByte(PI_I2C_ADDRESS, PI_BATTERY_LEVEL_REGISTER, (pct > 100 ? 100 : pct));
   //twi.SendWord(PI_I2C_ADDRESS, PI_DEBUG_WORD_REGISTER, bl);
   
   if(chargeState == eNotConnected && abs(pct - battPct) > 5)
   {
      battPct = pct;
      if(pct >= 75 && pct < 85)
      {
         T84_LED_STATES |= T84_ORANGE_BIT_MASK;
         T84_LED_STATES &= ~T84_RED_BIT_MASK;
         twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
      }
      else if(pct < 75)
      {
         T84_LED_STATES &= ~T84_ORANGE_BIT_MASK;
         T84_LED_STATES |= T84_RED_BIT_MASK;
         twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
      }
      else
      {
         T84_LED_STATES &= ~(T84_ORANGE_BIT_MASK | T84_RED_BIT_MASK);
         twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
      }
   }
}

void CheckOPT3001()
{
   uint16_t mid = opt3001.GetManufacturerID();
   twi.SendWord(PI_I2C_ADDRESS, PI_DEBUG_WORD_REGISTER_H, mid);
}

uint8_t s = 0;
void Test()
{
   if(s == 0)
   {
      s = 1;
      //tones.Play(F(5), 200);
      //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_GREEN_BIT_MASK);
   }
   else if(s == 1)
   {
      s = 2;
      //tones.Play(A(4), 200);
      //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_RED_BIT_MASK);
   }
   else if(s == 2)
   {
      s = 3;
      //tones.Play(E(3), 200);
      //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, 0x00);
      twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_I2C_TEST, 110);
   }
   else if(s == 3)
   {
      s = 0;
      //tones.Play(E(3), 200);
      //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, 0x00);
      twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_I2C_TEST, 150);
   }
}

void TestPWM()
{
   twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_WHITE_PWM_VALUE, 0xFF);
}

void TestOrange()
{
   T84_LED_STATES ^= T84_GREEN_BIT_MASK;
   twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
   //twi.SendWord(PI_I2C_ADDRESS, PI_INPUT_REGISTER, SWITCH_STATES);
}

uint8_t found[5] = { 0 };

void ScanI2C()
{
   //found = 0;
   memset(found, 0, 5);
   //found = twi.Ping(T84_I2C_SLAVE_ADDRESS);
   //twi.SendByte(PI_I2C_ADDRESS, PI_DEBUG_BYTE_REGISTER, address);
   for(uint8_t address = 1, i = 0; address < 127; address++)
   {
      if(twi.Ping(address) == SUCCESS)
      {
         found[i++] = address;
      }
   }
}

Timeout test(0, 500, Test);
Timeout testPWM(0, 10000, TestPWM);
Timeout testOrange(0, 1000, TestOrange);
Timeout charger(0, 2000, CheckChargerStatus);
Timeout batteryCheck(0, 6000, CheckBatteryLevel);
Timeout scanI2C(0, 5000, ScanI2C);
Timeout o3001(0, 4000, CheckOPT3001);

uint32_t le = 0;
int main(void)
{
   // Set the proper state for the pins
   InitializeIO();
   sei();

    /* Replace with your application code */
    //test.Active();
    //testPWM.Active();
    //testOrange.Active();
    batteryCheck.Active();
    //o3001.Active();
    charger.Active();
    //scanI2C.Active();

    //tones.Test();

    // Synchronize state with display
    PI_CLEAR_SSR(PI_EVENTS_REGISTER);
    twi.SendByte(PI_I2C_ADDRESS, PI_EVENT_REGISTER, PI_EVENTS_REGISTER);
    while (1) 
    {
      now = t1.millis();

      // Code that executes every frame.
      ints.CheckSwitchStates(now);

      // Code that runs on a schedule.
      test.RunOn(now);
      testPWM.RunOn(now);
      testOrange.RunOn(now);

      batteryCheck.RunAt(now);
      charger.RunOn(now);
      //o3001.RunOn(now);

      scanI2C.RunOn(now);
      le = ints.LastEventTime();
      if(now - le > SLEEP_TIMEOUT)//ints.LastEventTime() > SLEEP_TIMEOUT)
      {
         if(now - ints.LastEventTime() > DEEP_SLEEP_TIMEOUT)
         {
            //sleepState = 0;
            //sleepState |= DEEP_SLEEP_MASK;
            ////Sleep();
            //PI_SET_SHUTDOWN(PI_EVENTS_REGISTER);
            //PI_EVENT_SET_CHARGER_DISCONNECTED(PI_EVENTS_REGISTER);
         }
         else if((PI_EVENTS_REGISTER & PI_EVENT_SLEEP_BIT) == false)
         {
            PI_SET_SLEEP(PI_EVENTS_REGISTER);
            sleepState = 0;
            sleepState |= SLEEP_MASK;
            twi.SendByte(PI_I2C_ADDRESS, PI_EVENT_REGISTER, PI_EVENTS_REGISTER);
            //LCD_OFF();
         }
      }
      else if(PI_EVENTS_REGISTER & (PI_EVENT_SLEEP_BIT | PI_EVENT_SHUTDOWN_BIT))
      {
         PI_CLEAR_SSR(PI_EVENTS_REGISTER);
         twi.SendByte(PI_I2C_ADDRESS, PI_EVENT_REGISTER, PI_EVENTS_REGISTER);
      }

      //CheckScrollClicks();
      //CheckSleepTimer(now);
      tones.Update(now);

      // Feed the dog so it don't bite.
      //wdt_reset();
    }
}


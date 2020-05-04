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

void CheckSwitchStates()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		uint16_t change = SWITCH_PAST_STATE ^ SWITCH_STATES;

		if(change)
		{
			// Send event register
			uint8_t event = _BV(SWITCH_EVENT);
			I2C_0_write2ByteRegister(PI_I2C_ADDRESS, PI_EVENT_REGISTER, SWITCH_STATES);
			I2C_0_write1ByteRegister(T84_I2C_SLAVE_ADDRESS, T84_REG_WHITE_PWM_VALUE, 255);
			// Send the change data
			// Send the new switch states
			
			//if(change & _BV(i))
			//{
			//// Send i2c switch event.
			//if(_BV(SWITCH_STATES) == PRESSED)
			//{
			//}
			tone_play(F(5), 100);
			tone_play(A(5), 100);
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
			int8_t status = I2C_0_read1ByteRegister(T84_I2C_SLAVE_ADDRESS, T84_REG_SCROLL_CLICKS);
			I2C_0_write1ByteRegister(PI_I2C_ADDRESS, status > 0 ? SCROLL_RIGHT_EVENT : SCROLL_LEFT_EVENT, status);
		}
	}
}

void CheckBatteryLevel()
{
	
}

void CheckChargerStatus()
{
	
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
		else
		{
			// REBOOT!
		}
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// Initialize Touch IC
	I2C_0_write1ByteRegister(T84_I2C_SLAVE_ADDRESS, T84_REG_SYSTEM, T84_BOOT_UP_BIT);
	_delay_ms(100);

	tone_setup();
	tone_play(F(5), 250);
	tone_play(A(5), 250);
	
	// Wait for PI and T84 to come online
	
	/* Replace with your application code */
	while (1)
	{
		now = millis();
		CheckSwitchStates();
		CheckScrollClicks();
		CheckBatteryLevel();
		CheckChargerStatus();
		CheckKeyCombinations(now);
		CheckToneState(now);
	}
}
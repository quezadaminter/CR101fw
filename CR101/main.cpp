#include <atmel_start.h>
#include <avr/sfr_defs.h>
#include <util/atomic.h>
#include <util/delay.h>
#include "i2c_simple_master.h"
#include "Messages.h"
#include "tone.h"

uint16_t SWITCH_PAST_STATE = 0;
extern volatile bool touchEventDetected;

void CheckSwitchStates()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		uint16_t change = SWITCH_PAST_STATE ^ SWITCH_STATES;

		if(change)
		{
			// Send event register
			uint8_t event = _BV(SWITCH_EVENT);
			// Send the change data
			// Send the new switch states
			
			//if(change & _BV(i))
			//{
			//// Send i2c switch event.
			//if(_BV(SWITCH_STATES) == PRESSED)
			//{
			//}
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
			uint8_t status = I2C_0_read1ByteRegister(TOUCH_I2C_ADDRESS, TOUCH_STATUS_REGISTER);
			uint8_t input = I2C_0_read1ByteRegister(TOUCH_I2C_ADDRESS, TOUCH_INPUT_REGISTER);
			
			// Determine direction and send.
			
		}
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// Initialize Touch IC
	I2C_0_write1ByteRegister(TOUCH_I2C_ADDRESS, TOUCH_RESET_REGISTER, TOUCH_RESET_DATA);
	_delay_ms(100);

	tone_setup();
	tone_play(F(5));
	_delay_ms(250);
	tone_play(A(5));
	_delay_ms(250);
	tone_stop();
	
	/* Replace with your application code */
	while (1)
	{
		CheckSwitchStates();
		CheckScrollClicks();
	}
}
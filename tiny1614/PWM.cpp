/*
 * PWM.cpp
 *
 * Created: 5/3/2020 2:39:55 AM
 *  Author: MQUEZADA
 */ 
#include <avr/power.h>
#include <avr/io.h>
#include "PWM.h"
#include "PinMap.h"

#define RESET_COUNTER (TCA0.SINGLE.CNT = 0)
#define PWM_ON  (TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm)
#define PWM_OFF TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;RESET_COUNTER

PWM pwm;

void PWM::begin()//Pin &pin)
{
   // The pin must be set as output for the PWM
   // signal to make it outside the chip.
   PORTB.DIRSET = LEDPWMbm;
   PORTB.OUTCLR = LEDPWMbm;

   // Using alternate pin for WO0 (PB3)
   PORTMUX.CTRLC = 0x01;

   // Start at 50% duty cycle
   TCA0.SINGLE.CMP0 = 0x7f; /* Setting: 0x7fff */

   // TCA0.SINGLE.CMP1 = 0x0; /* Setting: 0x0 */

   // TCA0.SINGLE.CMP2 = 0x0; /* Setting: 0x0 */

   // Initialize at 0
   TCA0.SINGLE.CNT = 0x0; /* Count: 0x0 */

   TCA0.SINGLE.CTRLB = 0 << TCA_SINGLE_ALUPD_bp            /* Auto Lock Update: disabled */
                     | 1 << TCA_SINGLE_CMP0EN_bp         /* Setting: enabled */
                     | 0 << TCA_SINGLE_CMP1EN_bp         /* Setting: disabled */
                     | 0 << TCA_SINGLE_CMP2EN_bp         /* Setting: disabled */
                     | TCA_SINGLE_WGMODE_SINGLESLOPE_gc; /*  */

   // TCA0.SINGLE.CTRLC = 0 << TCA_SINGLE_CMP0OV_bp /* Setting: disabled */
   //		 | 0 << TCA_SINGLE_CMP1OV_bp /* Setting: disabled */
   //		 | 0 << TCA_SINGLE_CMP2OV_bp; /* Setting: disabled */

   // TCA0.SINGLE.DBGCTRL = 0 << TCA_SINGLE_DBGRUN_bp; /* Debug Run: disabled */

   // TCA0.SINGLE.EVCTRL = 0 << TCA_SINGLE_CNTEI_bp /* Count on Event Input: disabled */
   //		 | TCA_SINGLE_EVACT_POSEDGE_gc /* Count on positive edge event */;

   // TCA0.SINGLE.INTCTRL = 0 << TCA_SINGLE_CMP0_bp /* Compare 0 Interrupt: disabled */
   //		 | 0 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: disabled */
   //		 | 0 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: disabled */
   //		 | 0 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt Enable: disabled */

   TCA0.SINGLE.PER = 0xff; /* Top Value: 0xffff */

   // The frequency doesn't much matter.
   TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc /* System Clock / 64 */
                     | 1 << TCA_SINGLE_ENABLE_bp /* Module Enable: enabled */;
	setLevel(0);
}

void PWM::setLevel(uint16_t dutyCycle)// Pin &pin, uint8_t dutyCycle)
{
	TCA0.SINGLE.CMP0 = dutyCycle;
	if(dutyCycle == 0)
	{
		// timer off, set output low.
		PWM_OFF;
      LEDPWM_OFF;
	}
	else if(dutyCycle == 0xFF)
	{
		// timer off, set output high.
		PWM_OFF;
      LEDPWM_OFF;
	}
	else
	{
		PWM_ON;
	}
}

void PWM::Start()
{
	PWM_ON;
}

void PWM::Stop()
{
   setLevel(0);
	PWM_OFF;
}

void PWM::Sleep()
{
   Stop();
   TCA0.SINGLE.CTRLA = ~TCA_SINGLE_ENABLE_bm /* Module Enable: enabled */;
}
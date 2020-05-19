/*
 * Timer0.cpp
 *
 * Created: 4/22/2020 9:47:39 AM
 *  Author: MQUEZADA
 */ 
#include <avr/sfr_defs.h>
#include <avr/power.h>
#include <util/atomic.h>
#include "Timer0.h"

Timer0 timer0;

uint32_t ms = 0;

void Timer0::begin()
{
	//PRR &= ~(_BV(PRTIM0)); // Power up the timer0
   TCB0.CTRLB = 0x00; // Periodic interrupt mode.
   TCB0.INTCTRL = TCB_CAPT_bm; // Capture the compare interrupt.
   TCB0.CNT = 0; // Start counter from 0.
   TCB0.CCMP = F_CPU / 1000; // Set the compare value so the interrupt fires every millisecond.
   TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm; // Set the clock source and enable the timer.
}

uint32_t Timer0::millis()
{
	uint32_t mil(0);
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		mil = ms;
	}
	return(mil);
}

void Timer0::PauseTimer()
{
	TCB0.CTRLA &= ~TCB_ENABLE_bm;
//	PRR |= _BV(PRTIM0);
}

void Timer0::ResumeTimer()
{
//	PRR &= ~(_BV(PRTIM0));
	TCB0.CTRLA |= TCB_ENABLE_bm;
}

void Timer0::ResetTimer()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		ms = 0;
	}
}

void Timer0::TimerRemove(uint32_t rem)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if(rem > ms)
		{
			ms = 0;
		}
		else
		{
			ms -= rem;
		}
	}
}

void Timer0::TimerAdd(uint32_t add)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		ms += add;
	}
}

void Timer0::Sleep()
{
	//PRR |= _BV(PRTIM0);
   TCB0.CTRLA &= ~TCB_ENABLE_bm;
}

uint32_t Timer0::micros()
{
	// 64 scaler, 16 = F_CPU / 1,000,000 where F_CPU=16000000
//	return(((timer0Overflow << 8) + TCNT0) * (64 / 16));
	//return(TCNT0 * 8); // (8000000 / 64) = 125000Hz; 1/125000=.000008s;.000008*1000000=8uS every TCNT0 tick
   return(TCB0.CNT * 10);
}

ISR(TCB0_INT_vect)
{
	++ms;
   TCB0.INTFLAGS |= TCB_CAPT_bm;
}
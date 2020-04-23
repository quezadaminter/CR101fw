/*
 * Timer0.cpp
 *
 * Created: 4/22/2020 9:47:39 AM
 *  Author: MQUEZADA
 */ 
#include "Timer0.h"

void ConfigureTimer0()
{
	// Set timer 0 so it runs on Clear to Compare mode (CTC)
	TCCR0A = _BV(WGM01);

	// Select the clock prescaler
	#if F_CPU == 1000000UL
	// Divide by 8, effective clock tick is 125Kz
	TCCR0B = _BV(CS01);
	#elif F_CPU == 8000000UL
	// Divide by 64, effective clock tick is 125Kz
	TCCR0B = _BV(CS00) | _BV(CS01);
	#else
	#error "Invalid F_CPU value."
	#endif

	// Configure interrupt
	TIMSK0 = _BV(OCIE0A);

	// Set compare reference value
	#if F_CPU == 16000000UL
	OCR0A = ((F_CPU / 256UL) / 1000UL);
	#elif F_CPU == 8000000UL
	OCR0A = ((F_CPU / 64UL) / 1000UL);
	#endif
	
}

uint32_t millis()
{
	uint32_t mil(0);
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		mil = ms;
	}
	return(mil);
}

void PauseTimer()
{
	TIMSK0 &= ~(_BV(OCIE0A));
	power_timer0_disable();
}

void ResumeTimer()
{
	power_timer0_enable();
	TIMSK0 |= _BV(OCIE0A);
}

void ResetTimer()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		ms = 0;
	}
}

void TimerRemove(uint32_t rem)
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

void TimerAdd(uint32_t add)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		ms += add;
	}
}

size_t micros()
{
	// 64 scaler, 16 = F_CPU / 1,000,000 where F_CPU=16000000
//	return(((timer0Overflow << 8) + TCNT0) * (64 / 16));
	return(TCNT0 * 8); // (8000000 / 64) = 125000Hz; 1/125000=.000008s;.000008*1000000=8uS every TCNT0 tick
}

ISR(TIM0_COMPA_vect)
{
	++ms;
}
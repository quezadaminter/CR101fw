/* 
* Timer1.cpp
*
* Created: 5/9/2020 11:02:45 PM
* Author: MQUEZADA
*/

#include <avr/io.h>
#include <util/atomic.h>
#include "Timer1.h"

Timer1 t1;
volatile uint32_t ms = 0;

// default constructor
Timer1::Timer1()
{
} //Timer1

// default destructor
Timer1::~Timer1()
{
} //~Timer1

void Timer1::Init()
{
	/* Enable TC1 */
	PRR0 &= ~(1 << PRTIM1);

   TCCR1A = (0 << COM1A1) | (0 << COM1A0) /* Normal port operation, OCA disconnected */
			 | (0 << COM1B1) | (0 << COM1B0) /* Normal port operation, OCB disconnected */
			 | (0 << WGM11) | (0 << WGM10); /* TC16 Mode 4 Normal / CTC, depends on WGM13 and WGM12 on TCCR1B*/

   TCCR1B = (0 << WGM13) | (1 << WGM12)                /* TC16 Mode 4 CTC */
	         | 0 << ICNC1                               /* Input Capture Noise Canceler: disabled */
	         | 0 << ICES1                               /* Input Capture Edge Select: disabled */
#if F_CPU == 1000000 // divide by 8 // effective clock rate is 125KHz
	         | (0 << CS12) | (1 << CS11) | (0 << CS10); /* IO clock divided by 8 */
#elif F_CPU == 8000000 // divide by 64 // effective clock rate is 125KHz
	         | (0 << CS12) | (1 << CS11) | (1 << CS10); /* IO clock divided by 64 */
#elif F_CPU == 16000000 // divide by 64 // effective clock rate is 250000 Hz
	         | (1 << CS12) | (0 << CS11) | (0 << CS10); /* IO clock divided by 8 */
#else
#error "Invalid F_CPU value."
#endif

#if F_CPU == 1000000
   	OCR1A = ((F_CPU / 8UL) / 1000UL) - 1; // Count to 125
#elif F_CPU == 8000000
   	OCR1A = ((F_CPU / 64UL) / 1000UL) - 1; // Count to 125
#elif F_CPU == 16000000
   	OCR1A = ((F_CPU / 64UL) / 1000UL) - 1; // Count to 250
#else
#error "Invalid F_CPU value."
#endif

   TCNT1 = 0;
   TIFR1 = 0;

	// Enable the interrupt
	TIMSK1 = 0 << OCIE1B   /* Output Compare B Match Interrupt Enable: disabled */
	         | 1 << OCIE1A /* Output Compare A Match Interrupt Enable: enabled */
	         | 0 << ICIE1  /* Input Capture Interrupt Enable: disabled */
	         | 0 << TOIE1; /* Overflow Interrupt Enable: disabled */

   //TCCR1A = 0x00;
   //TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
   //OCR1A = 124;
   //TCNT1 = 0;
   //TIMSK1 = (1 << OCIE1A);
}

uint32_t Timer1::millis()
{
   uint32_t m;
   ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
   {
      m = ms;
   }
   return(m);
}

void Timer1::Sleep()
{
	PRR0 |= (1 << PRTIM1);
}

ISR(TIMER1_COMPA_vect)
{
   ++ms;
}
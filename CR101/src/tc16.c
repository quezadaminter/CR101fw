/**
 * \file
 *
 * \brief TC16 related functionality implementation.
 (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 */

/**
 * \addtogroup doc_driver_tc16
 *
 * \section doc_driver_tc16_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <tc16.h>
#include <utils.h>
#include <util/atomic.h>
/**
 * \brief Initialize TIMER_0 interface
 *
 * \return Initialization status.
 */

//const uint16_t A4_FREQ = 440;
//const uint16_t C5_FREQ = 523;
//const uint16_t D6_FREQ = 1175;
//
//// OCR1A values
//#define A4 (F_CPU / (A4_FREQ * TIMER1_PRESCALER * 2) - 1)
//#define C5 (F_CPU / (C5_FREQ * TIMER1_PRESCALER * 2) - 1)
//#define D6 (F_CPU / (D6_FREQ * TIMER1_PRESCALER * 2) - 1)
//
//
//int main(void)
//{
	//// Set OC1A as output pin
	//DDRB = (1 << PINB1);
//
	//// Set Timer1, mode CTC, toggle on compare, prescale 8
	//TCCR1A = (1 << COM1A0);
	//TCCR1B = (1 << WGM12)|(1 << CS10);;
//
	//while (1)
	//{
		//// play A4 for 0.5 sec
		//OCR1A = A4;
		//_delay_ms(500);
//
		//// play C5 for 0.5 sec
		//OCR1A = C5;
		//_delay_ms(500);
//
		//// play D6 for 0.5 sec
		//OCR1A = D6;
		//_delay_ms(500);
//
	//}
//}

volatile uint32_t ms = 0;

int8_t TIMER_1_init()
{

	/* Enable TC1 */
	PRR0 &= ~(1 << PRTIM1);

	 TCCR1A = (0 << COM1A1) | (0 << COM1A0) /* Normal port operation, OCA disconnected */
			 | (0 << COM1B1) | (0 << COM1B0) /* Normal port operation, OCB disconnected */
			 | (0 << WGM11) | (0 << WGM10); /* TC16 Mode 4 Normal / CTC, depends on WGM13 and WGM12 on TCCR1B*/

	TCCR1B = (0 << WGM13) | (1 << WGM12)                /* TC16 Mode 4 CTC */
	         | 0 << ICNC1                               /* Input Capture Noise Canceler: disabled */
	         | 0 << ICES1                               /* Input Capture Edge Select: disabled */
#if F_CPU == 8000000 // divice by 64 // effective clock rate is 125KHz
	         | (0 << CS12) | (1 << CS11) | (1 << CS10); /* IO clock divided by 64 */
#elif F_CPU == 16000000 // divide by 256 // effective clock rate is 62500 Hz
	         | (1 << CS12) | (0 << CS11) | (0 << CS10); /* IO clock divided by 256 */
#else
#error "Invalid F_CPU value."
#endif

	// ICR1 = 0x0; /* Input capture value, used as top counter value in some modes: 0x0 */

	// OCR1B = 0x0; /* Output compare B: 0x0 */

	// GTCCR = 0 << TSM /* Timer/Counter Synchronization Mode: disabled */
	//		 | 0 << PSRASY /* Prescaler Reset Timer/Counter2: disabled */
	//		 | 0 << PSRSYNC; /* Prescaler Reset: disabled */

	// Enable the interrupt
	TIMSK1 = 0 << OCIE1B   /* Output Compare B Match Interrupt Enable: disabled */
	         | 1 << OCIE1A /* Output Compare A Match Interrupt Enable: enabled */
	         | 0 << ICIE1  /* Input Capture Interrupt Enable: disabled */
	         | 0 << TOIE1; /* Overflow Interrupt Enable: disabled */

// Consider using this:
// TOP(OCRnx) = F_CPU / ((desiredFrequency * 2 * N) - 1) // N is the clock prescaler value
// desiredFreq = F_CPU / (2 * N * (1 + OCRnx))
   // Set the compare value
#if F_CPU == 8000000UL
   	OCR1A = ((F_CPU / 64UL) / 1000UL); // Count to 125
#elif F_CPU == 16000000UL
   	OCR1A = ((F_CPU / 256UL) / (1000UL * 0.5)); // Count to 125
#endif
	return 0;
}

uint32_t millis()
{
	uint32_t mil = 0;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		mil = ms;
	}
	return(mil);
}

ISR(TIMER1_COMPA_vect)
{
	++ms;
}

//void PauseTimer()
//{
	//TIMSK0 &= ~(_BV(OCIE0A));
	//power_timer0_disable();
//}
//
//void ResumeTimer()
//{
	//power_timer0_enable();
	//TIMSK0 |= _BV(OCIE0A);
//}
//
//void ResetTimer()
//{
	//ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	//{
		//ms = 0;
	//}
//}
//
//void TimerRemoveDelta(uint32_t rem)
//{
	//ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	//{
		//if(rem > ms)
		//{
			//ms = 0;
		//}
		//else
		//{
			//ms -= rem;
		//}
	//}
//}
//
//void TimerAddOffset(uint32_t add)
//{
	//ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	//{
		//ms += add;
	//}
//}
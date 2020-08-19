/* 
* ADC.cpp
*
* Created: 5/27/2020 2:58:01 PM
* Author: MQUEZADA
*/

#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include "ADC.h"

volatile uint16_t adc1Value = 0;

aDC adc;

// default constructor
aDC::aDC()
{
} //aDC

// default destructor
aDC::~aDC()
{
} //~aDC

ISR(ADC_vect)
{
   // Read low first to lock the registers
   uint8_t low(ADCL);
   adc1Value = (ADCH << 8) | low;

   // Trigger the next conversion
   ADCSRA |= _BV(ADSC);
}

void aDC::Init()
{
   // Enable the power to the ADC
   PRR0 &= ~(_BV(PRADC));

   //ADCSingleEndedMode = true;
   // Disable the digital input buffer on the
   // pins we are using for analog input since
   // we won't be using the digital input
   // functionality on these pins.
   // The datasheet recommends this in order
   // to save some extra power.
   DIDR0 |= _BV(ADC1D);// | _BV(ADC6D);

   // Select reference voltage. For this application
   // we use the voltage at VCC as reference (REFSx)
   // REFS1  REFS0       SOURCE
   //--------------------------
   //   0      0        AREF, Internal Vref turned off
   //   0      1        AVCC with external 0.1uF capacitor at AREF pin
   //   1      0        Reserved
   //   1      1        Internal 1.1V Voltage reference with externa; cap at AREF pin
   ADMUX &= ~((_BV(REFS1)) | _BV(REFS0));

   // Single ended input channel selection.
   // Single ended input on channel ADC1 (MUXx)
   ADMUX |= _BV(MUX0);// | _BV(MUX1) | _BV(MUX2);

   // Auto trigger source selection
   // Free running mode
   ADCSRB &= ~(_BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0));

   // Right adjusted input (ADLAR)
   ADCSRB &= ~(_BV(ADLAR));

   // ADC auto trigger enable/disable
   ADCSRA &= ~(_BV(ADATE)); // Disabled

   // The datsheet recommends an ADC frequency between
   // 50 KHz and 200 KHz for 10 bit resolution conversion.
   // ADC clock can be higher for 8 bit samples.
#if F_CPU == 1000000UL
   ADCSRA |= _BV(ADPS1) | _BV(ADPS0); // divide by 8 -> 125KHz
#elif F_CPU == 8000000UL
   ADCSRA |= _BV(ADPS2) | _BV(ADPS1); // divide by 64 -> 125KHz
#elif F_CPU == 16000000UL
   ADCSRA |= _BV(ADPS2) | (ADPS1) | _BV(ADPS0); // divide by 128 -> 125KHz
#else
   #error "Please define a valid ADC clock prescaler value for your main clock frequency."
#endif

   // Enable the ADC
   // Disable ADC interrupt
   ADCSRA |= _BV(ADEN);// | _BV(ADIE);

   // Start conversion
   // This will dump the first reading
   // which is usually not right.
   ADCSRA |= _BV(ADSC);
   while(ADCSRA & _BV(ADIF));
}

uint16_t aDC::Read()
{
   uint16_t val = 0;
   if(bit_is_set(ADCSRA, ADIE))
   {
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
      {
         val = adc1Value;
      }
   }
   else
   {
      // Start a conversion
      ADCSRA |= _BV(ADSC);
      // Wait for conversion
      while(ADCSRA & _BV(ADIF));
      ADCSRA |= _BV(ADIF);
      return(ADCL | (ADCH << 8));
   }
   return(val);
}

void aDC::Sleep()
{
   // Disable ADC
   ADCSRA &= ~(_BV(ADEN) | _BV(ADIE));
   // Remove power
   PRR0 |= _BV(PRADC);
}
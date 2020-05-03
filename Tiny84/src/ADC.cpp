/*
 * ADC.cpp
 *
 * Created: 5/3/2020 1:11:13 AM
 *  Author: MQUEZADA
 */ 

#include <avr/interrupt.h>
#include <util/atomic.h>
#include "ADC.h"

ADC84 adc;

void ADC84::ConfigureADCSingleEnded()
{
	// Select reference voltage. For this application
	// we use the voltage at VCC as reference (REFSx)
	//ADMUX &= ~((_BV(REFS1)) | _BV(REFS0));
    ADMUX = (0 << REFS1) |     // Sets ref. voltage to Vcc, bit 1
			(0 << REFS0) |     // Sets ref. voltage to Vcc, bit 0
			(0 << MUX5)  |     // use ADC3 for input (PA3), MUX bit 5
			(0 << MUX4)  |     // use ADC3 for input (PA3), MUX bit 4
			(0 << MUX3)  |     // use ADC3 for input (PA3), MUX bit 3
			(0 << MUX2)  |     // use ADC3 for input (PA3), MUX bit 2
			(1 << MUX1)  |     // use ADC3 for input (PA3), MUX bit 1
			(1 << MUX0);       // use ADC3 for input (PA3), MUX bit 0
			
	// Single ended input channel selection.
	// Single ended input on channel ADC0 (MUXx)
	//ADMUX &= ~(_BV(MUX0) | _BV(MUX1) | _BV(MUX2));
	//ADMUX |= _BV(MUX0) | _BV(MUX1) | _BV(MUX2);

	// Auto trigger source selection
	// Free running mode
	ADCSRB &= ~(_BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0));

	// Right adjusted input (ADLAR)
	ADCSRB &= ~(_BV(ADLAR));

	// ADC auto trigger source selection disabled
	ADCSRA &= ~(_BV(ADATE));

	// Disable the digital input buffer on the
	// pins we are using for analog input since
	// we won't be using the digital input
	// functionality on these pins.
	// The datasheet recommends this in order
	// to save some extra power.
	DIDR0 |= _BV(ADC3D);// | _BV(ADC6D);

	// Enable the ADC
	// Disable ADC interrupt
	ADCSRA = _BV(ADEN) |
	         _BV(ADIE) |
	         (1 << ADPS2) |     // set prescaler to 64, bit 2
	         (1 << ADPS1) |     // set prescaler to 64, bit 1
	         (0 << ADPS0);      // set prescaler to 64, bit 0

	// Start conversion
	ADCSRA |= _BV(ADSC);
	//serOutln("GO ADC!");
}



void ADC84::begin()
{
	ConfigureADCSingleEnded();
}

uint16_t ADC84::Read()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
	   ADCSRA |= (1 << ADSC);         // start ADC measurement
	   while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
	   
	   uint8_t low(ADCL);
	   uint16_t val = (ADCH << 8) | low;
	   return(val);
	}
	return(0);
}

void ADC84::Sleep()
{
	
}

ISR(ADC_vect)
{
	// Read low first to lock the registers
	//uint8_t low(ADCL);
	//uint16_t val = (ADCH << 8) | low;

	//serOut("ADC!");

	// Grab the mux bits
	uint8_t mux = (ADMUX & 0x3F);
	//serOut(ADMUX);
	//serOut(mux);
	switch(mux)
	{
		case(0):
		break;
		case(1):
		break;
		case(2):
		break;
		case(3):
		break;
		case(4):
		break;
		case(5):
		break;
		case(6):
		//adc6 = val;
		//serOut("6: ");
		//serOut(adc6);
		// Clear the mux bits but leave
		// the 2 high bits (refs)
		//ADMUX &= 0xC0;
		// Select channel 7
		//ADMUX |= 7;
		break;
		case(7):
		//adc7 = val;
		//serOut("7: ");serOut(adc7);
		// Set to channel 6
		//ADMUX &= 0xC0;
		//ADMUX |= 6;
		break;
		default:
		break;
	}
	//serOut(ADMUX);
	// Trigger the next conversion
	ADCSRA |= _BV(ADSC);
}


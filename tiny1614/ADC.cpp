/*
 * ADC.cpp
 *
 * Created: 5/3/2020 1:11:13 AM
 *  Author: MQUEZADA
 */ 

#include "ADC.h"

#define AIN6 6

ADC84 adc;

void ADC84::ConfigureADCSingleEnded(uint8_t channel)
{
   // Disable the digital input buffer
   PORTA.PIN6CTRL &= ~PORT_ISC_gm;
   PORTA.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;

   // Set the reference voltage
   VREF.CTRLA = VREF_ADC0REFSEL_4V34_gc; // 4.34 V

   // Do not accumulate samples
   ADC0.CTRLB = ADC_SAMPNUM_ACC1_gc;

   // Set the reference voltage source and
   // clock prescaling
   ADC0.CTRLC = ADC_REFSEL_INTREF_gc | ADC_PRESC_DIV16_gc;

   // Connect the ADC source
   ADC0.MUXPOS = channel == 0xFF ? AIN6 : channel;

   ADC0.CTRLA = 1 << ADC_ENABLE_bp     /* ADC Enable: enabled */
               | 0 << ADC_FREERUN_bp  /* ADC Freerun mode: disabled */
               | ADC_RESSEL_10BIT_gc  /* 10-bit mode */
               | 0 << ADC_RUNSTBY_bp; /* Run standby mode: disabled */

}

void ADC84::begin(uint8_t channel)
{
	//PRR &= ~(_BV(PRADC)); // Power up the ADC
	ConfigureADCSingleEnded(channel);
}

void ADC84::StartConversion(uint8_t channel)
{
   if(channel != 0xFF)
   {
      ADC0.MUXPOS = channel;
   }
   ADC1.COMMAND = ADC_STCONV_bm;
}

bool ADC84::Ready()
{
   return(ADC0.INTFLAGS & ADC_RESRDY_bm);
}

uint16_t ADC84::Get()
{
   return(ADC0.RES);
}

uint16_t ADC84::Read(uint8_t channel)
{
   uint16_t res(0);

   // Change channel if specified
   // or use previously selected channel
   if(channel != 0xFF)
	{
      ADC0.MUXPOS = channel;
   }
   // Start conversion
   ADC1.COMMAND = ADC_STCONV_bm;
   // Wait for conversion
   while(Ready() == false)
   {}
   // Get result from register
   res = ADC0.RES;
   // Clear ready flag.
   ADC0.INTFLAGS |= ADC_RESRDY_bm;// Not sure if we really need to do this...

	return(res);
}

void ADC84::Sleep()
{
	ADC0.CTRLA &= ~ADC_ENABLE_bm;                   // ADC off
	//PRR |= _BV(PRADC);
}

/*
 * ADC.h
 *
 * Created: 4/22/2020 10:51:59 AM
 *  Author: MQUEZADA
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <avr/io.h>

class ADC84
{
	public:
	   void begin(uint8_t channel = 0xFF);

	   uint16_t Read(uint8_t channel = 0xFF);

      void StartConversion(uint8_t channel = 0xFF);
      bool Ready();
      uint16_t Get();

	   void Sleep();
	   
	private:
	   void ConfigureADCSingleEnded(uint8_t channel);
};

extern ADC84 adc;

#endif /* ADC_H_ */
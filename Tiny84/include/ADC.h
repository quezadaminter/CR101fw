/*
 * ADC.h
 *
 * Created: 4/22/2020 10:51:59 AM
 *  Author: MQUEZADA
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

class ADC84
{
	public:
	   void begin();
	   uint16_t Read();
	   void Sleep();
	   
	private:
	   void ConfigureADCSingleEnded();
};

extern ADC84 adc;

#endif /* ADC_H_ */
/*
 * PWM.h
 *
 * Created: 4/22/2020 10:52:11 AM
 *  Author: MQUEZADA
 */ 

#include <stdint.h>
//#include "Pin.h"

#ifndef PWM_H_
#define PWM_H_

class PWM
{
	public:
	   void begin();//Pin &pin);
	   void setLevel(uint16_t dc);//Pin &pin, uint8_t dutyCycle);
	   void Sleep();
	   void Stop();
	   void Start();
};

extern PWM pwm;

#endif /* PWM_H_ */
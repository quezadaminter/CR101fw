/*
 * PWM.h
 *
 * Created: 4/22/2020 10:52:11 AM
 *  Author: MQUEZADA
 */ 

#include <stdint.h>

#ifndef PWM_H_
#define PWM_H_

class PWM
{
	public:
	   void begin(uint8_t pin);
	   void setLevel(uint8_t pin, uint8_t dutyCycle);
	   void Sleep();
};

extern PWM pwm;

#endif /* PWM_H_ */
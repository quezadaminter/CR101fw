/*
 * PWM.h
 *
 * Created: 4/22/2020 10:52:11 AM
 *  Author: MQUEZADA
 */ 

#include <stdint.h>
#include "Pin.h"

#ifndef PWM_H_
#define PWM_H_

// Disable the timer's clock source
#define PWM_OFF TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10);
//#define PWM_FULL TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10);

// Enable the timer's clock source
#define PWM_ON (TCCR1B = (1<<CS11) | (1<<CS10));

class PWM
{
	public:
	   void begin(Pin &pin);
	   void setLevel(Pin &pin, uint8_t dutyCycle);
	   void Sleep();
	   void Stop();
	   void Start();
};

extern PWM pwm;

#endif /* PWM_H_ */
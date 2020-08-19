/*
 * Timer0.h
 *
 * Created: 4/22/2020 9:47:19 AM
 *  Author: MQUEZADA
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_

#include <stdint.h>

class Timer0
{
	public:
	   void begin();
       uint32_t millis();
	   void PauseTimer();
	   void ResumeTimer();
	   void ResetTimer();
	   void TimerRemove(uint32_t rem);
	   void TimerAdd(uint32_t add);
	   uint32_t micros(); // As a fraction of the millis!
	   void Sleep();
	   
	private:
};

extern Timer0 timer0;

#endif /* TIMER0_H_ */
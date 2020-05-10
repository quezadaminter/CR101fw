/*
 * PWM.cpp
 *
 * Created: 5/3/2020 2:39:55 AM
 *  Author: MQUEZADA
 */ 
#include "PWM.h"
#include <avr/power.h>

PWM pwm;

//https://www.marcelpost.com/wiki/index.php/ATtiny84_PWM
//// Example #1  PWM on pin PB2 (OC0A which is on 8-bit timer0)
//// For 8MHz clock: 61.2kHz period (16.25ms) of which half is on and half is off
//DDRB = 1<<DDB2;       // make OC0A (DDB2) PWM output pin
//TCCR0A = (1<<COM0A1) | (1<<COM0B0) | (1<<WGM00);  // Clear OC0A/OC0B on Compare Match (bit 7 + 6)
//// PWM, Phase Correct
//TCCR0B = (1<<CS02);   // clkI/O/256(from prescaler)
//// not used // TIMSK1=0B00000010;    //enable output compare interrupt for OCR1A
//OCR0A = 127;          // set 50% duty cycle (0.5*256)
//
//
//// Example #2  PWM on pin PA6 (OC1A which is on 16-bit Timer1)
//// For 8MHz clock: 61.2kHz period (16.25ms) of which half is on and half is off
//DDRA = 1<<DDA6;       // make OC1A (DDA6) PWM output pin
//TCCR1A = (1<<COM1A1) | (1<<COM1B0) | (1<<WGM00);  // Clear OC1A/OC1B on Compare Match (bit 7 + 6)
//// PWM, Phase Correct
//TCCR1B = (1<<CS02);   // 256 prescaler
//OCR1A = 127;          // set 50% duty cycle (0.5*256)

void PWM::begin(Pin &pin)
{
	PRR &= ~(_BV(PRTIM1));
	//https://andreasrohner.at/posts/Electronics/How-to-set-the-PWM-frequency-for-the-Attiny84/
	pin.SetOutput();
	TCCR1A = (1<<COM1B1)|(1<<WGM10);  // mode #1, OC1B pin, Phase correct 8 bit, TOP = 255
	TCCR1B = (1<<CS11) | (1<<CS10);  // div64 (any speed would do)
	//OCR1B = 0;          // set 23% duty cycle 0.23*256
	setLevel(pin, 0);
}

void PWM::setLevel(Pin &pin, uint8_t dutyCycle)
{
	OCR1B = dutyCycle;
	if(dutyCycle == 0)
	{
		// timer off, set output low.
		PWM_OFF;
		pin.Set(LOW);
	}
	else if(dutyCycle == 255)
	{
		// timer off, set output high.
		PWM_OFF;
		pin.Set(HIGH);
	}
	else
	{
		PWM_ON;
	}
}

void PWM::Start()
{
	PWM_ON;
}

void PWM::Stop()
{
	PWM_OFF;
}

void PWM::Sleep()
{
	PRR |= _BV(PRTIM1);
}
/*
 * Tiny84.cpp
 *
 * Created: 4/21/2020 8:42:14 PM
 * Author : MQUEZADA
 */ 

#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/power.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include "TinyWireS.h"
#include "Timer0.h"
#include "ADC.h"
#include "PWM.h"
#include "Registers.h"

//#define ORANGE_PIN PINA7
//#define GREEN_PIN PINB0
//#define RED_PIN PINB1
//#define WHITE_PIN PORTA5
//#define DATA_READY_PIN PORTB2

Pin WHITE_PIN(&DDRA, &PORTA, &PINA, 5);
Pin ORANGE_PIN(&DDRA, &PORTA, &PINA, 7);
Pin GREEN_PIN(&DDRB, &PORTB, &PINB, 0);
Pin RED_PIN(&DDRB, &PORTB, &PINB, 1);
Pin DATA_READY_PIN(&DDRB, &PORTB, &PINB, 2);

#define DATA_READY_INPUT PORTB &= ~(_BV(DATA_READY_PIN)); DDRB &= ~(_BV(DATA_READY_PIN));
#define DATA_READY_OUTPUT PORTB &= ~(_BV(DATA_READY_PIN)); DDRB |= _BV(DATA_READY_PIN);

uint32_t now = 0;

uint32_t whiteLEDHold = 0;
volatile int8_t scrollClicks = 0;

volatile uint8_t i2c_regs[T84_REG_RANGE] = { 0 };

// Tracks the current register pointer position
volatile uint8_t reg_position = 0;
const uint8_t reg_size = sizeof(i2c_regs);

#define DB 50
uint16_t a,b,c,pa,pb,pc,avg;
int16_t da, db, dc;
int8_t ca, cb, cc;

// Return the data obtained from the
// operation selected by the register pointer.
void masterWantsData() // handles a master read operation:: requestFrom()
{
	TinyWireS.send(i2c_regs[reg_position]);
	if(reg_position == T84_REG_SCROLL_CLICKS)
	{
		// Sent the scroll clicks, clear them
		i2c_regs[T84_REG_SCROLL_CLICKS] = 0;
	}
	// Increment the reg position on each read, and loop back to zero
	reg_position++;
	if (reg_position >= reg_size)
	{
		reg_position = 0;
	}
}

// Move the pointer to the desired register
// so the dedicated code can execute.
void masterSentData(uint8_t howManyBytes) // handles a master write operation
{
	if (howManyBytes == 0)
	{
		// Sanity-check
		return;
	}
	if (howManyBytes > TWI_RX_BUFFER_SIZE)
	{
		// Also insane number
		return;
	}

	reg_position = TinyWireS.receive();
	howManyBytes--;
	if (!howManyBytes)
	{
		// This write was only to set the buffer for next read
	}
	else
	{
		while(howManyBytes--)
		{
			i2c_regs[reg_position] = TinyWireS.receive();
			reg_position++;
			if (reg_position >= reg_size)
			{
				reg_position = 0;
			}
		}
	}
}

void InitializeIO()
{
   ORANGE_PIN.SetOutput();
   GREEN_PIN.SetOutput();
   RED_PIN.SetOutput();
   DATA_READY_PIN.SetOutput();
  
   adc.begin();
   timer0.begin(); 
   pwm.begin(WHITE_PIN);

   ORANGE_PIN.Set(LOW);
   GREEN_PIN.Set(LOW);
   RED_PIN.Set(LOW);
   DATA_READY_PIN.Set(HIGH);
	
   TinyWireS.begin(T84_I2C_SLAVE_ADDRESS);
   TinyWireS.onReceive(masterSentData);
   TinyWireS.onRequest(masterWantsData);
}

void Sleep()
{
	adc.Sleep();
	timer0.PauseTimer();
	pwm.Sleep();
	//https://www.avrfreaks.net/forum/solved-low-power-when-when-sinking-current
	////Flash(4, 100, 10);
	//// Set pins to high impedance inputs
	//// to save power and hold the line
	//// so there are no fake button presses.
	//pulse.SetInput();
	//headUp.SetInput();
	//zeroG.SetInput();
	//flat.SetInput();
	//status.SetInput();
//
	//ADCSRA &= ~_BV(ADEN);                   // ADC off
	//ACSR |= _BV(ACD);                       // Disable comparator
	//MCUCR |= _BV(BODS) | _BV(BODSE);          //turn off the brown-out detector
//
	//set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement
	//sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
	//sleep_mode();                            // sleep
	//sleep_disable();                        // Clear SE bit
//
	//InitializeIO();
	//sei();                                  // Enable interrupts
}

uint16_t readTheOne(uint8_t ch)
{
	PORTA |= _BV(PORTA0 + ch);//(ch, HIGH);
	uint16_t d = adc.Read();//readChannel(0);
	PORTA &= ~(_BV(PORTA0 + ch));//(ch, LOW);
	//Serial.print(d);
	tws_delay(10);
	return(d);
}

void SignalUpdate()
{
	// Let the master know there is data ready to be sent.
	DATA_READY_PIN.Set(HIGH);
	DATA_READY_PIN.Set(LOW);
	tws_delay(1);
	DATA_READY_PIN.Set(HIGH);
}

int16_t delta = 0;
void CheckWhiteLEDLevel(uint32_t now)
{
	delta = i2c_regs[T84_REG_WHITE_PWM_VALUE] - OCR1B;
	if(delta != 0)
	{
		// Move the brightness one step at a time
		// at a rate of 256 steps per 4096 milliseconds.
		// 4096 / 256 = 16ms between PWM steps.
		if(now % 16 == 0)
		{
		   OCR1B += (delta > 0 ? 1 : -1);
		   if(OCR1B == 255)
		   {
			   //WHITE_PWM_FULL;
			   pwm.setLevel(WHITE_PIN, 255);
			   whiteLEDHold = now + 5000;
		   }
		   else if(OCR1B == 0)
		   {
			   pwm.setLevel(WHITE_PIN, 0);
			   //WHITE_PWM_OFF;
		   }
		}
	}
	else
	{
		// Reached the target.
		if(i2c_regs[T84_REG_WHITE_PWM_VALUE] == 255)
		{
			// At full...
		   if(now > whiteLEDHold)
		   {
			   // ... then fade them off.
			   i2c_regs[T84_REG_WHITE_PWM_VALUE] = 0;
			   //WHITE_PWM_ON;
			   pwm.Start();
		   }
		}
	}
	
}

void CheckScrollWheel()
{
	a = readTheOne(1);
	//Serial.print(DELIM);
	b = readTheOne(2);
	//Serial.print(DELIM);
	c = readTheOne(3);
	//Serial.println();
	
	da = a - pa;
	db = b - pb;
	dc = c - pc;
	
	avg = (a + b + c) / 3;
	
	if(abs(a - avg) < (DB * 0.5) &&
	abs(b - avg) < (DB * 0.5) &&
	abs(c - avg) < (DB * 0.5))
	{
		pa = a;
		pb = b;
		pc = c;
	}
	else
	{
		if(abs(da) > DB)// || da < -DB)
		{
			pa = a; // Change resolution in steps of DB
			ca = da > 0 ? 1 : -1; //Change direction
		}
		
		if(abs(db) > DB)// || db < -DB)
		{
			pb = b;
			cb = db > 0 ? 1 : -1;
		}
		
		if(abs(dc) > DB)// || dc < -DB)
		{
			pc = c;
			cc = dc > 0 ? 1 : -1;
		}
		
		if((ca == 0  && cb == 1  && cc == -1) ||
		(ca == 1  && cb == -1 && cc == 0) ||
		(ca == -1 && cb == 0  && cc == 1))
		{
			i2c_regs[T84_REG_SCROLL_CLICKS] += 1;
			SignalUpdate();
			//Serial.print("CW:  ");Serial.print(a);Serial.print(DELIM);Serial.print(b);Serial.print(DELIM);Serial.println(c);
		}
		else if((ca == 1  && cb == 0  && cc == -1) ||
		(ca == -1 && cb == 1  && cc == 0) ||
		(ca == 0  && cb == -1 && cc == 1))
		{
			i2c_regs[T84_REG_SCROLL_CLICKS] -= 1;
			SignalUpdate();
			//Serial.print("CCW: ");Serial.print(a);Serial.print(DELIM);Serial.print(b);Serial.print(DELIM);Serial.println(c);
		}
	}
	ca = cb = cc = 0;
}

int main(void)
{
    /* Replace with your application code */
   InitializeIO();

	for(uint8_t i = 0; i < 255; ++i)
	{
		//OCR1B = i;
		pwm.setLevel(WHITE_PIN, i);
		_delay_ms(5);
	}
	for(uint8_t i = OCR1B; i > 1; --i)
	{
		//OCR1B--;
		pwm.setLevel(WHITE_PIN, i);
		_delay_ms(5);
	}
	pwm.setLevel(WHITE_PIN, 0);
	sei();

   uint32_t s = 0;
    while (1) 
    {
		now = timer0.millis();
		//if(now % 20000 == 0)
		//{
			//if(now > s)
			//{
			   //reg_position = T84_REG_WHITE_PWM_VALUE;
			   //s = now + 200;
			   ////PORTA ^= _BV(WHITE_PIN);
			   //i2c_regs[T84_REG_WHITE_PWM_VALUE] = 255;
			//}
		//}
		TinyWireS_stop_check();// Check for a stop condition here.
		switch(reg_position)
		{
			case(T84_REG_LED_STATE):
               ORANGE_PIN.Set(bit_is_set(i2c_regs[reg_position], T84_RED_BIT) ? LOW : HIGH);
               RED_PIN.Set(bit_is_set(i2c_regs[reg_position], T84_RED_BIT) ? LOW : HIGH);
               GREEN_PIN.Set(bit_is_set(i2c_regs[reg_position], T84_RED_BIT) ? LOW : HIGH);
			   reg_position = T84_REG_ID;
			   break;
			case(T84_REG_WHITE_PWM_VALUE):
			   pwm.Start();
			   //WHITE_PWM_ON;
			   if(whiteLEDHold > now)
			   {
				   // In the middle of the hold,
				   // extend it again.
				   whiteLEDHold += 5000;
			   }
			   reg_position = T84_REG_ID;
			   break;
			case(T84_REG_SLEEP):
			   // Go to sleep!
			   Sleep();
			  
			   // Set interrupt pin to listen
			   MCUCR |= _BV(ISC01); // Falling edge triggers INT0.
	           GIMSK |= _BV(INT0); // Enable INT0 interrupt
			   
	           //GIMSK |= _BV(PCIE); // Enable Pin change interrupts
	           // Monitor the zeroG pin to wake us up.
	           //PCMSK |= _BV(zeroG.Nip) | _BV(flat.Nip);
			   
			   // Set to wake on interrupt.
			   // Sleep.
			   //power_all_disable();
			   // Wake up...
			   // Disable interrupt, tell master we are awake!
			   GIMSK &= ~(_BV(INT0));
			   SignalUpdate();
			   break;
		}
		
		CheckWhiteLEDLevel(now);
    }
}


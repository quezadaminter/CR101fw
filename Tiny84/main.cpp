/*
 * Tiny84.cpp
 *
 * Created: 4/21/2020 8:42:14 PM
 * Author : MQUEZADA
 */ 

#include <avr/io.h>
#include "TinyWireS.h"
#include "Timer0.h"

#define I2C_SLAVE_ADDRESS 0x4 // the 7-bit address (remember to change this when adapting this example)

typedef enum
{
	REG_ID,
	REG_ORANGE_STATE,
	REG_RED_GREEN_STATE,
	REG_WHITE_PWM_VALUE,
	REG_SCROLL_CLICKS,
	REG_SLEEP,
	REG_RANGE
} Registers;

#define ORANGE_BIT 0
#define RED_BIT 1
#define GREEN_BIT 2
#define RG_OFF_BIT 3

#define ORANGE_PIN PINB1
#define RED_GREEN_PIN PINB2
#define WHITE_PIN PORTA5

// Enable the timer's clock source
#define WHITE_PWM_ON (TCCR1B = (1<<CS11) | (1<<CS10))
// Disable the timer's clock source
#define WHITE_PWM_OFF (TCCR1B = (0<<CS12) | (0<<CS11) | (0<<CS10))

volatile uint8_t whitePWMValue = 0;
volatile uint8_t oRGState = 0;
volatile int8_t scrollClicks = 0;

volatile uint8_t i2c_regs[REG_RANGE] = { 0 };

// Tracks the current register pointer position
volatile uint8_t reg_position = 0;
const uint8_t reg_size = sizeof(i2c_regs);

// Return the data obtained from the
// operation selected by the register pointer.
void masterWantsData() // handles a master read operation:: requestFrom()
{
	TinyWireS.send(i2c_regs[reg_position]);
	if(reg_position == REG_SCROLL_CLICKS)
	{
		// Sent the scroll clicks, clear them
		i2c_regs[REG_SCROLL_CLICKS] = 0;
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

//https://andreasrohner.at/posts/Electronics/How-to-set-the-PWM-frequency-for-the-Attiny84/
void ConfigurePWM()
{	
	DDRA |= _BV(WHITE_PIN);
	TCCR1A = (1<<COM1B1)|(1<<WGM10);  // mode #1, OC1B pin, Phase correct 8 bit, TOP = 255
	TCCR1B = (1<<CS11) | (1<<CS10);  // div64 (any speed would do)
	OCR1B = 59;          // set 23% duty cycle 0.23*256
}

void ConfigureADCSingleEnded()
{
	// Select reference voltage. For this application
	// we use the voltage at VCC as reference (REFSx)
	ADMUX &= ~((_BV(REFS1)) | _BV(REFS0));

	// Single ended input channel selection.
	// Single ended input on channel ADC0 (MUXx)
	ADMUX &= ~(_BV(MUX0) | _BV(MUX1) | _BV(MUX2));
	//ADMUX |= _BV(MUX0) | _BV(MUX1) | _BV(MUX2);

	// Auto trigger source selection
	// Free running mode
	ADCSRB &= ~(_BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0));

	// Right adjusted input (ADLAR)
	ADCSRB &= ~(_BV(ADLAR));

	// ADC auto trigger source selection
	ADCSRA &= ~(_BV(ADATE));

	// Disable the digital input buffer on the
	// pins we are using for analog input since
	// we won't be using the digital input
	// functionality on these pins.
	// The datasheet recommends this in order
	// to save some extra power.
	DIDR0 |= _BV(ADC0D);// | _BV(ADC6D);

	// Enable the ADC
	// Enable ADC interrupt
	ADCSRA |= _BV(ADEN) | _BV(ADIE);

	// Start conversion
	ADCSRA |= _BV(ADSC);
	//serOutln("GO ADC!");
}



ISR(ADC_vect)
{
	// Read low first to lock the registers
	uint8_t low(ADCL);
	uint16_t val = (ADCH << 8) | low;

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
		adc6 = val;
		//serOut("6: ");
		//serOut(adc6);
		// Clear the mux bits but leave
		// the 2 high bits (refs)
		//ADMUX &= 0xC0;
		// Select channel 7
		//ADMUX |= 7;
		break;
		case(7):
		adc7 = val;
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

void SignalUpdate()
{
	DDRA |= _BV(PORTA7);
	PORTA &= ~(_BV(PORTA7));
	tws_delay(1);
	DDRA &= ~(_BV(PORTA7)); 
}

uint16_t readChannel(uint8_t ch)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
	   return(analogRead(ch));
	}
}

uint16_t readTheOne(uint8_t ch)
{
	PORTA |= _BV(PORTA0 + ch);//(ch, HIGH);
	uint16_t d = readChannel(0);
	PORTA &= ~(_BV(PORTA0 + ch));//(ch, LOW);
	//Serial.print(d);
	tws_delay(10);
	return(d);
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
			i2c_regs[REG_SCROLL_CLICKS] += 1;
			SignalUpdate();
			//Serial.print("CW:  ");Serial.print(a);Serial.print(DELIM);Serial.print(b);Serial.print(DELIM);Serial.println(c);
		}
		else if((ca == 1  && cb == 0  && cc == -1) ||
		(ca == -1 && cb == 1  && cc == 0) ||
		(ca == 0  && cb == -1 && cc == 1))
		{
			i2c_regs[REG_SCROLL_CLICKS] -= 1;
			SignalUpdate();
			//Serial.print("CCW: ");Serial.print(a);Serial.print(DELIM);Serial.print(b);Serial.print(DELIM);Serial.println(c);
		}
	}
	ca = cb = cc = 0;
}

int main(void)
{
    /* Replace with your application code */
	DDRA &= ~(_BV(PORTA7)); // Scroll interrupt
	
	TinyWireS.begin(I2C_SLAVE_ADDRESS);
	TinyWireS.onReceive(masterSentData);
	TinyWireS.onRequest(masterWantsData);
    while (1) 
    {
		TinyWireS_stop_check();// Check for a stop condition here.
		switch(reg_position)
		{
			case(REG_ORANGE_STATE):
			   if (bit_is_set(i2c_regs[reg_position], ORANGE_BIT))
			   {
				   PORTB &= ~(_BV(ORANGE_PIN));
			   }
			   else
			   {
				   PORTB |= _BV(ORANGE_PIN);
			   }
			   reg_position = REG_ID;
			   break;
			case(REG_RED_GREEN_STATE):
			   if(bit_is_set(i2c_regs[reg_position], RED_BIT))
			   {
				   DDRB |= _BV(RED_GREEN_PIN);
				   PORTB &= ~(_BV(RED_GREEN_PIN));
			   }
			   else if(bit_is_set(i2c_regs[reg_position], GREEN_BIT))
			   {
				   DDRB |= _BV(RED_GREEN_PIN);
				   PORTB |= _BV(i2c_regs[reg_position], RED_GREEN_PIN);
			   }
			   else
			   {
				   PORTB &= ~(_BV(RED_GREEN_PIN));
				   DDRB &= ~(_BV(RED_GREEN_PIN));
			   }
			   reg_position = REG_ID;
			   break;
			case(REG_WHITE_PWM_VALUE):
			   if (i2c_regs[reg_position] == 0)
			   {
				   WHITE_PWM_OFF;
				   PORTA &= ~(_BV(WHITE_PIN));
			   }
			   else if(i2c_regs[reg_position] == 255)
			   {
				   WHITE_PWM_OFF;
				   PORTA |= _BV(WHITE_PIN);
			   }
			   else
			   {
				   OCR1B = i2c_regs[reg_position];
				   WHITE_PWM_ON;
			   }
			   reg_position = REG_ID;
			   break;
			case(REG_SLEEP):
			   // Go to sleep!
			   break;
		}
    }
}


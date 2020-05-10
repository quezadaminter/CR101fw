/*
 * Test328PB.cpp
 *
 * Created: 5/9/2020 3:20:37 PM
 * Author : MQUEZADA
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/eeprom.h>
#include "TWI.h"
#include "Timer1.h"
#include "Tones.h"

#include "Messages.h"
#define SimpleFIFO_NONVOLATILE
#include "SimpleFIFO.h"

//volatile uint32_t ms = 0;
//
//ISR(TIMER1_COMPA_vect)
//{
   //++ms;
//}

//uint32_t millis()
//{
   //uint32_t m;
   //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
   //{
      //m = ms;;
   //}
   //return(m);
//}

//typedef struct s_note
//{
	//uint8_t OCRxn; // 0..255
	//uint8_t N;
//} note_t;
//
//typedef struct s_octave {
	//note_t note_C;
	//note_t note_CS;
	//note_t note_D;
	//note_t note_DS;
	//note_t note_E;
	//note_t note_F;
	//note_t note_FS;
	//note_t note_G;
	//note_t note_GS;
	//note_t note_A;
	//note_t note_AS;
	//note_t note_B;
//} octave_t;
//
//#define	N_1	(_BV(CS00))
//#define	N_8	(_BV(CS01))
//#define	N_64	(_BV(CS01)|_BV(CS00))
//#define	N_256	(_BV(CS02))
//#define	N_1024	(_BV(CS02)|_BV(CS00))
//
//#define C(o)  0,o
//#define CS(o) 1,o
//#define D(o)  2,o
//#define DS(o) 3,o
//#define E(o)  4,o
//#define F(o)  5,o
//#define FS(o) 6,o
//#define G(o)  7,o
//#define GS(o) 8,o
//#define A(o)  9,o
//#define AS(o) 10,o
//#define B(o)  11,o
//
//EEMEM const octave_t octaves[8] =
//{
	//{ // octave 0
	//.note_C = {142, N_256}, // 16.35 Hz
	//.note_CS = {134, N_256}, // 17.32 Hz
	//.note_D = {127, N_256}, // 18.35 Hz
	//.note_DS = {120, N_256}, // 19.45 Hz
	//.note_E = {113, N_256}, // 20.60 Hz
	//.note_F = {106, N_256}, // 21.83 Hz
	//.note_FS = {100, N_256}, // 23.12 Hz
	//.note_G = {95, N_256}, // 24.50 Hz
	//.note_GS = {89, N_256}, // 25.96 Hz
	//.note_A = {84, N_256}, // 27.50 Hz
	//.note_AS = {79, N_256}, // 29.14 Hz
	//.note_B = {75, N_256} // 30.87 Hz
	//},
	//{ // octave 1
	//.note_C = {71, N_256}, // 32.70 Hz
	//.note_CS = {67, N_256}, // 34.65 Hz
	//.note_D = {63, N_256}, // 36.71 Hz
	//.note_DS = {59, N_256}, // 38.89 Hz
	//.note_E = {56, N_256}, // 41.20 Hz
	//.note_F = {53, N_256}, // 43.65 Hz
	//.note_FS = {50, N_256}, // 46.25 Hz
	//.note_G = {47, N_256}, // 49.00 Hz
	//.note_GS = {44, N_256}, // 51.91 Hz
	//.note_A = {42, N_256}, // 55.00 Hz
	//.note_AS = {39, N_256}, // 58.27 Hz
	//.note_B = {37, N_256} // 61.74 Hz
	//},
	//{ // octave 2
	//.note_C = {142, N_64}, // 65.41 Hz
	//.note_CS = {134, N_64}, // 69.30 Hz
	//.note_D = {127, N_64}, // 73.42 Hz
	//.note_DS = {120, N_64}, // 77.78 Hz
	//.note_E = {113, N_64}, // 82.41 Hz
	//.note_F = {106, N_64}, // 87.31 Hz
	//.note_FS = {100, N_64}, // 92.50 Hz
	//.note_G = {95, N_64}, // 98.00 Hz
	//.note_GS = {89, N_64}, // 103.83 Hz
	//.note_A = {84, N_64}, // 110.00 Hz
	//.note_AS = {79, N_64}, // 116.54 Hz
	//.note_B = {75, N_64} // 123.47 Hz
	//},
	//{ // octave 3
	//.note_C = {71, N_64}, // 130.81 Hz
	//.note_CS = {67, N_64}, // 138.59 Hz
	//.note_D = {63, N_64}, // 146.83 Hz
	//.note_DS = {59, N_64}, // 155.56 Hz
	//.note_E = {56, N_64}, // 164.81 Hz
	//.note_F = {53, N_64}, // 174.61 Hz
	//.note_FS = {50, N_64}, // 185.00 Hz
	//.note_G = {47, N_64}, // 196.00 Hz
	//.note_GS = {44, N_64}, // 207.65 Hz
	//.note_A = {42, N_64}, // 220.00 Hz
	//.note_AS = {39, N_64}, // 233.08 Hz
	//.note_B = {37, N_64} // 246.94 Hz
	//},
	//{ // octave 4
	//.note_C = {35, N_64}, // 261.63 Hz
	//.note_CS = {33, N_64}, // 277.18 Hz
	//.note_D = {31, N_64}, // 293.66 Hz
	//.note_DS = {29, N_64}, // 311.13 Hz
	//.note_E = {27, N_64}, // 329.63 Hz
	//.note_F = {26, N_64}, // 349.23 Hz
	//.note_FS = {24, N_64}, // 369.99 Hz
	//.note_G = {23, N_64}, // 392.00 Hz
	//.note_GS = {22, N_64}, // 415.30 Hz
	//.note_A = {20, N_64}, // 440.00 Hz
	//.note_AS = {19, N_64}, // 466.16 Hz
	//.note_B = {18, N_64} // 493.88 Hz
	//},
	//{  // octave 5
	//.note_C = {142, N_8}, // 523.25 Hz
	//.note_CS = {134, N_8}, // 554.37 Hz
	//.note_D = {127, N_8}, // 587.33 Hz
	//.note_DS = {120, N_8}, // 622.25 Hz
	//.note_E = {113, N_8}, // 659.25 Hz
	//.note_F = {106, N_8}, // 349.23 Hz
	//.note_FS = {100, N_8}, // 369.99 Hz
	//.note_G = {95, N_8}, // 392.00 Hz
	//.note_GS = {89, N_8}, // 415.30 Hz
	//.note_A = {84, N_8}, // 440.00 Hz
	//.note_AS = {79, N_8}, // 466.16 Hz
	//.note_B = {75, N_8} // 493.88 Hz
	//},
	//{  // octave 6
	//.note_C = {71, N_8}, // 1046.50 Hz
	//.note_CS = {67, N_8}, // 1108.73 Hz
	//.note_D = {63, N_8}, // 1174.66 Hz
	//.note_DS = {59, N_8}, // 1244.51 Hz
	//.note_E = {56, N_8}, // 1318.51 Hz
	//.note_F = {53, N_8}, // 1396.91 Hz
	//.note_FS = {50, N_8}, // 1479.98 Hz
	//.note_G = {47, N_8}, // 1567.98 Hz
	//.note_GS = {44, N_8}, // 1661.22 Hz
	//.note_A = {42, N_8}, // 1760.00 Hz
	//.note_AS = {39, N_8}, // 1864.66 Hz
	//.note_B = {37, N_8} // 1975.53 Hz
	//},
	//{  // octave 7
	//.note_C = {35, N_8}, // 2093.00 Hz
	//.note_CS = {33, N_8}, // 2217.46 Hz
	//.note_D = {31, N_8}, // 2349.32 Hz
	//.note_DS = {29, N_8}, // 2489.02 Hz
	//.note_E = {27, N_8}, // 2637.02 Hz
	//.note_F = {26, N_8}, // 2793.83 Hz
	//.note_FS = {24, N_8}, // 2959.96 Hz
	//.note_G = {23, N_8}, // 3135.96 Hz
	//.note_GS = {22, N_8}, // 3322.44 Hz
	//.note_A = {20, N_8}, // 3520.00 Hz
	//.note_AS = {19, N_8}, // 3729.31 Hz
	//.note_B = {18, N_8} // 3951.07 Hz
	//}
//};
//
//class queuedNote
//{
	//public:
	   //queuedNote(){}
	   //queuedNote(uint8_t n, uint8_t o, uint32_t l) :
	      //note(n), octave(o), length(l)
 	   //{
 	   //}
		//uint8_t note = 0;
		//uint8_t octave = 0;
		//uint32_t length = 0;
//};
//
//uint8_t octave = 0, note = 0;
//SimpleFIFO<queuedNote, 10> noteQueue;
//bool tonePlaying = false;
//uint32_t noteEnd = 0;
//
//void tone_delay(uint32_t d)
//{
   //while(d > 0)
   //{
      //_delay_ms(1);
      //--d;
   //}
//}
//
//void tone_stop(void)
//{
	//TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00)); // stop the timer
	//tonePlaying = false;
//}
//
//void tone_play_block(uint8_t note, uint8_t octave, uint32_t length)
//{
   ////if (FLASH_0_is_eeprom_ready())
	//{
	   //note_t val;
		////FLASH_0_read_eeprom_block((uint16_t *)&octaves + sizeof(octave_t) * octave + sizeof(note_t) * note, &val, sizeof(note_t));
		////FLASH_0_read_eeprom_block((unsigned int *)(&octaves + sizeof(octave_t) * octave + sizeof(note_t) * note), (uint8_t *)&val, sizeof(note_t));
		//uint16_t address = ((unsigned int)&octaves + sizeof(octave_t) * octave + sizeof(note_t) * note);
		////FLASH_0_read_eeprom_block(address, (uint8_t *)&val, sizeof(note_t));
      //eeprom_read_block(&val, (uint8_t *)address, sizeof(note_t));
		//TCCR0B = (TCCR0B & ~((1<<CS02)|(1<<CS01)|(1<<CS00))) | val.N;
		//OCR0A = val.OCRxn - 1; // set the OCRnx
		//tonePlaying = true;
      //tone_delay(length);
      //tone_stop();
      //tonePlaying = false;
	//}
//}
//
//void tone_play_next()
//{
	////uint32_t ret;
	//queuedNote x = noteQueue.dequeue();
	//note_t val;
	//if (x.length > 0)// && FLASH_0_is_eeprom_ready())
	//{
		////FLASH_0_read_eeprom_block((uint16_t *)&octaves + sizeof(octave_t) * octave + sizeof(note_t) * note, &val, sizeof(note_t));
		////FLASH_0_read_eeprom_block((unsigned int *)(&octaves + sizeof(octave_t) * octave + sizeof(note_t) * note), (uint8_t *)&val, sizeof(note_t));
		//uint16_t address = ((unsigned int)&octaves + sizeof(octave_t) * x.octave + sizeof(note_t) * x.note);
		////FLASH_0_read_eeprom_block(address, (uint8_t *)&val, sizeof(note_t));
      //eeprom_read_block(&val, (uint8_t *)address, sizeof(note_t));
		//TCCR0B = (TCCR0B & ~((1<<CS02)|(1<<CS01)|(1<<CS00))) | val.N;
		//OCR0A = val.OCRxn - 1; // set the OCRnx
		//tonePlaying = true;
		//noteEnd = t1.millis() + x.length;
	//}
//}
//
//void tone_update(uint32_t now)
//{
	//if(tonePlaying)
	//{
		//if(now > noteEnd)
		//{
			//if(noteQueue.count() > 0)
			//{
				//tone_play_next();
			//}
			//else
			//{
			   //tone_stop();
			//}
		//}
	//}
	//else if(noteQueue.count() > 0)
	//{
		//tone_play_next();
	//}
//}
//
//void tone_play(uint8_t note, uint8_t octave, uint32_t length)
//{
	//noteQueue.enqueue(queuedNote(note, octave, length));
//}

int8_t InitPinChangeInterrupts()
{

	PCICR = (1 << PCIE0) | // Enable pin change interrupt 0
	        (1 << PCIE0) | // Enable pin change interrupt 0
	        (1 << PCIE0) | // Enable pin change interrupt 0
	        (1 << PCIE0) | // Enable pin change interrupt 0
	        (1 << PCIE0) | // Enable pin change interrupt 0
	        (1 << PCIE0) | // Enable pin change interrupt 0
	        (1 << PCIE1) | // Enable pin change interrupt 1
	        (1 << PCIE1) | // Enable pin change interrupt 1
	        (1 << PCIE1) | // Enable pin change interrupt 1
	        (1 << PCIE2) | // Enable pin change interrupt 2
	        (1 << PCIE2) | // Enable pin change interrupt 2
	        (1 << PCIE2) | // Enable pin change interrupt 2
	        (1 << PCIE2) | // Enable pin change interrupt 2
	        (1 << PCIE3);  // Enable pin change interrupt 3

	PCMSK3 = (1 << PCINT27) | // Pin change enable mask 27
            (1 << PCINT26);  // Pin change enable mask 26

	PCMSK2 = (1 << PCINT17) | // Pin change enable mask 17
	         (1 << PCINT20) |  // Pin change enable mask 20
	         (1 << PCINT23);  // Pin change enable mask 23

	PCMSK1 = (1 << PCINT8) |  // Pin change enable mask 8
	         (1 << PCINT10) | // Pin change enable mask 10
	         (1 << PCINT11);  // Pin change enable mask 11

	PCMSK0 = (1 << PCINT0) | // Pin change enable mask 0
	         (1 << PCINT1) | // Pin change enable mask 1
	         (1 << PCINT2) | // Pin change enable mask 2
	         (1 << PCINT3) | // Pin change enable mask 3
	         (1 << PCINT4) | // Pin change enable mask 4
	         (1 << PCINT5);  // Pin change enable mask 5

	return 0;
}

uint8_t pcint0_history = 0;
uint8_t pcint1_history = 0;
uint8_t pcint2_history = 0;
uint8_t pcint3_history = 0;

volatile bool touchEventDetected = false;
uint8_t EVENTS = 0;
uint16_t SWITCH_STATES = 0;

ISR(PCINT0_vect)
{

	/* Insert your pin change 0 interrupt handling code here */
   // Interrupts 0 to 7
   uint8_t changedbits;

   changedbits = PINB ^ pcint0_history;
   pcint0_history = PINB;
   
   if(changedbits & _BV(INT_PLAY_PAUSE))
   {
      bit_is_set(PINB, INT_PLAY_PAUSE) ? (SWITCH_STATES &= ~(_BV(SWITCH_PLAY_PAUSE))) : (SWITCH_STATES |= _BV(SWITCH_PLAY_PAUSE));
   }
   else if(changedbits & _BV(INT_REWIND))
   {
      bit_is_set(PINB, INT_REWIND) ? (SWITCH_STATES &= ~(_BV(SWITCH_REWIND))) : (SWITCH_STATES |= _BV(SWITCH_REWIND));
   }
   else if(changedbits & _BV(INT_C))
   {
      bit_is_set(PINB, INT_C) ? (SWITCH_STATES &= ~(_BV(SWITCH_C))) : (SWITCH_STATES |= _BV(SWITCH_C));
   }
   else if(changedbits & _BV(INT_B))
   {
      bit_is_set(PINB, INT_B) ? (SWITCH_STATES &= ~(_BV(SWITCH_B))) : (SWITCH_STATES |= _BV(SWITCH_B));
   }
   else if(changedbits & _BV(INT_A))
   {
      bit_is_set(PINB, INT_A) ? (SWITCH_STATES &= ~(_BV(SWITCH_A))) : (SWITCH_STATES |= _BV(SWITCH_A));
   }
   else if(changedbits & _BV(INT_VOL_DN))
   {
      bit_is_set(PINB, INT_VOL_DN) ? (SWITCH_STATES &= ~(_BV(SWITCH_VOL_DN))) : (SWITCH_STATES |= _BV(SWITCH_VOL_DN));
   }
}

ISR(PCINT1_vect)
{

	/* Insert your pin change 1 interrupt handling code here */
	// Interrupts 8 to 14
   uint8_t changedbits;

   changedbits = PINC ^ pcint1_history;
   pcint1_history = PINC;
   
   if(changedbits & _BV(INT_ENTER))
   {
      bit_is_set(PINC, INT_ENTER) ? (SWITCH_STATES &= ~(_BV(SWITCH_ENTER))) : (SWITCH_STATES |= _BV(SWITCH_ENTER));
   }
   else if(changedbits & _BV(INT_ZONE))
   {
      bit_is_set(PINC, INT_ZONE) ? (SWITCH_STATES &= ~(_BV(SWITCH_ZONE))) : (SWITCH_STATES |= _BV(SWITCH_ZONE));
   }
   else if(changedbits & _BV(INT_BACK))
   {
      bit_is_set(PINC, INT_BACK) ? (SWITCH_STATES &= ~(_BV(SWITCH_BACK))) : (SWITCH_STATES |= _BV(SWITCH_BACK));
   }
}

ISR(PCINT2_vect)
{

	/* Insert your pin change 2 interrupt handling code here */
	// Interrupts 16 to 23
   uint8_t changedbits;

   changedbits = PIND ^ pcint2_history;
   pcint2_history = PIND;
   
   if(changedbits & _BV(T84_DATA_READY_INT))
   {
	   if(bit_is_clear(PIND, T84_DATA_READY_INT))
	   {
	      touchEventDetected = true;
	   }
   }
   else if(changedbits & _BV(INT_MUSIC))
   {
      bit_is_set(PIND, INT_MUSIC) ? (SWITCH_STATES &= ~(_BV(SWITCH_MUSIC))) : (SWITCH_STATES |= _BV(SWITCH_MUSIC));
   }
   else if(changedbits & _BV(INT_FORWARD))
   {
      bit_is_set(PIND, INT_FORWARD) ? (SWITCH_STATES &= ~(_BV(SWITCH_FORWARD))) : (SWITCH_STATES |= _BV(SWITCH_FORWARD));
   }
}

ISR(PCINT3_vect)
{

	/* Insert your pin change 3 interrupt handling code here */
	// Interrupts 24 to 27
   uint8_t changedbits;

   changedbits = PINE ^ pcint3_history;
   pcint3_history = PINE;
   
   if(changedbits & _BV(INT_VOL_UP))
   {
      bit_is_set(PINE, INT_VOL_UP) ? (SWITCH_STATES &= ~(_BV(SWITCH_VOL_UP))) : (SWITCH_STATES |= _BV(SWITCH_VOL_UP));
   }
   else if(changedbits & _BV(INT_MUTE))
   {
      bit_is_set(PINE, INT_MUTE) ? (SWITCH_STATES &= ~(_BV(SWITCH_MUTE))) : (SWITCH_STATES |= _BV(SWITCH_MUTE));
   }
}
void InitPorts(void)
{
   InitPinChangeInterrupts();
}

uint16_t SWITCH_PAST_STATE = 0;

void CheckSwitchStates()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		uint16_t change = SWITCH_PAST_STATE ^ SWITCH_STATES;

		if(change)
		{
         DDRD |= _BV(5);
         PORTD ^= _BV(5);
			tones.Play(A(4), 200);
			//// Send event register
			////uint8_t event = _BV(SWITCH_EVENT);
         //// Send the PI the switch event.
			//I2C_0_write2ByteRegister(PI_I2C_ADDRESS, PI_EVENT_REGISTER, SWITCH_STATES);
         //// Tell the T84 to light up the white LEDs.
			//I2C_0_write1ByteRegister(T84_I2C_SLAVE_ADDRESS, T84_REG_WHITE_PWM_VALUE, 255);
			
			//if(change & _BV(i))
			//{
			//// Send i2c switch event.
			//if(_BV(SWITCH_STATES) == PRESSED)
			//{
			//}
			tones.Play(A(5), 200);
		}
		SWITCH_PAST_STATE = SWITCH_STATES;
	}
}

uint32_t now = 0;
int main(void)
{
   DDRB = 0xFF;
   PORTB = 0x00;
   DDRD = 0xFF;//|= _BV(1);
   PORTD = 0x00;


   DDRB &= 0B00000011;
   PORTB |= 0B00000011;
   InitPinChangeInterrupts();

   //// ms timer
   //TCCR1A = 0x00;
   //TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
   //OCR1A = 124;
   //TCNT1 = 0;
   //TIMSK1 = (1 << OCIE1A);

   //// Tones
	//DDRD |= _BV(6);
	//PORTD &= ~(_BV(PORTD6));
	//// Set Timer0, mode CTC, toggle on compare, OCR0A sets limit, prescale 8 but will vary according to the table
	//TCCR0A = (1 << COM0A0) | (1 << WGM01);
	//TCCR0B= (1 << CS10);;
	//TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00)); // stop the timer

   twi.Init();
   t1.Init();
   tones.Init();

   sei();

   tones.PlayBlock(4, 5, 200);
   tones.PlayBlock(5, 5, 200);

   uint8_t s = 0;
   uint32_t d = 0;
    /* Replace with your application code */
    while (1) 
    {
      now = t1.millis();

      if(now % 1000 == 0)
      {
         if(now > d)
         {
            d = now + 250;
            PORTD ^= _BV(1);
            //note_t val;
            //uint16_t address = ((unsigned int)&octaves + sizeof(octave_t) * octave + sizeof(note_t) * note);
            //eeprom_read_block(&val, (uint8_t *)address, sizeof(note_t));
            //TCCR0B = (TCCR0B & ~((1<<CS02)|(1<<CS01)|(1<<CS00))) | val.N;
            //OCR0A = val.OCRxn - 1; // set the OCRnx
//
            //++note;
            //if(note > 11)
            //{
               //note = 0;
               //++octave;
               //if(octave > 7)
               //{
                  //octave = 0;
               //}
            //}
            if(s == 0)
            {
               s = 1;
               DDRD |= (_BV(0));
               PORTD |= (_BV(0));
               //tone_play(F(5), 250);
            }
            else if(s == 1)
            {
               s = 2;
               PORTD &= ~(_BV(0));
            }
            else if(s == 2)
            {
               s = 0;
               PORTD &= ~(_BV(0));
               DDRD &= ~(_BV(0));
            }
         }
      }
      CheckSwitchStates();
      tones.Update(now);
    }
}


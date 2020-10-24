/* 
* Tones.cpp
*
* Created: 5/9/2020 11:14:13 PM
* Author: MQUEZADA
*
*  This version uses Timer counter 0
*  and the OCR0A output pin on the ATMega328PB.
*/


#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "Tones.h"
#include "Timer1.h"
#include "PinMap.h"

Tones tones;

typedef struct s_note
{
	uint8_t OCRxn; // 0..255
	uint8_t N;
} note_t;

typedef struct s_octave {
	note_t note_C;
	note_t note_CS;
	note_t note_D;
	note_t note_DS;
	note_t note_E;
	note_t note_F;
	note_t note_FS;
	note_t note_G;
	note_t note_GS;
	note_t note_A;
	note_t note_AS;
	note_t note_B;
} octave_t;

#define	N_1	(_BV(CS00))
#define	N_8	(_BV(CS01))
#define	N_64	(_BV(CS01)|_BV(CS00))
#define	N_256	(_BV(CS02))
#define	N_1024	(_BV(CS02)|_BV(CS00)

EEMEM const octave_t octaves[8] =
{
	{ // octave 0
	.note_C = {142, N_256}, // 16.35 Hz
	.note_CS = {134, N_256}, // 17.32 Hz
	.note_D = {127, N_256}, // 18.35 Hz
	.note_DS = {120, N_256}, // 19.45 Hz
	.note_E = {113, N_256}, // 20.60 Hz
	.note_F = {106, N_256}, // 21.83 Hz
	.note_FS = {100, N_256}, // 23.12 Hz
	.note_G = {95, N_256}, // 24.50 Hz
	.note_GS = {89, N_256}, // 25.96 Hz
	.note_A = {84, N_256}, // 27.50 Hz
	.note_AS = {79, N_256}, // 29.14 Hz
	.note_B = {75, N_256} // 30.87 Hz
	},
	{ // octave 1
	.note_C = {71, N_256}, // 32.70 Hz
	.note_CS = {67, N_256}, // 34.65 Hz
	.note_D = {63, N_256}, // 36.71 Hz
	.note_DS = {59, N_256}, // 38.89 Hz
	.note_E = {56, N_256}, // 41.20 Hz
	.note_F = {53, N_256}, // 43.65 Hz
	.note_FS = {50, N_256}, // 46.25 Hz
	.note_G = {47, N_256}, // 49.00 Hz
	.note_GS = {44, N_256}, // 51.91 Hz
	.note_A = {42, N_256}, // 55.00 Hz
	.note_AS = {39, N_256}, // 58.27 Hz
	.note_B = {37, N_256} // 61.74 Hz
	},
	{ // octave 2
	.note_C = {142, N_64}, // 65.41 Hz
	.note_CS = {134, N_64}, // 69.30 Hz
	.note_D = {127, N_64}, // 73.42 Hz
	.note_DS = {120, N_64}, // 77.78 Hz
	.note_E = {113, N_64}, // 82.41 Hz
	.note_F = {106, N_64}, // 87.31 Hz
	.note_FS = {100, N_64}, // 92.50 Hz
	.note_G = {95, N_64}, // 98.00 Hz
	.note_GS = {89, N_64}, // 103.83 Hz
	.note_A = {84, N_64}, // 110.00 Hz
	.note_AS = {79, N_64}, // 116.54 Hz
	.note_B = {75, N_64} // 123.47 Hz
	},
	{ // octave 3
	.note_C = {71, N_64}, // 130.81 Hz
	.note_CS = {67, N_64}, // 138.59 Hz
	.note_D = {63, N_64}, // 146.83 Hz
	.note_DS = {59, N_64}, // 155.56 Hz
	.note_E = {56, N_64}, // 164.81 Hz
	.note_F = {53, N_64}, // 174.61 Hz
	.note_FS = {50, N_64}, // 185.00 Hz
	.note_G = {47, N_64}, // 196.00 Hz
	.note_GS = {44, N_64}, // 207.65 Hz
	.note_A = {42, N_64}, // 220.00 Hz
	.note_AS = {39, N_64}, // 233.08 Hz
	.note_B = {37, N_64} // 246.94 Hz
	},
	{ // octave 4
	.note_C = {35, N_64}, // 261.63 Hz
	.note_CS = {33, N_64}, // 277.18 Hz
	.note_D = {31, N_64}, // 293.66 Hz
	.note_DS = {29, N_64}, // 311.13 Hz
	.note_E = {27, N_64}, // 329.63 Hz
	.note_F = {26, N_64}, // 349.23 Hz
	.note_FS = {24, N_64}, // 369.99 Hz
	.note_G = {23, N_64}, // 392.00 Hz
	.note_GS = {22, N_64}, // 415.30 Hz
	.note_A = {20, N_64}, // 440.00 Hz
	.note_AS = {19, N_64}, // 466.16 Hz
	.note_B = {18, N_64} // 493.88 Hz
	},
	{  // octave 5
	.note_C = {142, N_8}, // 523.25 Hz
	.note_CS = {134, N_8}, // 554.37 Hz
	.note_D = {127, N_8}, // 587.33 Hz
	.note_DS = {120, N_8}, // 622.25 Hz
	.note_E = {113, N_8}, // 659.25 Hz
	.note_F = {106, N_8}, // 349.23 Hz
	.note_FS = {100, N_8}, // 369.99 Hz
	.note_G = {95, N_8}, // 392.00 Hz
	.note_GS = {89, N_8}, // 415.30 Hz
	.note_A = {84, N_8}, // 440.00 Hz
	.note_AS = {79, N_8}, // 466.16 Hz
	.note_B = {75, N_8} // 493.88 Hz
	},
	{  // octave 6
	.note_C = {71, N_8}, // 1046.50 Hz
	.note_CS = {67, N_8}, // 1108.73 Hz
	.note_D = {63, N_8}, // 1174.66 Hz
	.note_DS = {59, N_8}, // 1244.51 Hz
	.note_E = {56, N_8}, // 1318.51 Hz
	.note_F = {53, N_8}, // 1396.91 Hz
	.note_FS = {50, N_8}, // 1479.98 Hz
	.note_G = {47, N_8}, // 1567.98 Hz
	.note_GS = {44, N_8}, // 1661.22 Hz
	.note_A = {42, N_8}, // 1760.00 Hz
	.note_AS = {39, N_8}, // 1864.66 Hz
	.note_B = {37, N_8} // 1975.53 Hz
	},
	{  // octave 7
	.note_C = {35, N_8}, // 2093.00 Hz
	.note_CS = {33, N_8}, // 2217.46 Hz
	.note_D = {31, N_8}, // 2349.32 Hz
	.note_DS = {29, N_8}, // 2489.02 Hz
	.note_E = {27, N_8}, // 2637.02 Hz
	.note_F = {26, N_8}, // 2793.83 Hz
	.note_FS = {24, N_8}, // 2959.96 Hz
	.note_G = {23, N_8}, // 3135.96 Hz
	.note_GS = {22, N_8}, // 3322.44 Hz
	.note_A = {20, N_8}, // 3520.00 Hz
	.note_AS = {19, N_8}, // 3729.31 Hz
	.note_B = {18, N_8} // 3951.07 Hz
	}
};

// default constructor
Tones::Tones()
{
} //Tones

// default destructor
Tones::~Tones()
{
} //~Tones

void Tones::Init()
{
   PRR0 &= ~(_BV(PRTIM0));
	//DDRD |= _BV(6);
	//PORTD &= ~(_BV(PORTD6));
   // Output low.
   BUZZER_DIR_OUT;
   BUZZER_LOW;

	// Set Timer0, mode CTC, toggle on compare, OCR0A sets limit, prescale 8 but will vary according to the table
	TCCR0A = (1 << COM0A0) | (1 << WGM01);
	TCCR0B= (1 << CS10);;

	TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00)); // stop the timer until there is something to play
   TCNT0 = 0;
   OCR0A = 0xFF;
}

void Tones::Delay(uint32_t d)
{
   while(d > 0)
   {
      _delay_ms(1);
      --d;
   }
}

void Tones::PlayBlock(uint8_t note, uint8_t octave, uint32_t hold)
{
   if(octave < 8 && note < 12)
   {
      note_t val;
      uint16_t address = ((unsigned int)&octaves + sizeof(octave_t) * octave + sizeof(note_t) * note);
      eeprom_read_block(&val, (uint8_t *)address, sizeof(note_t));
      TCCR0A |= (1 << COM0A0); // Connect OC0A
      TCCR0B = (TCCR0B & ~((1<<CS02)|(1<<CS01)|(1<<CS00))) | val.N;
      OCR0A = val.OCRxn - 1; // set the OCRnx
      tonePlaying = true;
      Delay(hold);
      Stop();
   }
}

void Tones::Play(uint8_t note, uint8_t octave, uint32_t hold)
{
   if(octave < 8 && note < 12)
   {
	   noteQueue.enqueue(queuedNote(note, octave, hold));
   }
}

void Tones::PlayNext()
{
   queuedNote x = noteQueue.dequeue();
	note_t val;
	if (x.length > 0)
	{
		uint16_t address = ((unsigned int)&octaves + sizeof(octave_t) * x.octave + sizeof(note_t) * x.note);
      eeprom_read_block(&val, (uint8_t *)address, sizeof(note_t));
      TCCR0A |= (1 << COM0A0); // Connect OC0A
		TCCR0B = (TCCR0B & ~((1<<CS02)|(1<<CS01)|(1<<CS00))) | val.N;
		OCR0A = val.OCRxn - 1; // set the OCRnx
		tonePlaying = true;
		noteEnd = t1.millis() + x.length;
   }
}

void Tones::Update(uint32_t now)
{
   if(tonePlaying)
	{
		if(now > noteEnd)
		{
			if(noteQueue.count() > 0)
			{
				PlayNext();
			}
			else
			{
			   Stop();
			}
		}
	}
	else if(noteQueue.count() > 0)
	{
		PlayNext();
	}
}

void Tones::Stop()
{
   TCCR0A &= ~(1 << COM0A0); // Disconnect OC0A, otherwise the output may hang high.
   TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00)); // stop the timer
   OCR0A = 0xFF;
   TCNT0 = 0;
   // Stay low
   BUZZER_LOW;
	//PORTD &= ~(_BV(PORTD6)); // Make sure the port stays low to drain the mosfet gate.
	tonePlaying = false;
}

void Tones::Flush()
{
   Stop();
   noteQueue.flush();
}

void Tones::Sleep()
{
   Flush();
   // Input (Hi Z), no pullup.
   BUZZER_LOW;
   BUZZER_DIR_IN;

   PRR0 |= _BV(PRTIM0);
}

void Tones::Test()
{
   for(uint8_t n = 0; n < 12; ++n)
   {
      for(uint8_t o = 0; o < 8; ++o)
      {
         PlayBlock(n, o, 50);
      }
   }
}
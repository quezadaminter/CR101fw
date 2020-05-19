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
#include <avr/wdt.h>
#include <avr/sleep.h>
#include "TWI.h"
#include "Timer1.h"
#include "Tones.h"
#include "PCInterrupts.h"
#include "Registers.h"

uint32_t now = 0;

void WDTInit()
{
   // Clear WDT reset flag if it was set
   // is possible previous WDT reset
   // since we can not clear WDE if WDRF is set.
   MCUSR &= ~(1<<WDRF);

   cli();

   wdt_reset();
   // Enable a change on the WD control register.
   // Must be completed within 4 clock cycles so
   // we do it on the same instruction.
   WDTCSR |= (1<<WDCE)|(1<<WDE); // Enable changes to the WDT
   // Enable the system reset and the interrupt mode also and set prescaling
   // for 2 second timeout.
   WDTCSR = (1<<WDE)|(1<< WDIE)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0);

   sei();
}

ISR(WDT_vect)
{
   WDTCSR |= (1<<WDIF); // Clear interrupt flag (should be done by hw)
}

#define BDDR_ON 0B00000000
#define BPOR_ON 0B00111111
#define CDDR_ON 0B00000000
#define CPOR_ON 0B11001101
#define DDDR_ON 0B00100010
#define DPOR_ON 0B10010000

#define BDDR_SL 0B00000000
#define BPOR_SL 0B00000000
#define CDDR_SL 0B00000000
#define CPOR_SL 0B00000000
#define DDDR_SL 0B00000000
#define DPOR_SL 0B00000000

void InitializeIO()
{
   //WDTInit();
   // Initialize the pullups
   // on the switch input ports.
   DDRB  = BDDR_ON;   //0B00000000;
   PORTB = BPOR_ON;   //0B00111111;
   DDRC  = CDDR_ON;   //0B00000000;
   PORTC = CPOR_ON;   //0B11001101;
   DDRD  = DDDR_ON;   //0B00100010;
   PORTD = DPOR_ON;   //0B10010000;

   ints.Init();
   twi.Init();
   t1.Init();
   tones.Init();

   tones.PlayBlock(4, 5, 200);
   tones.PlayBlock(5, 5, 200);
}

void Sleep()
{
   //// Set pins to high impedance inputs (no pullup)
	//// to save power and hold the line
	//// so there are no fake button presses.
   //BYTE &= ~((1 << 7) | (1 << 6))
   DDRB  = BDDR_SL;   //0B00000000;
   PORTB = BPOR_SL;   //0B00111111;
   DDRC  = CDDR_SL;   //0B00000000;
   PORTC = CPOR_SL;   //0B11001101;
   DDRD  = DDDR_SL;   //0B00100010;
   PORTD = DPOR_SL;   //0B10010000;

   ints.Sleep();
   twi.Sleep();
   t1.Sleep();
   tones.Sleep();
   wdt_disable();

	ACSR |= _BV(ACD);                       // Disable comparator
	MCUCR |= _BV(BODS) | _BV(BODSE);          //turn off the brown-out detector

   set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement
	sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
	sleep_mode();                            // sleep
	sleep_disable();                        // Clear SE bit

	InitializeIO();
	sei();
}

int main(void)
{
   // Set the proper state for the pins

   //DDRB = 0x00;
   //PORTB = 0x00;
   //DDRD = 0x00;//|= _BV(1);
   //PORTD = 0x00;

   InitializeIO();
   sei();

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
            //if(s == 0)
            //{
               //s = 1;
               //DDRD |= (_BV(0));
               //PORTD |= (_BV(0));
               ////tones.Play(A(3), 200);
////               twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_GREEN_BIT);
               ////tone_play(F(5), 250);
            //}
            //else if(s == 1)
            //{
               //s = 2;
               //PORTD &= ~(_BV(0));
               ////tones.Play(A(4), 200);
               ////twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_RED_BIT);
            //}
            //else if(s == 2)
            //{
               //s = 0;
               //PORTD &= ~(_BV(0));
               //DDRD &= ~(_BV(0));
               ////tones.Play(A(5), 200);
               ////twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_RG_OFF_BIT);
            //}
         }
      }
      ints.CheckSwitchStates();
      //CheckScrollClicks();
		//CheckChargerStatus();
		//CheckBatteryLevel(now);
		//CheckKeyCombinations(now);
//		CheckToneState(now);
      //CheckSleepTimer(now);
      tones.Update(now);

      // Feed the dog so it don't bite.
      wdt_reset();
    }
}


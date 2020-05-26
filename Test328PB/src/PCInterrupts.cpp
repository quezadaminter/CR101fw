/* 
* PCInterrupts.cpp
*
* Created: 5/9/2020 11:46:24 PM
* Author: MQUEZADA
*/

#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>
#include "PCInterrupts.h"
#include "Tones.h"
#include "TWI.h"
#include "Messages.h"
#include "Registers.h"

PCInterrupts ints;

uint8_t pcint0_history = 0;
uint8_t pcint1_history = 0;
uint8_t pcint2_history = 0;
uint8_t pcint3_history = 0;

volatile bool scrollEventDetected = false;
uint16_t SWITCH_STATES = 0;

// default constructor
PCInterrupts::PCInterrupts()
{
} //PCInterrupts

// default destructor
PCInterrupts::~PCInterrupts()
{
} //~PCInterrupts

void PCInterrupts::Init()
{
// PCIE0 - PORTB
// PCIE1 - PORTC
// PCIE2 - PORTD
// PCIE3 - PORTE
	PCICR = (1 << PCIE0) | // Enable pin change interrupt 0
	        (1 << PCIE1) | // Enable pin change interrupt 1
	        (1 << PCIE2) | // Enable pin change interrupt 2
	        (1 << PCIE3);  // Enable pin change interrupt 3

           // Select the pins that will listen to interrupts
           // PORTE
	PCMSK3 = (1 << PCINT27) | // Pin change enable mask 27
            (1 << PCINT26);  // Pin change enable mask 26

           // PORTD
	PCMSK2 = (1 << PCINT17) | // Pin change enable mask 17
	         (1 << PCINT19) |  // Pin change enable mask 20
	         (1 << PCINT20) |  // Pin change enable mask 20
	         (1 << PCINT23);  // Pin change enable mask 23

            // PORTC
	PCMSK1 = (1 << PCINT8) |  // Pin change enable mask 8
	         (1 << PCINT10) | // Pin change enable mask 10
	         (1 << PCINT11);  // Pin change enable mask 11

            // PORTB
	PCMSK0 = (1 << PCINT0) | // Pin change enable mask 0
	         (1 << PCINT1) | // Pin change enable mask 1
	         (1 << PCINT2) | // Pin change enable mask 2
	         (1 << PCINT3) | // Pin change enable mask 3
	         (1 << PCINT4) | // Pin change enable mask 4
	         (1 << PCINT5);  // Pin change enable mask 5
}

ISR(PCINT0_vect)
{
   // PORTB
	/* Insert your pin change 0 interrupt handling code here */
   // Interrupts 0 to 7
   uint8_t changedbits;

   changedbits = PINB ^ pcint0_history;
   pcint0_history = PINB;
   
   if(changedbits & _BV(INT_PLAY_PAUSE))
   {
      IS_PRESSED(PINB, INT_PLAY_PAUSE) ? (SWITCH_STATES &= ~(_BV(SWITCH_PLAY_PAUSE))) : (SWITCH_STATES |= _BV(SWITCH_PLAY_PAUSE));
   }
   else if(changedbits & _BV(INT_REWIND))
   {
      IS_PRESSED(PINB, INT_REWIND) ? (SWITCH_STATES &= ~(_BV(SWITCH_REWIND))) : (SWITCH_STATES |= _BV(SWITCH_REWIND));
   }
   else if(changedbits & _BV(INT_C))
   {
      IS_PRESSED(PINB, INT_C) ? (SWITCH_STATES &= ~(_BV(SWITCH_C))) : (SWITCH_STATES |= _BV(SWITCH_C));
   }
   else if(changedbits & _BV(INT_B))
   {
      IS_PRESSED(PINB, INT_B) ? (SWITCH_STATES &= ~(_BV(SWITCH_B))) : (SWITCH_STATES |= _BV(SWITCH_B));
   }
   else if(changedbits & _BV(INT_A))
   {
      IS_PRESSED(PINB, INT_A) ? (SWITCH_STATES &= ~(_BV(SWITCH_A))) : (SWITCH_STATES |= _BV(SWITCH_A));
   }
   else if(changedbits & _BV(INT_VOL_DN))
   {
      IS_PRESSED(PINB, INT_VOL_DN) ? (SWITCH_STATES &= ~(_BV(SWITCH_VOL_DN))) : (SWITCH_STATES |= _BV(SWITCH_VOL_DN));
   }
}

ISR(PCINT1_vect)
{
   // PORTC
	/* Insert your pin change 1 interrupt handling code here */
	// Interrupts 8 to 14
   uint8_t changedbits;

   changedbits = PINC ^ pcint1_history;
   pcint1_history = PINC;
   
   if(changedbits & _BV(INT_ENTER))
   {
      IS_PRESSED(PINC, INT_ENTER) ? (SWITCH_STATES &= ~(_BV(SWITCH_ENTER))) : (SWITCH_STATES |= _BV(SWITCH_ENTER));
   }
   else if(changedbits & _BV(INT_ZONE))
   {
      IS_PRESSED(PINC, INT_ZONE) ? (SWITCH_STATES &= ~(_BV(SWITCH_ZONE))) : (SWITCH_STATES |= _BV(SWITCH_ZONE));
   }
   else if(changedbits & _BV(INT_BACK))
   {
      IS_PRESSED(PINC, INT_BACK) ? (SWITCH_STATES &= ~(_BV(SWITCH_BACK))) : (SWITCH_STATES |= _BV(SWITCH_BACK));
   }
}

ISR(PCINT2_vect)
{
   // PORTD
	/* Insert your pin change 2 interrupt handling code here */
	// Interrupts 16 to 23
   uint8_t changedbits;

   changedbits = PIND ^ pcint2_history;
   pcint2_history = PIND;
   
   if(changedbits & _BV(T84_DATA_READY_INT))
   {
      //IS_PRESSED(PIND, T84_DATA_READY_INT) ? (SWITCH_STATES &= ~(_BV(SWITCH_SCROLL_EVENT))) : (SWITCH_STATES |= _BV(SWITCH_SCROLL_EVENT));
      if(IS_RELEASED(PIND, T84_DATA_READY_INT))
      {
         SWITCH_STATES |= _BV(SWITCH_SCROLL_EVENT);
      }
      else
      {
         SWITCH_STATES &= ~(_BV(SWITCH_SCROLL_EVENT));
      }
   }
   else if(changedbits & _BV(INT_MUSIC))
   {
      IS_PRESSED(PIND, INT_MUSIC) ? (SWITCH_STATES &= ~(_BV(SWITCH_MUSIC))) : (SWITCH_STATES |= _BV(SWITCH_MUSIC));
   }
   else if(changedbits & _BV(INT_FORWARD))
   {
      IS_PRESSED(PIND, INT_FORWARD) ? (SWITCH_STATES &= ~(_BV(SWITCH_FORWARD))) : (SWITCH_STATES |= _BV(SWITCH_FORWARD));
   }
}

ISR(PCINT3_vect)
{
   // PORTE
	/* Insert your pin change 3 interrupt handling code here */
	// Interrupts 24 to 27
   uint8_t changedbits;

   changedbits = PINE ^ pcint3_history;
   pcint3_history = PINE;
   
   if(changedbits & _BV(INT_VOL_UP))
   {
      IS_PRESSED(PINE, INT_VOL_UP) ? (SWITCH_STATES &= ~(_BV(SWITCH_VOL_UP))) : (SWITCH_STATES |= _BV(SWITCH_VOL_UP));
   }
   else if(changedbits & _BV(INT_MUTE))
   {
      IS_PRESSED(PINE, INT_MUTE) ? (SWITCH_STATES &= ~(_BV(SWITCH_MUTE))) : (SWITCH_STATES |= _BV(SWITCH_MUTE));
   }
}

void PCInterrupts::CheckSwitchStates()
{
   uint16_t switch_states = 0;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
      switch_states = SWITCH_STATES;
   }
   uint16_t change = SWITCH_PAST_STATE ^ switch_states;

   if(change)
   {
      DDRD |= _BV(5);
      PORTD ^= _BV(5);
      if(twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_WHITE_PWM_VALUE, 255) == ERROR)
      {
         tones.Play(A(3), 50);
         tones.Play(A(2), 50);
         //_delay_ms(1);
      }
      else
      {
         //tones.Play(E(3), 20);
      }

      if(change & _BV(SWITCH_MUTE))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_MUTE);
         //_delay_ms(1);
      }
      if(change & _BV(SWITCH_VOL_UP))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_VOL_UP);
         //_delay_ms(1);
      }
      if(change & _BV(SWITCH_VOL_DN))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_VOL_DN);
         //_delay_ms(1);
      }
      if(change & _BV(SWITCH_A))
      {
         IS_PRESSED(switch_states, SWITCH_A) ?
            twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_GREEN_BIT_MASK) :
            twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, 0);
         SWITCH_PAST_STATE ^= _BV(SWITCH_A);
         //_delay_ms(1);
      }
      if(change & _BV(SWITCH_B))
      {
         IS_RELEASED(switch_states, SWITCH_B) ?
            twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_SLEEP, 0xFF) :
            0;
         SWITCH_PAST_STATE ^= _BV(SWITCH_B);
         //_delay_ms(1);
      }
      if(change & _BV(SWITCH_C))
      {
         IS_PRESSED(switch_states, SWITCH_C) ?
            twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_RED_BIT_MASK) :
            twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, 0);
         SWITCH_PAST_STATE ^= _BV(SWITCH_C);
         //_delay_ms(1);
      }
      if(change & _BV(SWITCH_ZONE))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_ZONE);
         //_delay_ms(1);
      }
      if(change & _BV(SWITCH_BACK))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_BACK);
         //_delay_ms(1);
      }
      if(change & _BV(SWITCH_MUSIC))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_MUSIC);
         //_delay_ms(1);
      }
      if(change & _BV(SWITCH_ENTER))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_ENTER);
         //_delay_ms(1);
      }
      if(change & _BV(SWITCH_REWIND))
      {
         if(IS_RELEASED(switch_states, SWITCH_REWIND))
         {
            // Simulate a scroll -
            twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_I2C_TEST, 110);
            //_delay_ms(1);
         }
         SWITCH_PAST_STATE ^= _BV(SWITCH_REWIND);
      }
      if(change & _BV(SWITCH_PLAY_PAUSE))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_PLAY_PAUSE);
         //_delay_ms(1);
      }
      if(change & _BV(SWITCH_FORWARD))
      {
         if(IS_RELEASED(switch_states, SWITCH_FORWARD))
         {
            // Simulate a scroll +
            twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_I2C_TEST, 150);
            //_delay_ms(1);
         }
         SWITCH_PAST_STATE ^= _BV(SWITCH_FORWARD);
      }
      if(change & _BV(SWITCH_SCROLL_EVENT))
      {
         if(IS_PRESSED(switch_states, SWITCH_SCROLL_EVENT))
         {
            DDRD |= _BV(0);
            PORTD ^= _BV(0);
            // Turn it off, only act on a falling edge
            // of the interrupt.
            //switch_states ^= _BV(SWITCH_SCROLL_EVENT);
            // Call the T84 to get the event.
            uint8_t c(128);
            if(twi.ReceiveByte(T84_I2C_SLAVE_ADDRESS, T84_REG_SCROLL_CLICKS, c) == SUCCESS)
            {
               int8_t d = c - 128;
               if(d > 0)
               {
                  tones.Play(F(4), 20);
               }
               else if(d < 0)
               {
                  tones.Play(C(4), 20);
               }
            }
         }
         SWITCH_PAST_STATE ^= _BV(SWITCH_SCROLL_EVENT);
      }
      //// Send the PI the switch event.
      //I2C_0_write2ByteRegister(PI_I2C_ADDRESS, PI_EVENT_REGISTER, switch_states);
   }
//   SWITCH_PAST_STATE = switch_states;
}

void PCInterrupts::Sleep()
{

}
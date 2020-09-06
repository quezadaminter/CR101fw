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
#include "PinMap.h"
#include "Timer1.h"

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
   // Inputs with pullups.
   DDRB  &= ~(SW_PLAYbm | SW_REWbm | SW_Cbm | SW_Bbm | SW_Abm | SW_VOL_DNbm);
   PORTB |=  (SW_PLAYbm | SW_REWbm | SW_Cbm | SW_Bbm | SW_Abm | SW_VOL_DNbm);

   DDRC  &= ~(SW_ENTERbm | SW_ZONESbm | SW_RETURNbm);
   PORTC |=  (SW_ENTERbm | SW_ZONESbm | SW_RETURNbm);

   DDRD  &= ~(SCROLL_INTbm | SW_FORWARDbm);
   PORTD |=  (SCROLL_INTbm | SW_FORWARDbm);

   DDRE  &= ~(SW_MUSICbm | SW_VOL_UPbm | SW_MUTEbm);
   PORTE |=  (SW_MUSICbm | SW_VOL_UPbm | SW_MUTEbm);

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
	PCMSK3 = (SW_MUSICbm  |  // Pin change enable mask 24
             SW_VOL_UPbm |  // Pin change enable mask 26
             SW_MUTEbm);    // Pin change enable mask 27

           // PORTD
	PCMSK2 = (SCROLL_INTbm | // Pin change enable mask 17
	          SW_FORWARDbm);  // Pin change enable mask 23

            // PORTC
	PCMSK1 = (SW_ENTERbm |  // Pin change enable mask 8
	          SW_ZONESbm |  // Pin change enable mask 10
	          SW_RETURNbm); // Pin change enable mask 11

            // PORTB
	PCMSK0 = (SW_PLAYbm | // Pin change enable mask 0
	          SW_REWbm  | // Pin change enable mask 1
	          SW_Cbm    | // Pin change enable mask 2
	          SW_Bbm    | // Pin change enable mask 3
	          SW_Abm    | // Pin change enable mask 4
	          SW_VOL_DNbm);  // Pin change enable mask 5

   LAST_EVENT_TIME = t1.millis();
}

ISR(PCINT0_vect)
{
   // PORTB
	/* Insert your pin change 0 interrupt handling code here */
   // Interrupts 0 to 7
   uint8_t changedbits = PINB ^ pcint0_history;
   pcint0_history = PINB;
   
   if(changedbits & SW_PLAYbm)
   {
      //IS_PRESSED(PINB, INT_PLAY_PAUSE) ? (SWITCH_STATES &= ~(_BV(SWITCH_PLAY_PAUSE))) : (SWITCH_STATES |= _BV(SWITCH_PLAY_PAUSE));
      SW_PLAY_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_PLAY_PAUSE))) : (SWITCH_STATES |= _BV(SWITCH_PLAY_PAUSE));
   }
   else if(changedbits & SW_REWbm)
   {
      //IS_PRESSED(PINB, INT_REWIND) ? (SWITCH_STATES &= ~(_BV(SWITCH_REWIND))) : (SWITCH_STATES |= _BV(SWITCH_REWIND));
      SW_REW_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_REWIND))) : (SWITCH_STATES |= _BV(SWITCH_REWIND));
   }
   else if(changedbits & SW_Cbm)
   {
      //IS_PRESSED(PINB, INT_C) ? (SWITCH_STATES &= ~(_BV(SWITCH_C))) : (SWITCH_STATES |= _BV(SWITCH_C));
      SW_C_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_C))) : (SWITCH_STATES |= _BV(SWITCH_C));
   }
   else if(changedbits & SW_Bbm)
   {
      //IS_PRESSED(PINB, INT_B) ? (SWITCH_STATES &= ~(_BV(SWITCH_B))) : (SWITCH_STATES |= _BV(SWITCH_B));
      SW_B_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_B))) : (SWITCH_STATES |= _BV(SWITCH_B));
   }
   else if(changedbits & SW_Abm)
   {
      //IS_PRESSED(PINB, INT_A) ? (SWITCH_STATES &= ~(_BV(SWITCH_A))) : (SWITCH_STATES |= _BV(SWITCH_A));
      SW_A_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_A))) : (SWITCH_STATES |= _BV(SWITCH_A));
   }
   else if(changedbits & SW_VOL_DNbm)
   {
      //IS_PRESSED(PINB, INT_VOL_DN) ? (SWITCH_STATES &= ~(_BV(SWITCH_VOL_DN))) : (SWITCH_STATES |= _BV(SWITCH_VOL_DN));
      SW_VOL_DN_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_VOL_DN))) : (SWITCH_STATES |= _BV(SWITCH_VOL_DN));
   }
}

ISR(PCINT1_vect)
{
   // PORTC
	/* Insert your pin change 1 interrupt handling code here */
	// Interrupts 8 to 14
   uint8_t changedbits = PINC ^ pcint1_history;
   pcint1_history = PINC;
   
   if(changedbits & SW_ENTERbm)
   {
      //IS_PRESSED(PINC, INT_ENTER) ? (SWITCH_STATES &= ~(_BV(SWITCH_ENTER))) : (SWITCH_STATES |= _BV(SWITCH_ENTER));
      SW_ENTER_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_ENTER))) : (SWITCH_STATES |= _BV(SWITCH_ENTER));
   }
   else if(changedbits & SW_ZONESbm)
   {
      //IS_PRESSED(PINC, INT_ZONE) ? (SWITCH_STATES &= ~(_BV(SWITCH_ZONE))) : (SWITCH_STATES |= _BV(SWITCH_ZONE));
      SW_ZONES_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_ZONE))) : (SWITCH_STATES |= _BV(SWITCH_ZONE));
   }
   else if(changedbits & SW_RETURNbm)
   {
      //IS_PRESSED(PINC, INT_BACK) ? (SWITCH_STATES &= ~(_BV(SWITCH_BACK))) : (SWITCH_STATES |= _BV(SWITCH_BACK));
      SW_RETURN_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_BACK))) : (SWITCH_STATES |= _BV(SWITCH_BACK));
   }
}

ISR(PCINT2_vect)
{
   // PORTD
	/* Insert your pin change 2 interrupt handling code here */
	// Interrupts 16 to 23
   uint8_t changedbits = PIND ^ pcint2_history;
   pcint2_history = PIND;
   
   if(changedbits & SCROLL_INTbm)
   {
      //IS_PRESSED(PIND, T84_DATA_READY_INT) ? (SWITCH_STATES &= ~(_BV(SWITCH_SCROLL_EVENT))) : (SWITCH_STATES |= _BV(SWITCH_SCROLL_EVENT));
      //if(IS_RELEASED(PIND, T84_DATA_READY_INT))
      //{
         //SWITCH_STATES |= _BV(SWITCH_SCROLL_EVENT);
      //}
      //else
      //{
         //SWITCH_STATES &= ~(_BV(SWITCH_SCROLL_EVENT));
      //}
      if(SCROLL_INT_IS_HIGH)
      {
         SWITCH_STATES |= _BV(SWITCH_SCROLL_EVENT);
      }
      else
      {
         SWITCH_STATES &= ~(_BV(SWITCH_SCROLL_EVENT));
      }
   }
   else if(changedbits & SW_FORWARDbm)
   {
      //IS_PRESSED(PIND, INT_FORWARD) ? (SWITCH_STATES &= ~(_BV(SWITCH_FORWARD))) : (SWITCH_STATES |= _BV(SWITCH_FORWARD));
      SW_FORWARD_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_FORWARD))) : (SWITCH_STATES |= _BV(SWITCH_FORWARD));
   }
}

ISR(PCINT3_vect)
{
   // PORTE
	/* Insert your pin change 3 interrupt handling code here */
	// Interrupts 24 to 27
   uint8_t changedbits = PINE ^ pcint3_history;
   pcint3_history = PINE;
   
   if(changedbits & SW_MUSICbm)
   {
      //IS_PRESSED(PINE, INT_VOL_UP) ? (SWITCH_STATES &= ~(_BV(SWITCH_VOL_UP))) : (SWITCH_STATES |= _BV(SWITCH_VOL_UP));
      SW_MUSIC_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_MUSIC))) : (SWITCH_STATES |= _BV(SWITCH_MUSIC));
   }
   else if(changedbits & SW_VOL_UPbm)
   {
      //IS_PRESSED(PINE, INT_VOL_UP) ? (SWITCH_STATES &= ~(_BV(SWITCH_VOL_UP))) : (SWITCH_STATES |= _BV(SWITCH_VOL_UP));
      SW_VOL_UP_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_VOL_UP))) : (SWITCH_STATES |= _BV(SWITCH_VOL_UP));
   }
   else if(changedbits & SW_MUTEbm)
   {
      //IS_PRESSED(PINE, INT_MUTE) ? (SWITCH_STATES &= ~(_BV(SWITCH_MUTE))) : (SWITCH_STATES |= _BV(SWITCH_MUTE));
      SW_MUTE_IS_HIGH ? (SWITCH_STATES &= ~(_BV(SWITCH_MUTE))) : (SWITCH_STATES |= _BV(SWITCH_MUTE));
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
      LAST_EVENT_TIME = t1.millis();

      if(twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_WHITE_PWM_VALUE, 255) == ERROR)
      {
         tones.Play(A(3), 50);
         tones.Play(A(2), 50);
      }
      else
      {
         tones.Play(E(3), 200);
      }

      // Forward the switch states to the PI
      if(twi.SendWord(PI_I2C_ADDRESS, PI_INPUT_REGISTER_H, switch_states) == ERROR)
      {
         tones.Play(G(1), 100);
      }

      if(change & _BV(SWITCH_MUTE))
      {
         //if(IS_PRESSED(switch_states, SWITCH_A))
         //{
            //T84_LED_STATES |= T84_GREEN_BIT_MASK;
            //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
         //}
         //else
         //{
            //T84_LED_STATES &= ~T84_GREEN_BIT_MASK;
            //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
         //}
         SWITCH_PAST_STATE ^= _BV(SWITCH_MUTE);
      }
      if(change & _BV(SWITCH_VOL_UP))
      {
         //if(IS_PRESSED(switch_states, SWITCH_A))
         //{
            //T84_LED_STATES |= T84_ORANGE_BIT_MASK;
            //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
         //}
         //else
         //{
            //T84_LED_STATES &= ~T84_ORANGE_BIT_MASK;
            //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
         //}
         SWITCH_PAST_STATE ^= _BV(SWITCH_VOL_UP);
      }
      if(change & _BV(SWITCH_VOL_DN))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_VOL_DN);
      }
      if(change & _BV(SWITCH_A))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_A);
      }
      if(change & _BV(SWITCH_B))
      {
         //IS_RELEASED(switch_states, SWITCH_B) ?
            //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_SLEEP, 0xFF) :
            //0;
         SWITCH_PAST_STATE ^= _BV(SWITCH_B);
      }
      if(change & _BV(SWITCH_C))
      {
         //if(IS_PRESSED(switch_states, SWITCH_C))
         //{
            //T84_LED_STATES |= T84_RED_BIT_MASK;
            //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
         //}
         //else
         //{
            //T84_LED_STATES &= ~T84_RED_BIT_MASK;
            //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_LED_STATE, T84_LED_STATES);
         //}
         SWITCH_PAST_STATE ^= _BV(SWITCH_C);
      }
      if(change & _BV(SWITCH_ZONE))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_ZONE);
      }
      if(change & _BV(SWITCH_BACK))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_BACK);
      }
      if(change & _BV(SWITCH_MUSIC))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_MUSIC);
      }
      if(change & _BV(SWITCH_ENTER))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_ENTER);
      }
      if(change & _BV(SWITCH_REWIND))
      {
         //if(IS_RELEASED(switch_states, SWITCH_REWIND))
         //{
            //// Simulate a scroll -
            //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_I2C_TEST, 110);
         //}
         SWITCH_PAST_STATE ^= _BV(SWITCH_REWIND);
      }
      if(change & _BV(SWITCH_PLAY_PAUSE))
      {
         SWITCH_PAST_STATE ^= _BV(SWITCH_PLAY_PAUSE);
      }
      if(change & _BV(SWITCH_FORWARD))
      {
         //if(IS_RELEASED(switch_states, SWITCH_FORWARD))
         //{
            //// Simulate a scroll +
            //twi.SendByte(T84_I2C_SLAVE_ADDRESS, T84_REG_I2C_TEST, 150);
         //}
         SWITCH_PAST_STATE ^= _BV(SWITCH_FORWARD);
      }
      if(change & _BV(SWITCH_SCROLL_EVENT))
      {
         if(IS_PRESSED(switch_states, SWITCH_SCROLL_EVENT))
         {
            // Turn it off, only act on a falling edge
            // of the interrupt.
            //switch_states ^= _BV(SWITCH_SCROLL_EVENT);
            // Call the T84 to get the event.
            uint8_t c(128);
            if(twi.ReceiveByte(T84_I2C_SLAVE_ADDRESS, T84_REG_SCROLL_CLICKS, c) == SUCCESS)
            {
               int8_t d = c - 128;
               // Forward click count to PI
               twi.SendByte(PI_I2C_ADDRESS, PI_SCROLL_CLICKS_REGISTER, d);
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
   }
}

void PCInterrupts::Sleep()
{
   // Turn off pullups, leave interrupts running.
   DDRB  &= ~(SW_PLAYbm | SW_REWbm | SW_Cbm | SW_Bbm | SW_Abm | SW_VOL_DNbm);
   PORTB &= ~(SW_PLAYbm | SW_REWbm | SW_Cbm | SW_Bbm | SW_Abm | SW_VOL_DNbm);

   DDRC  &= ~(SW_ENTERbm | SW_ZONESbm | SW_RETURNbm);
   PORTC &= ~(SW_ENTERbm | SW_ZONESbm | SW_RETURNbm);

   DDRD  &= ~(SCROLL_INTbm | SW_FORWARDbm);
   PORTD &= ~(SCROLL_INTbm | SW_FORWARDbm);

   DDRE  &= ~(SW_MUSICbm | SW_VOL_UPbm | SW_MUTEbm);
   PORTE &= ~(SW_MUSICbm | SW_VOL_UPbm | SW_MUTEbm);
}
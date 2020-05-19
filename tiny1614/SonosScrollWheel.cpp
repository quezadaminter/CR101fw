/* 
* SonosScrollWheel.cpp
*
* Created: 5/18/2020 11:30:38 AM
* Author: MQUEZADA
*/

#include <util/delay.h>
#include <stdlib.h>
#include "SonosScrollWheel.h"
#include "ADC.h"

#define DB 50
#define AIN 6

#define SC1b 3
#define SC1PORT PORTA
#define SC1bm (1 << SC1b)
#define SC1_DIR_OUT (SC1PORT.DIRSET = SC1bm)
#define SC1_DIR_IN (SC1PORT.DIRCLR = SC1bm)
#define SC1_HIGH (SC1PORT.OUTSET = SC1bm)
#define SC1_LOW (SC1PORT.OUTCLR = SC1bm)
#define SC1_IS_HIGH (SC1PORT.OUT & SC1bm)
#define SC1_IS_LOW !(SC1PORT.OUT & SC1bm)

#define SC2b 2
#define SC2PORT PORTA
#define SC2bm (1 << SC2b)
#define SC2_DIR_OUT (SC2PORT.DIRSET = SC2bm)
#define SC2_DIR_IN (SC2PORT.DIRCLR = SC2bm)
#define SC2_HIGH (SC2PORT.OUTSET = SC2bm)
#define SC2_LOW (SC2PORT.OUTCLR = SC2bm)
#define SC2_IS_HIGH (SC2PORT.OUT & SC2bm)
#define SC2_IS_LOW !(SC2PORT.OUT & SC2bm)

#define SC3b 1
#define SC3PORT PORTA
#define SC3bm (1 << SC3b)
#define SC3_DIR_OUT (SC3PORT.DIRSET = SC3bm)
#define SC3_DIR_IN (SC3PORT.DIRCLR = SC3bm)
#define SC3_HIGH (SC3PORT.OUTSET = SC3bm)
#define SC3_LOW (SC3PORT.OUTCLR = SC3bm)
#define SC3_IS_HIGH (SC3PORT.OUT & SC3bm)
#define SC3_IS_LOW !(SC3PORT.OUT & SC3bm)

SonosScrollWheel SW;

// default constructor
SonosScrollWheel::SonosScrollWheel()
{
} //SonosScrollWheel

// default destructor
SonosScrollWheel::~SonosScrollWheel()
{
} //~SonosScrollWheel

void SonosScrollWheel::begin()
{
   // Initialize ADC
   adc.begin(AIN);
   // Initialize channels.
   SC1_DIR_OUT; SC1_LOW;
   SC2_DIR_OUT; SC2_LOW;
   SC3_DIR_OUT; SC3_LOW;
}

void SonosScrollWheel::setOnTouch(void (*handler)(void))
{
   onTouch = handler;
}

void SonosScrollWheel::setOnRelease(void (*handler)(void))
{
   onRelease = handler;
}

void SonosScrollWheel::setOnMotion(void (*handler)(int8_t))
{
   onMotion = handler;
}

bool SonosScrollWheel::isTouched() const
{
   return(false);
}

int8_t SonosScrollWheel::clicksMoved() const
{
   return(0);
}

uint16_t SonosScrollWheel::ReadTheOne(PORT_t &port, uint8_t bit)
{
   //PORTA |= _BV(PORTA0 + ch);
   //PORTA.OUTSET = _BV(ch);
   port.OUTSET = _BV(bit);
   uint16_t d = adc.Read();
   port.OUTCLR = _BV(bit);
   //PORTA.OUTCLR = _BV(ch);
   //PORTA &= ~(_BV(PORTA0 + ch));
   //Serial.print(d);
   _delay_ms(10);
   return(d);
}

void SonosScrollWheel::SignalUpdate()
{
   if(onMotion != nullptr)
   {
      onMotion(steps);
   }
}

int8_t SonosScrollWheel::Check()
{
   a = ReadTheOne(SC1PORT, SC1b);// 1);
   b = ReadTheOne(SC2PORT, SC2b);// 2);
   c = ReadTheOne(SC3PORT, SC3b);// 3);
   
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
      steps = 0;
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
         //Right
         steps += 1;
         SignalUpdate();
      }
      else if((ca == 1  && cb == 0  && cc == -1) ||
         (ca == -1 && cb == 1  && cc == 0) ||
         (ca == 0  && cb == -1 && cc == 1))
      {
         // Left
         steps -= 1;
         SignalUpdate();
      }
   }
   ca = cb = cc = 0;

   return(steps);
}


void SonosScrollWheel::Sleep()
{
   SC1_LOW; SC1_DIR_IN;
   SC2_LOW; SC2_DIR_IN;
   SC3_LOW; SC3_DIR_IN;
   adc.Sleep();
}
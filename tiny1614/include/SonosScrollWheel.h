/* 
* SonosScrollWheel.h
*
* Created: 5/18/2020 11:30:38 AM
* Author: MQUEZADA
*/

#include <stdint.h>
#include <avr/io.h>

#ifndef __SONOSSCROLLWHEEL_H__
#define __SONOSSCROLLWHEEL_H__


class SonosScrollWheel
{
   //functions
   public:
      SonosScrollWheel();
      ~SonosScrollWheel();

      void setOnTouch(void(*handler)(void));
      void setOnRelease(void(*handler)(void));
      void setOnMotion(void (*handler)(int8_t));

      void begin();//uint8_t adcChannel, PORT_t *cA, PORT_t *cB, PORT_t *cC);
      int8_t Check();
      bool isTouched() const;
      int8_t clicksMoved() const;

      void Sleep();
   protected:
   private:
      SonosScrollWheel( const SonosScrollWheel &c );
      SonosScrollWheel& operator=( const SonosScrollWheel &c );

      uint16_t a,b,c,pa,pb,pc,avg;
      int16_t da, db, dc;
      int8_t ca, cb, cc, steps = 0;

      PORT_t *pinA, *pinB, *pinC;

      uint16_t ReadTheOne(PORT_t &port, uint8_t bit);
      void SignalUpdate();

      void (*onTouch)(void) = nullptr;
      void (*onRelease)(void) = nullptr;
      void (*onMotion)(int8_t) = nullptr;

}; //SonosScrollWheel

extern SonosScrollWheel SW;

#endif //__SONOSSCROLLWHEEL_H__

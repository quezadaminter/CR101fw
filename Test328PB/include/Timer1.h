/* 
* Timer1.h
*
* Created: 5/9/2020 11:02:45 PM
* Author: MQUEZADA
*/

#include <stdint.h>

#ifndef __TIMER1_H__
#define __TIMER1_H__


class Timer1
{
   //variables
   public:
   protected:
   private:

   //functions
   public:
      Timer1();
      ~Timer1();
      void Init();
      uint32_t millis();
      void Sleep();

   protected:
   private:
      Timer1( const Timer1 &c );
      Timer1& operator=( const Timer1 &c );

}; //Timer1

extern Timer1 t1;

#endif //__TIMER1_H__

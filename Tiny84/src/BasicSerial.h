/*
* BasicSerial.h
*
* Created: 5/7/2020 9:08:29 AM
* Author: MQUEZADA
*/


#ifndef __BASICSERIAL_H__
#define __BASICSERIAL_H__

#include <stdint.h>

class BasicSerial
{
   public:
      BasicSerial();
      ~BasicSerial();

      void print(const char *str);
      void println(const char *str);
      void print(uint8_t i);
      void print(int16_t i);
      void print(uint16_t i);
      void print(uint32_t i);
   protected:
   private:
      BasicSerial( const BasicSerial &c );
      BasicSerial& operator=( const BasicSerial &c );

}; //BasicSerial

extern BasicSerial Serial;

#endif //__BASICSERIAL_H__

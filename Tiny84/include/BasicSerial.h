/*
* BasicSerial.h
*
* Created: 5/7/2020 9:08:29 AM
* Author: MQUEZADA
*/


#ifndef __BASICSERIAL_H__
#define __BASICSERIAL_H__

#include <stdint.h>

#define TX_BUFFER_SIZE 64
#define TX_BUFFER_MASK (TX_BUFFER_SIZE - 1)

class BasicSerial
{
   public:
      BasicSerial();
      ~BasicSerial();

      void print(const char *str);
      void println(const char *str);
      void println(uint8_t i);
      void print(uint8_t i);
      void print(int16_t i);
      void print(uint16_t i);
      void print(uint32_t i);
      void send();
   protected:
   private:
      BasicSerial( const BasicSerial &c );
      BasicSerial& operator=( const BasicSerial &c );
      uint8_t TX_BUFFER[TX_BUFFER_SIZE];
      uint8_t txHead = 0;
      uint8_t txTail = 0;
      uint8_t txCount = 0;
      void write(uint8_t i);
      void write(const char *c);

}; //BasicSerial

extern BasicSerial Serial;

#endif //__BASICSERIAL_H__

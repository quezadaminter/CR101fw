/* 
* SerialIO.h
*
* Created: 5/16/2020 7:03:41 PM
* Author: MQUEZADA
*/


#ifndef __SERIAL_H__
#define __SERIAL_H__


#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_TX_BUFFER_MASK (SERIAL_TX_BUFFER_SIZE - 1)

class SerialIO
{
   public:
      SerialIO();
      ~SerialIO();

      void begin(uint32_t speed);
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
      SerialIO( const SerialIO &c );
      SerialIO& operator=( const SerialIO &c );

      //uint8_t TX_BUFFER[SERIAL_TX_BUFFER_SIZE];
      //uint8_t txHead = 0;
      //uint8_t txTail = 0;
      //uint8_t txCount = 0;
      void write(uint8_t i);
      void write(const char *c);
      void push(uint8_t i);

}; //SerialIO

extern SerialIO Serial;
#endif //__SERIAL_H__

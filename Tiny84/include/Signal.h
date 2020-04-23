/*
 * Signal.h
 *
 * Created: 8/10/2018 17:58:39
 *  Author: MQUEZADA
 */ 


#ifndef SIGNAL_H_
#define SIGNAL_H_
#include <stdlib.h>
#include "Pin.h"

/*
#define HIGH 1
#define LOW 0

class Pin
{
   public:
      Pin(int n) : id(n) {}
      void Set(int hl)
      {
         std::cout << id << ", " << hl << std::endl;
      }
      int id = 0;
};
*/

class Signal
{
   public:
      Signal(Pin *p, uint8_t a = 0, uint8_t b = 0, uint8_t c = 0, uint8_t d = 0, uint8_t e = 0, uint8_t f = 0, uint8_t g = 0, uint8_t h = 0, uint8_t i = 0, uint8_t j = 0);
      uint8_t index = 0;
      uint8_t end = 0;
      uint32_t targetTime = 0;
      Pin *pin = NULL;
      uint16_t pattern[11] = { 0 };

      void high();
      void low();
      void start(uint32_t ms);
      Signal *next(uint32_t ms);
};



#endif /* SIGNAL_H_ */
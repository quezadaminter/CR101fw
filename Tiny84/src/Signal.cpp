/*
 * Signal.cpp
 *
 * Created: 8/10/2018 17:58:52
 *  Author: MQUEZADA
 */ 
#include "Signal.h"

Signal::Signal(Pin *p, uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h, uint8_t i, uint8_t j) :
   pin(p)
{
   pattern[0] = a, pattern[1] = b, pattern[2] = c, pattern[3] = d, pattern[4] = e, pattern[5] = f, pattern[6] = g, pattern[7] = h, pattern[8] = i, pattern[9] = j;
   for(uint8_t i = 0; i < 11; ++i)
   {
      if(pattern[i] == 0)
      {
         end = i;
         // Pad the end of the event with some time
         // to avoid engaging the next queued signal
         // immediately after this signal ends.
         pattern[end] = 400;
         break;
      }
   }
}

void Signal::high()
{
   pin->Set(HIGH);
}

void Signal::low()
{
   pin->Set(LOW);
}

void Signal::start(uint32_t ms)
{
   index = 0;
   targetTime = ms + pattern[index];
   high();
}

Signal *Signal::next(uint32_t ms)
{
   index++;
   if(index < 10 && index < end)
   {
      targetTime = ms + pattern[index];
      index % 2 == 0 ? high() : low();
      return(this);
   }
   else if(index == end)
   {
      if((index - 1) > 0 && index % 2 != 0)
      {
         if(pattern[index - 1] < pattern[index])
         {
            targetTime = ms + (pattern[index] - pattern[index - 1]);
         }
      }
      else
      {
         targetTime = ms + pattern[index];
      }
      low();
      return(this);
   }
   //std::cout << this->pin->id << ", END" << std::endl;
   targetTime = 0;
   return(NULL);
}         
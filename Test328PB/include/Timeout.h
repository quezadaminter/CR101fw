/* 
* Timeout.h
*
* Created: 5/28/2020 10:50:41 AM
* Author: MQUEZADA
*/

#include <stdint.h>

#ifndef __TIMEOUT_H__
#define __TIMEOUT_H__

class Timeout
{
   public:
      Timeout(uint32_t tk, uint16_t sp);
      Timeout(uint32_t tk, uint16_t sp, void (*cb)(uint32_t));
      Timeout(uint32_t tk, uint16_t sp, void (*cb)(void));

      bool isActive() const;
      void Active(bool a = true);
      /*
       * Runs at a specific time and
       * then moves the next run point
       * by "step".
       */
      bool RunAt(uint32_t now);
      /*
       * Runs every time "now" is an
       * even multiple of "step".
       */
      bool RunOn(uint32_t now);

   private:
      uint32_t tick = 0;
      uint16_t step = -1;
      bool tocked = false;
      bool active = false;
      void (*onExecuteTime)(uint32_t n) = nullptr;
      void (*onExecute)(void) = nullptr;
};

#endif //__TIMEOUT_H__

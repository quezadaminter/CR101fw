/* 
* Timeout.cpp
*
* Created: 5/28/2020 10:50:41 AM
* Author: MQUEZADA
*/


#include "Timeout.h"

Timeout::Timeout(uint32_t tk, uint16_t sp) :
   tick(tk), step(sp)
{
}

Timeout::Timeout(uint32_t tk, uint16_t sp, void (*cb)(uint32_t) = nullptr) :
   tick(tk), step(sp), onExecuteTime(cb)
{
}

Timeout::Timeout(uint32_t tk, uint16_t sp, void (*cb)(void) = nullptr) :
   tick(tk), step(sp), onExecute(cb)
{
}

bool Timeout::RunAt(uint32_t now)
{
   if(active == true)
   {
      if(now >= tick)
      {
         tick = now + step;
         if(onExecute != nullptr)
         {
            onExecute();
         }
         else if(onExecuteTime != nullptr)
         {
            onExecuteTime(now);
         }
         return(true);
      }
   }
   return(false);
}

bool Timeout::RunOn(uint32_t now)
{
   if(active == true)
   {
      if(now % step == 0)
      {
         if(tocked == false)
         {
            // Execute
            tocked = true;
            if(onExecute != nullptr)
            {
               onExecute();
            }
            else if(onExecuteTime != nullptr)
            {
               onExecuteTime(now);
            }
         }
      }
      else
      {
         tocked = false;
      }
      return(tocked);
   }
   return(false);
}

bool Timeout::isActive() const
{
   return(active);
}

void Timeout::Active(bool a)
{
   active = a;
}
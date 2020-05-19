/* 
* PCInterrupts.h
*
* Created: 5/9/2020 11:46:25 PM
* Author: MQUEZADA
*/


#ifndef __PCINTERRUPTS_H__
#define __PCINTERRUPTS_H__

#define IS_PRESSED(reg, button) (reg & _BV(button))
#define IS_RELEASED(reg, button)  (!(reg & _BV(button)))

class PCInterrupts
{
   //functions
   public:
      PCInterrupts();
      ~PCInterrupts();

      void Init();
      void CheckSwitchStates();
      void Sleep();

   protected:
   private:
      uint16_t SWITCH_PAST_STATE = 0;

      PCInterrupts( const PCInterrupts &c );
      PCInterrupts& operator=( const PCInterrupts &c );

}; //PCInterrupts

extern PCInterrupts ints;

#endif //__PCINTERRUPTS_H__

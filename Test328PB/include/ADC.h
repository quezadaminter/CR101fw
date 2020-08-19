/* 
* ADC.h
*
* Created: 5/27/2020 2:58:01 PM
* Author: MQUEZADA
*/

#include <stdint.h>

#ifndef __aDC_H__
#define __aDC_H__

class aDC
{
   public:
      aDC();
      ~aDC();
      void Init();
      void Sleep();
      uint16_t Read();
   protected:
   private:
      aDC( const aDC &c );
      aDC& operator=( const aDC &c );

}; //aDC

extern aDC adc;

#endif //__ADC_H__

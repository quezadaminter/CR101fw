/* 
* Tones.h
*
* Created: 5/9/2020 11:14:14 PM
* Author: MQUEZADA
*/


#define SimpleFIFO_NONVOLATILE
#include "SimpleFIFO.h"

#ifndef __TONES_H__
#define __TONES_H__

#define C(o)  0,o
#define CS(o) 1,o
#define D(o)  2,o
#define DS(o) 3,o
#define E(o)  4,o
#define F(o)  5,o
#define FS(o) 6,o
#define G(o)  7,o
#define GS(o) 8,o
#define A(o)  9,o
#define AS(o) 10,o
#define B(o)  11,o

class Tones
{
   public:
      Tones();
      ~Tones();

      void Init();
      void PlayBlock(uint8_t note, uint8_t octave, uint32_t hold);
      void Play(uint8_t note, uint8_t octave, uint32_t hold);
      void Update(uint32_t now);
      void Stop();
      void Flush();
      void Sleep();
   protected:
   private:

      class queuedNote
      {
         public:
            queuedNote(){}
            queuedNote(uint8_t n, uint8_t o, uint32_t l) :
               note(n), octave(o), length(l)
            {
            }
            uint8_t note = 0;
            uint8_t octave = 0;
            uint32_t length = 0;
      };

      bool tonePlaying = false;
      uint32_t noteEnd = 0;
      SimpleFIFO<queuedNote, 10> noteQueue;

      Tones( const Tones &c );
      Tones& operator=( const Tones &c );
      void Delay(uint32_t d);
      void PlayNext();

}; //Tones

extern Tones tones;

#endif //__TONES_H__

/*
 * Pin.h
 *
 * Created: 2/20/2016 11:49:05 AM
 *  Author: MQUEZADA
 */ 


#ifndef PIN_H_
#define PIN_H_

#include <avr/sfr_defs.h>

#ifndef HIGH
#define HIGH 0xFF
#endif

#ifndef LOW
#define LOW 0x00
#endif

class Pin
{
   public:
      Pin(volatile uint8_t *ddr, volatile uint8_t *port, volatile uint8_t *in, uint8_t pin) :
         DDR(ddr), Port(port), Pinp(in), Nip(pin)
      {}
      
      void SetOutput() const
      {
         *DDR |= _BV(Nip);
      }
      void SetInput() const
      {
         *DDR &= ~(_BV(Nip));
      }
      void Set(uint8_t hl)
      {
         (hl == HIGH) ? *Port |= _BV(Nip) : *Port &= ~(_BV(Nip));
      }
	  void Toggle()
	  {
		  *Port ^= _BV(Nip);
	  }
      void Pullup(uint8_t hl)
      {
         Set(hl);
      }
      uint8_t Get()
      {
         return(bit_is_set(*Pinp, Nip) ? HIGH : LOW);
      }
      uint8_t GetOutput()
      {
         return(bit_is_set(*Port, Nip));
      }
      volatile uint8_t *DDR;
      volatile uint8_t *Port;
      volatile uint8_t *Pinp;
      uint8_t Nip;
};

#endif /* PIN_H_ */
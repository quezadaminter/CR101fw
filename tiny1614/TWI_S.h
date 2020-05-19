/* 
* TWI_S.h
*
* Created: 5/16/2020 4:02:33 PM
* Author: MQUEZADA
*/


#ifndef __TWI_S_H__
#define __TWI_S_H__

#include "i2c_slave.h"

#ifndef TWI_S_RX_BUFFER_SIZE
#define TWI_S_RX_BUFFER_SIZE 16
#endif

#define TWI_S_RX_BUFFER_MASK (TWI_S_RX_BUFFER_SIZE - 1)

#if ( TWI_RX_BUFFER_SIZE & TWI_RX_BUFFER_MASK )
#  error TWI RX buffer size is not a power of 2
#endif

typedef void(* I2C_callback_byte)(uint8_t);

class TWI_S
{
   //functions
   public:
      TWI_S();
      void begin(uint8_t address);
      ~TWI_S();
    
       void setOnReceive(void (*handler)(uint8_t));
       void setOnRequest(void (*handler)(void));

       uint8_t available();
       uint8_t receive();
       void send(uint8_t data);
   protected:
   private:
      TWI_S( const TWI_S &c );
      TWI_S& operator=( const TWI_S &c );

      static void onMasterWrite(void);
      static void onMasterRead(void);
      static void onAddress(void);
      static void onStop(void);
      static void onCollision(void);
      static void onBusError(void);

      static void (*onReceive)(uint8_t);
      static void (*onRequest)(void);

//      static uint8_t rxBuffer[TWI_S_RX_BUFFER_SIZE];
//      static uint8_t rxCount;
      //uint8_t rxHead = 0;
//      static uint8_t rxTail;

}; //TWI_S

extern TWI_S WireS;

#endif //__TWI_S_H__

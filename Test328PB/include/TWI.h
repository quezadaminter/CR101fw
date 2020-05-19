/* 
* TWI.h
*
* Created: 5/9/2020 8:46:32 PM
* Author: MQUEZADA
*/

#include <stdint.h>

#ifndef __TWI_H__
#define __TWI_H__

// Successful start condition code
#define START_TRANSMITTED 0x08
#define REPEATED_START_TRANSMITTED 0x10

// Status codes in master transmitter mode
#define SLA_W_SENT_ACK_RECVD 0x18
#define SLA_W_SENT_NOT_ACK_RECVD 0x20
#define DATA_BYTE_SENT_ACK_RECVD 0x28
#define DATA_BYTE_SENT_NOT_ACK_RECVD 0x30
#define ARBITRATION_LOST_SLA_W_OR_DATA 0x38

// Status codes in master receiver mode
#define ARBITRATION_LOST_SLA_R_OR_NACK 0x38
#define SLA_R_SENT_ACK_RECVD 0x40
#define SLA_R_SENT_NOT_ACK_RECVD 0x48
#define DATA_BYTE_RECVD_ACK_RECVD 0x50
#define DATA_BYTE_RECVD_NOT_ACK_RECVD 0x58

#define ERROR 1
#define SUCCESS (!ERROR)

class TWI
{
   //variables
   public:
   protected:
   private:

   //functions
   public:
      TWI();
      ~TWI();

      void Init();
      uint8_t SendByte(uint8_t address, uint16_t reg, uint8_t data);
      uint8_t SendWord(uint8_t address, uint16_t reg, uint16_t data);
      uint8_t ReceiveByte(uint8_t device, uint16_t reg, uint8_t &recvdData);
      uint8_t ReceiveWord(uint8_t device, uint16_t reg, uint16_t &rcvdData);
      void Sleep();

   protected:
   private:
      TWI( const TWI &c );
      TWI& operator=( const TWI &c );

      uint8_t Start();
      uint8_t SendAddress(uint8_t address, uint8_t rw);
      uint8_t Write(uint8_t data);
      uint8_t ReadAck();
      uint8_t ReadNack();
      uint8_t Status();
      void Stop();

}; //TWI

extern TWI twi;

#endif //__TWI_H__

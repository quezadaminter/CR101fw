/* 
* TWI.cpp
*
* Created: 5/9/2020 8:46:32 PM
* Author: MQUEZADA
*/

#include <avr/io.h>
#include <util/delay.h>
#include "TWI.h"

TWI twi;
#define WRITE 0x00
#define READ 0x01

// default constructor
TWI::TWI()
{
} //TWI

// default destructor
TWI::~TWI()
{
} //~TWI

void TWI::Init()
{
	/* Enable TWI0 */
	PRR0 &= ~(1 << PRTWI0);

   TWCR0 = 0; // This allows the interface to reinitialize.

	/* SCL bitrate = F_CPU / (16 + 2 * TWBR0 * TWPS value) */
	/* Configured bit rate is 100.000kHz, based on CPU frequency 8.000MHz */
	TWBR0 = 0x20;          /* SCL bit rate: 100.000kHZ before prescaling */
	TWSR0 = 0x00 << TWPS0; /* SCL precaler: 1, effective bitrate = 100.000kHz */
   //enable TWI
   TWCR0 = (1 << TWEN); // Enable interrupt here eventually
}

void TWI::Sleep()
{

}

uint8_t TWI::SendByte(uint8_t address, uint16_t reg, uint8_t data)
{
   if(Start() != START_TRANSMITTED)
   {
      return(ERROR);
   }
   // Call device
   if(SendAddress(address, WRITE) != SLA_W_SENT_ACK_RECVD) // Write
   {
      Stop();
      return(ERROR);
   }
   // Send register address
   if(Write(reg) != DATA_BYTE_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   // Send the data
   if(Write(data) != DATA_BYTE_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   // End transmission
   Stop();
   return(SUCCESS);
}

uint8_t TWI::SendWord(uint8_t address, uint16_t reg, uint16_t data)
{
   if(Start() != START_TRANSMITTED)
   {
      return(ERROR);
   }
   // Call device
   if(SendAddress(address, WRITE) != SLA_W_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   // Set register address
   if(Write(reg) != DATA_BYTE_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   // Write the data, send high byte first first
   if(Write(data >> 8) != DATA_BYTE_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   if(Write(data) != DATA_BYTE_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   Stop();
   return(SUCCESS);
}

uint8_t TWI::ReceiveByte(uint8_t device, uint16_t reg, uint8_t &recvdData)
{
   if(Start() != START_TRANSMITTED)
   {
      return(ERROR);
   }

   // Call device
   if(SendAddress(device, WRITE) != SLA_W_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   // Set register address
   if(Write(reg) != DATA_BYTE_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   //_delay_ms(1);
   // Send repeated start to not
   // relinquish the bus.
   if(Start() != REPEATED_START_TRANSMITTED)
   {
      return(ERROR);
   }
   if (SendAddress(device, READ) != SLA_R_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   recvdData = ReadNack();
   if(Status() != DATA_BYTE_RECVD_NOT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   Stop();
   return(SUCCESS);
}

uint8_t TWI::ReceiveWord(uint8_t device, uint16_t reg, uint16_t &recvdData)
{
   if(Start() != START_TRANSMITTED)
   {
      return(ERROR);
   }
   // Call device
   if (SendAddress(device, READ) != SLA_W_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   // Set register
   if(Write(reg) != DATA_BYTE_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }

   if(Start() != REPEATED_START_TRANSMITTED)
   {
      Stop();
      return(ERROR);
   }
   if (SendAddress(device, READ) != SLA_R_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   recvdData = ReadAck();
   if(Status() != DATA_BYTE_RECVD_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   recvdData = (recvdData << 8) | ReadNack();
   if(Status() != DATA_BYTE_RECVD_NOT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }
   Stop();
   return(SUCCESS);
}

uint8_t TWI::Start()
{
   TWCR0 = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
   while ((TWCR0 & (1<<TWINT)) == 0);
   return(Status());
}

uint8_t TWI::SendAddress(uint8_t address, uint8_t rw)
{
   // Write address to the bus.
   // the address is 7 bits, it
   // is shifted to the left by one
   // with a 0 which marks the
   // write flag or a 1 which is
   // the read flag.
   Write(rw == 0 ? address << 1 : (address << 1) | 0x01);
   return(Status()); // Check for ACK.
}

uint8_t TWI::Write(uint8_t data)
{
   TWDR0 = data;
   TWCR0 = (1<<TWINT)|(1<<TWEN);
   while ((TWCR0 & (1<<TWINT)) == 0);
   return(Status());
}

uint8_t TWI::ReadAck()
{
   TWCR0 = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
   while ((TWCR0 & (1<<TWINT)) == 0);
   return TWDR0;
}

uint8_t TWI::ReadNack()
{
   TWCR0 = (1<<TWINT)|(1<<TWEN);
   while ((TWCR0 & (1<<TWINT)) == 0);
   return TWDR0;
}

uint8_t TWI::Status()
{
   //mask status
   return(TWSR0 & 0xF8);// Read the upper 5 bits of the register, mask the lower 3
}

void TWI::Stop()
{
   TWCR0 = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}
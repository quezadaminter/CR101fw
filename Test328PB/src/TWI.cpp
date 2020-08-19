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



static uint16_t twi_lockup_recovery_count = 0;
/**
 * This defines the maximum number of loops various while (...) continue;
 *
 * Note that the amount of time this takes is somewhat dependent on F_CPU and
 * the expression inside the while conditional. It is set by SBWire using
 * fuzzy guessing based on F_CPU.
 *
 * This is chosen over doing millis() or micros() based timeouts to reduce
 * overhead.
 * make twi_maxloops something reasonable. Default I2C
 * freq is 100KHz, so 10 us, so lets wait around 100 us
 *
 * The maths: F_CPU>>20 is dividing by approximately 1e6, so from 16 MHz
 * we get 15. Multiplied by 100 we get 1500. 1500 cycles at 16 MHz is 93.75 us
 */
static volatile uint16_t twi_maxloops = 800;//100 * (F_CPU>>20);
/**
 * Added to allow for the SB_WAIT and SB_DOWHILE macros
 */
static volatile uint16_t twi_loopcnt;
#define SB_WAIT(COND, TO_RET) \
  do { twi_loopcnt = 0; \
    while(COND && twi_loopcnt < twi_maxloops) { twi_loopcnt++;} \
    if (twi_loopcnt >= twi_maxloops) {Stop(); Init(); twi_lockup_recovery_count++; \
		return TO_RET;} \
  } while(0)

#define SB_DOWHILE(COND, TO_RET, LOOP_STATEMENT) \
  do { \
    twi_loopcnt = 0; \
    do { \
      LOOP_STATEMENT; twi_loopcnt++; \
    } while (COND && twi_loopcnt < twi_maxloops); \
    if (twi_loopcnt >= twi_maxloops) {twi_stop(); twi_init(); twi_lockup_recovery_count++; \
		return TO_RET;} \
  } while(0)


// default constructor
TWI::TWI()
{
} //TWI

// default destructor
TWI::~TWI()
{
} //~TWI

void TWI::Init(SpeedT speed)
{
	/* Enable TWI0 */
	PRR0 &= ~(1 << PRTWI0);

   TWCR0 = 0; // This allows the interface to reinitialize.
   Speed(speed);
   //enable TWI
   TWCR0 = (1 << TWEN); // Enable interrupt here eventually
}

void TWI::Speed(SpeedT s)
{
	/* SCLf = F_CPU / (16 + 2 * TWBR0 * TWPS value) */
   // TWBR0 = ((F_CPU / SCLf) - 16) / (2 * TWPS)
   // Use prescaler of 1 until we need to update this.
	TWSR0 = 0x00 << TWPS0; /* SCL precaler: 1, effective bitrate = 100.000kHz */
   TWBR0 = ((F_CPU / 100000UL) - 16) >> 1;
	/* Configured bit rate is 100.000kHz, based on CPU frequency 8.000MHz */
	//TWBR0 = 0x20;          /* SCL bit rate: 100.000kHZ before prescaling */
}

void TWI::Sleep()
{
	/* Enable TWI0 */
	PRR0 |= (1 << PRTWI0);
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

uint8_t TWI::SendBlock(uint8_t address, uint16_t reg, const uint8_t *data, uint32_t len)
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
 
   // Send the message length (limited to a byte for now)
   if(Write(len) != DATA_BYTE_SENT_ACK_RECVD)
   {
      Stop();
      return(ERROR);
   }

   // First byte first (FIFO)
   uint32_t i = 0;
   while(i < len)
   {
      const uint8_t *src = data + i;
      // Write the data, send high byte first
      if(Write(*src) != DATA_BYTE_SENT_ACK_RECVD)
      {
         Stop();
         return(ERROR);
      }
      ++i;
   }

   // Last byte first (LIFO)
   //uint32_t i = len;
   //while(i > 0)
   //{
      //const uint8_t *src = data + (i - 1);
      //// Write the data, send high byte first
      //if(Write(*src) != DATA_BYTE_SENT_ACK_RECVD)
      //{
         //Stop();
         //return(ERROR);
      //}
      //--i;
   //}

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

uint8_t TWI::Ping(uint8_t address)
{
   uint8_t r = Start();
   if(r == START_TRANSMITTED)
   {
      //r = SendAddress(address, READ) != SLA_R_SENT_ACK_RECVD ? ERROR : SUCCESS;
      r = SendAddress(address, WRITE) != SLA_W_SENT_ACK_RECVD ? ERROR : SUCCESS;
   }
   else
   {
      r = ERROR;
   }
   Stop();
   return(r);
}

uint8_t TWI::Start()
{
   TWCR0 = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
   //while ((TWCR0 & (1<<TWINT)) == 0);
   SB_WAIT((TWCR0 & (1<<TWINT)) == 0, ERROR);
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
   return(Write(rw == 0 ? address << 1 : (address << 1) | 0x01));
   //return(Status()); // Check for ACK.
}

uint8_t TWI::Write(uint8_t data)
{
   TWDR0 = data;
   TWCR0 = (1<<TWINT)|(1<<TWEN);
   //while ((TWCR0 & (1<<TWINT)) == 0);
   SB_WAIT((TWCR0 & (1<<TWINT)) == 0, ERROR);
   return(Status());
}

uint8_t TWI::ReadAck()
{
   TWCR0 = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
   //while ((TWCR0 & (1<<TWINT)) == 0);
   SB_WAIT((TWCR0 & (1<<TWINT)) == 0, TWDR0);
   return TWDR0;
}

uint8_t TWI::ReadNack()
{
   TWCR0 = (1<<TWINT)|(1<<TWEN);
   //while ((TWCR0 & (1<<TWINT)) == 0);
   SB_WAIT((TWCR0 & (1<<TWINT)) == 0, TWDR0);
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
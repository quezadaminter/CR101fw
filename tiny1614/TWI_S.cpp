/* 
* TWI_S.cpp
*
* Created: 5/16/2020 4:02:33 PM
* Author: MQUEZADA
*/


#include "TWI_S.h"

TWI_S WireS;

void (*TWI_S::onReceive)(uint8_t);
void (*TWI_S::onRequest)(void);
//uint8_t TWI_S::rxCount = 0;
//uint8_t TWI_S::rxTail = 0;
//uint8_t TWI_S::rxBuffer[TWI_S_RX_BUFFER_SIZE] = { 0 };
uint8_t rxCount = 0;
uint8_t rxTail = 0;
uint8_t rxBuffer[TWI_S_RX_BUFFER_SIZE] = { 0 };

// default constructor
TWI_S::TWI_S()
{
} //TWI_S

// default destructor
TWI_S::~TWI_S()
{
} //~TWI_S

void TWI_S::begin(uint8_t address)
{
   // Initializes the interface.
   I2C_init(address);

   I2C_set_read_callback(TWI_S::onMasterRead);
   I2C_set_write_callback(TWI_S::onMasterWrite);
   I2C_set_address_callback(TWI_S::onAddress);
   I2C_set_stop_callback(TWI_S::onStop);
   I2C_set_collision_callback(TWI_S::onCollision);
   I2C_set_bus_error_callback(TWI_S::onBusError);
   
   // Enables the service
   I2C_open();
}

void TWI_S::onMasterWrite()
{
   // Master sent a byte of data
   if (rxCount < TWI_S_RX_BUFFER_SIZE)
   {
      rxBuffer[rxCount] = I2C_read(); // Get byte from TWI register
      rxCount = ( rxCount + 1 ) & TWI_S_RX_BUFFER_MASK;
      // Acknowledge receipt.
      I2C_send_ack();
   }
   else
   {
      // No more data please, buffer is full.
      I2C_send_nack();
   }
}

void TWI_S::onMasterRead()
{
   if(onRequest != NULL)
   {
      onRequest();
   }
}

void TWI_S::send(uint8_t data)
{
   //txBuffer[txHead] = data;
   I2C_write(data);
}

uint8_t TWI_S::receive()
{
   uint8_t r(0);
   if(rxTail != rxCount)
   {
      r = rxBuffer[rxTail];
      rxTail = ( rxTail + 1 ) & TWI_S_RX_BUFFER_MASK;
   }
   return(r);
}

void TWI_S::onAddress()
{
   // Reset the rx buffer.
   //rxCount = 0;
   // Got the call, picking up the line.
   I2C_send_ack();
}

void TWI_S::onStop()
{
   // Hang up nicely.
   I2C_send_ack();
   if(onReceive != NULL && rxCount != rxTail)
   {
      onReceive(rxCount > rxTail ? (rxCount - rxTail) : (TWI_S_RX_BUFFER_SIZE - rxTail) + rxCount);
   }
}

void TWI_S::onCollision()
{

}

void TWI_S::onBusError()
{

}

void TWI_S::setOnReceive(void (*handler)(uint8_t))
{
   onReceive = handler;
}

void TWI_S::setOnRequest(void (*handler)(void))
{
   // Master wants some amount of data
   onRequest = handler;
}

void TWI_S::Sleep()
{
   I2C_close();
}
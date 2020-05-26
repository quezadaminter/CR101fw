/* 
* SerialIO.cpp
*
* Created: 5/16/2020 7:03:41 PM
* Author: MQUEZADA
*/

#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "SerialIO.h"

SerialIO Serial;

uint8_t TX_BUFFER[SERIAL_TX_BUFFER_SIZE];
uint8_t txHead = 0;
uint8_t txTail = 0;
uint8_t txCount = 0;

// default constructor
SerialIO::SerialIO()
{
} //SerialIO

// default destructor
SerialIO::~SerialIO()
{
} //~SerialIO

/* Baud Rate Calculation
 * Attiny1614's default clock is 20MHz(RC)
 * CLK_PER is 3.333MHz ( default prescaler rate is 6, then 20/6 == 3.3 )
 *
 * Requested Baud Rate: f_baud = 9600[bps]
 * register parameter: BAUD = floor((64 * CLK_PER ) / (N * f_baud))
 *  N = 16 (asynchronous mode)
 *  -> BAUD = 1389
 */
 #define PIN_TX_bp 2 // PORTB2
 #define PIN_TX_bm (1 << PIN_TX_bp)
 #define PIN_RX_bp 3 // PORTB3
 #define PIN_RX_bm (1 << PIN_RX_bp)

void SerialIO::begin(uint32_t speed)
{
   uint16_t PARAM_BAUD = ((64 * F_CPU ) / (16 * speed));

   // USART Configuration
   PORTB.DIRSET = PIN_TX_bm; // TX as OUTPUT, RX as INPUT
   PORTB.OUTSET = PIN_TX_bm; // TX is high
   USART0.BAUD = PARAM_BAUD; // Set Baud Rate
   USART0.CTRLC =
                  USART_CMODE_ASYNCHRONOUS_gc | // Mode: Asynchronous[default]
                  USART_PMODE_DISABLED_gc | // Parity: None[default]
                  USART_SBMODE_1BIT_gc | // StopBit: 1bit[default]
                  USART_CHSIZE_8BIT_gc; // CharacterSize: 8bit[default]
   USART0.CTRLA =
                  USART_RXCIE_bm | // Enable RX interrupt
                  //USART_TXCIE_bm | // Disable TX interrupt
                  USART_DREIE_bm;  // Enable the Data register empty interrupt
   USART0.CTRLB =
                  //USART_RXEN_bm | // Start Receiver
                  USART_TXEN_bm | // Start Transmitter
                  USART_RXMODE_NORMAL_gc; // Receiver mode is Normal USART & 1x-speed

   sei();
}

ISR(USART0_RXC_vect)
{
   // Received byte
   //onReceive(USART0.RXDATAL);
}

ISR(USART0_TXC_vect)
{
}

ISR(USART0_DRE_vect)
{
   if(txTail != txHead)
   {
      USART0.TXDATAL = TX_BUFFER[txTail];
      txTail = ( txTail + 1 ) & SERIAL_TX_BUFFER_MASK;
      txCount--;
   }
   else
   {
      // Stop the interrupt
      USART0.CTRLA &= ~USART_DREIF_bm;
   }
}

void SerialIO::push(uint8_t i)
{
   // If TX buffer is empty
   if(USART0.STATUS & USART_DREIF_bm)
   {
      // Send byte
      USART0.TXDATAL = i;
   }
}

void SerialIO::send()
{
   ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
   {
      while(txTail != txHead)
      {
         push(TX_BUFFER[ txTail ]);
         txTail = ( txTail + 1 ) & SERIAL_TX_BUFFER_MASK;
         txCount--;
      }
   }
}

void SerialIO::write(uint8_t i)
{
   TX_BUFFER[ txHead ] = i;
   txHead = ( txHead + 1 ) & SERIAL_TX_BUFFER_MASK;
   txCount++;
   // Start the interrupt
   USART0.CTRLA |= USART_DREIF_bm;
}

void SerialIO::write(const char *str)
{
	while(*str)
	{
      TX_BUFFER[ txHead ] = *str++;
      txHead = ( txHead + 1 ) & SERIAL_TX_BUFFER_MASK;
      txCount++;
	}
   // Start the interrupt
   USART0.CTRLA |= USART_DREIF_bm;
}

void SerialIO::print(const char *str)
{
   write(str);
}

void SerialIO::println(const char *str)
{
   write(str);
   write('\n');
}

void SerialIO::println(uint8_t i)
{
   print(i);
   write('\n');
}

void SerialIO::print(uint8_t i)
{
	char buf[10] = {'\0'};
	utoa(i, buf, 10);
   print(buf);
}


void SerialIO::print(int16_t i)
{
	char buf[10] = {'\0'};
	ltoa(i, buf, 10);
	println(buf);
}

void SerialIO::print(uint16_t i)
{
	char buf[10] = {'\0'};
	ultoa(i, buf, 10);
	println(buf);
}

void SerialIO::print(uint32_t i)
{
	char buf[10] = {'\0'};
	ultoa(i, buf, 10);
	println(buf);
}

void SerialIO::Sleep()
{
   USART0.CTRLB &= ~(USART_RXEN_bm |  // Disable Receiver
                    USART_TXEN_bm);  // Disable Transmitter
}
/* 
* BasicSerial.cpp
*
* Created: 5/7/2020 9:08:29 AM
* Author: MQUEZADA
*/


#include <stdlib.h>
#include "BasicSerial.h"
#include "BasicSerial3.h"

BasicSerial Serial;

// default constructor
BasicSerial::BasicSerial()
{
} //BasicSerial

// default destructor
BasicSerial::~BasicSerial()
{
} //~BasicSerial

void BasicSerial::send()
{
   while(txTail != txHead)
   {
      TxByte(TX_BUFFER[ txTail ]);
      txTail = ( txTail + 1 ) & TX_BUFFER_MASK;
      txCount--;
   }
}

void BasicSerial::write(uint8_t i)
{
   TX_BUFFER[ txHead ] = i;
   txHead = ( txHead + 1 ) & TX_BUFFER_MASK;
   txCount++;
}

void BasicSerial::write(const char *str)
{
	while(*str)
	{
      TX_BUFFER[ txHead ] = *str++;
      txHead = ( txHead + 1 ) & TX_BUFFER_MASK;
      txCount++;
	}
}

void BasicSerial::print(const char *str)
{
   write(str);
	//while(*str)
	//{
		//TxByte(*str++);
	//}
}

//DGI
void BasicSerial::println(const char *str)
{
   write(str);
   write("\r\n");
	//while(*str)
	//{
		//TxByte(*str++);
	//}
	//TxByte('\r');
	//TxByte('\n');
}

void BasicSerial::println(uint8_t i)
{
   print(i);
   write("\r\n");
   //print(i);
	//TxByte('\r');
	//TxByte('\n');
}

//DGI
void BasicSerial::print(uint8_t i)
{
	char buf[10] = {'\0'};
	utoa(i, buf, 10);
   print(buf);
}


void BasicSerial::print(int16_t i)
{
	char buf[10] = {'\0'};
	ltoa(i, buf, 10);
	println(buf);
}

void BasicSerial::print(uint16_t i)
{
	char buf[10] = {'\0'};
	ultoa(i, buf, 10);
	println(buf);
}

void BasicSerial::print(uint32_t i)
{
	char buf[10] = {'\0'};
	ultoa(i, buf, 10);
	println(buf);
}
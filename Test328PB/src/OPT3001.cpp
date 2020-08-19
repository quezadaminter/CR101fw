/* 
* OPT3001.cpp
*
* Created: 6/15/2020 4:25:11 PM
* Author: MQUEZADA
*/


#include "OPT3001.h"
#include "TWI.h"

OPT3001 opt3001;

// default constructor
OPT3001::OPT3001()
{
} //OPT3001

// default destructor
OPT3001::~OPT3001()
{
} //~OPT3001

void OPT3001::Init()
{
   Init(OPT3001_DEFAULT_CONFIG_800);
}

void OPT3001::Init(uint16_t config)
{
   twi.SendWord(OPT3001_I2C_ADDRESS, OPT3001_CONFIGURATION_REGISTER, config);
}

uint16_t OPT3001::GetManufacturerID()
{
   uint16_t mid(0x0000);
   twi.ReceiveWord(OPT3001_I2C_ADDRESS, OPT3001_MANUFACTURER_ID_REGISTER, mid);
   return(mid);
}

uint16_t OPT3001::GetDeviceID()
{
   uint16_t did(0x0000);
   twi.ReceiveWord(OPT3001_I2C_ADDRESS, OPT3001_DEVICE_ID_REGISTER, did);
   return(did);
}

uint16_t OPT3001::GetConfigRegister()
{
   uint16_t conf(0x0000);
   twi.ReceiveWord(OPT3001_I2C_ADDRESS, OPT3001_CONFIGURATION_REGISTER, conf);
   return(conf);
}

uint16_t OPT3001::GetLowLimitRegister()
{
   uint16_t llimit(0x0000);
   twi.ReceiveWord(OPT3001_I2C_ADDRESS, OPT3001_LOW_LIMIT_REGISTER, llimit);
   return(llimit);
}

uint16_t OPT3001::GetHighLimitRegister()
{
   uint16_t hlimit(0x0000);
   twi.ReceiveWord(OPT3001_I2C_ADDRESS, OPT3001_HIGH_LIMIT_REGISTER, hlimit);
   return(hlimit);
}

uint16_t OPT3001::GetResult()
{
   uint16_t res(0x0000);
   twi.ReceiveWord(OPT3001_I2C_ADDRESS, OPT3001_RESULT_REGISTER, res);

   return ((res & 0x0fff) << ((res & 0xf000) >> 12)) / 100;
}

bool OPT3001::GetConversionReady()
{
   uint16_t res(0x0000);
   twi.ReceiveWord(OPT3001_I2C_ADDRESS, OPT3001_CONFIGURATION_REGISTER, res);

   return(res & OPT3001_CONF_CONVERSION_READYbm);
}

void OPT3001::StartConversion()
{
   Init(OPT3001_DEFAULT_CONFIG_100_OS);
}

void OPT3001::StartConversion(uint16_t convFlags)
{
   Init(convFlags);
}

void OPT3001::Sleep()
{
   Init(OPT3001_DEFAULT_CONFIG_SHDWN);
}
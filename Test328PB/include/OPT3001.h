/* 
* OPT3001.h
*
* Created: 6/15/2020 4:25:12 PM
* Author: MQUEZADA
*/
#include <stdint.h>

#ifndef __OPT3001_H__
#define __OPT3001_H__

/************************************************************************/
/* The registers in this device are double wides, 16 bits long.         */
/************************************************************************/

#define OPT3001_I2C_ADDRESS 0x44

#define OPT3001_RESULT_REGISTER 0x00
#define OPT3001_CONFIGURATION_REGISTER 0x01
#define OPT3001_LOW_LIMIT_REGISTER 0x02
#define OPT3001_HIGH_LIMIT_REGISTER 0x02
#define OPT3001_MANUFACTURER_ID_REGISTER 0x7E
#define OPT3001_DEVICE_ID_REGISTER 0x7F

#define OPT3001_RESULT_EXPONENTbm (0x0F << 12)
#define OPT3001_RESULT_MANTISSAbm (0x0FFF << 0)

#define OPT3001_CONF_RANGE_NUMBERbm (0x0F << 12)
#define OPT3001_CONF_RANGE_NUMBER_DEF B1100
#define OPT3001_CONF_CONVERSION_TIMEbm (1 << 11)
#define OPT3001_CONF_CONVERSION_TIME_DEF 1
#define OPT3001_CONF_MODE_OF_CONVERSIONbm (B11 << 9)
#define OPT3001_CONF_MODE_OF_CONVERSION_DEF B00
#define OPT3001_CONF_OVERFLOW_FLAGbm (1 << 8)
#define OPT3001_CONF_OVERFLOW_FLAG_DEF 1
#define OPT3001_CONF_CONVERSION_READYbm (1 << 7)
#define OPT3001_CONF_CONVERSION_READY_DEF 0
#define OPT3001_CONF_FLAG_HIGHbm (1 << 6)
#define OPT3001_CONF_FLAG_HIGH_DEF 0
#define OPT3001_CONF_FLAG_LOWbm (1 << 5)
#define OPT3001_CONF_FLAG_LOW_DEF 0
#define OPT3001_CONF_LATCH_FIELDbm (1 << 4)
#define OPT3001_CONF_LATCH_FIELD_DEF 1
#define OPT3001_CONF_POLARITY_FIELDbm (1 << 3)
#define OPT3001_CONF_POLARITY_FIELD_DEF 0
#define OPT3001_CONF_MASK_EXPONENTbm (1 << 2)
#define OPT3001_CONF_MASK_EXPONENT_DEF 0
#define OPT3001_CONF_FAULT_COUNTbm (B11 << 0)
#define OPT3001_CONF_FAULT_COUNT_DEF 0

#define OPT3001_LOW_LIMIT_EXPONENTbm (0x0F << 12)
#define OPT3001_LOW_LIMIT_EXPONENT_DEF 0
#define OPT3001_LOW_LIMIT_MANTISSAbm (0x0FFF << 0)
#define OPT3001_LOW_LIMIT_MANTISSA_DEF 0

#define OPT3001_HIGH_LIMIT_EXPONENTbm (0x0F << 12)
#define OPT3001_HIGH_LIMIT_EXPONENT_DEF 0x0B
#define OPT3001_HIGH_LIMIT_MANTISSAbm (0x0FFF << 0)
#define OPT3001_HIGH_LIMIT_MANTISSA_DEF 0x0FFF

#define OPT3001_MANUFACTURER_IDbm (0xFFFF << 0)
#define OPT3001_MANUFACTURER_ID_DEF 0x5449 // TI

#define OPT3001_DEVICE_IDbm (0xFFFF << 0)
#define OPT3001_DEVICE_ID_DEF 0x3001

#define OPT3001_DEFAULT_CONFIG_800    0b1100110000010000 // 800ms
#define OPT3001_DEFAULT_CONFIG_800_OS 0b1100101000010000 // 800ms, one shot

#define OPT3001_DEFAULT_CONFIG_100    0b1100010000010000 // 100ms
#define OPT3001_DEFAULT_CONFIG_100_OS 0b1100001000010000 // 100ms, one shot

#define OPT3001_DEFAULT_CONFIG_SHDWN  0xc810 // Shut down converter

class OPT3001
{
   public:
      OPT3001();
      ~OPT3001();

      void Init(uint16_t config);
      void Init();
      void Sleep();
      uint16_t GetManufacturerID();
      uint16_t GetDeviceID();
      uint16_t GetConfigRegister();
      uint16_t GetLowLimitRegister();
      uint16_t GetHighLimitRegister();
      uint16_t GetResult();
      bool GetConversionReady();
      void StartConversion();
      void StartConversion(uint16_t conversionFlags);
   protected:
   private:
      OPT3001( const OPT3001 &c );
      OPT3001& operator=( const OPT3001 &c );

}; //OPT3001

extern OPT3001 opt3001;
#endif //__OPT3001_H__

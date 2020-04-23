/*
 * AT42QT1070.c
 *
 * Created: 4/15/2020 10:24:47 PM
 *  Author: MQUEZADA
 */ 
#include "AT42QT1070.h"
#include "i2c_simple_master.h"
#include <avr/sfr_defs.h>

#define ADDRESS 0x2E

void Initialize()
{
	
}

bool KeyTouchDetected()
{
   uint8_t s = I2C_0_read1ByteRegister(ADDRESS, DETECTION_STATUS_REGISTER);
   return(bit_is_set(s, TOUCH_DETECTED_BIT));
}

bool Calibrating()
{
   uint8_t s = I2C_0_read1ByteRegister(ADDRESS, DETECTION_STATUS_REGISTER);
   return(bit_is_set(s, CALIBRATE_BIT));
}

bool Overflow()
{
   uint8_t s = I2C_0_read1ByteRegister(ADDRESS, DETECTION_STATUS_REGISTER);
   return(bit_is_set(s, OVERFLOW_BIT));
}

uint8_t KeyStatus()
{
   return(I2C_0_read1ByteRegister(ADDRESS, KEY_STATUS_REGISTER));
}

uint16_t KeySignalValue(uint8_t key)
{
   return(I2C_0_read2ByteRegister(ADDRESS, KEY_SIGNAL_MSB_REGISTER(key)));
}

uint16_t KeyReferenceData(uint8_t key)
{
   return(I2C_0_read2ByteRegister(ADDRESS, KEY_REF_DATA_MSB_REGISTER(key)));
}

void KeyNegativeThresholdLevel(uint8_t k, uint8_t count)
{
   I2C_0_write1ByteRegister(ADDRESS, KEY_NEGATIVE_THRESHOLD_REGISTER(k), count);
}

uint8_t KeyNegativeThresholdLevel(uint8_t key)
{
   return(I2C_0_read1ByteRegister(ADDRESS, KEY_NEGATIVE_THRESHOLD_REGISTER(key)));
}

void KeyAveragingFactor(uint8_t key, uint8_t factor)
{
   if(factor <= KEY_AVERAGING_VALUE_32)
   {
	  uint8_t s = KeyAveragingFactor(key);
      I2C_0_write1ByteRegister(ADDRESS, KEY_AVF_AKS_REGISTER(key), s | (factor << 2));
   }
}

uint8_t KeyAveragingFactor(uint8_t key)
{
   return(I2C_0_read1ByteRegister(ADDRESS, KEY_AVF_AKS_REGISTER(key)));
}

void AdjacentKeySupression(uint8_t key, uint8_t group)
{
   if(group <= KEY_ADJACENT_KEY_SUPPRESSION_GROUP_3)
   {
      uint8_t s = AdjacentKeySupression(key);
      I2C_0_write1ByteRegister(ADDRESS, KEY_AVF_AKS_REGISTER(key), s & (252 + group));
   }
}

uint8_t AdjacentKeySupression(uint8_t key)
{
   return(I2C_0_read1ByteRegister(ADDRESS, KEY_AVF_AKS_REGISTER(key)));
}

void DetectionIntegrator(uint8_t key, uint8_t val)
{
   I2C_0_write1ByteRegister(ADDRESS, KEY_DETECTION_INTEGRATOR_REGISTER(key), val);
}

uint8_t DetectionIntegrator(uint8_t key)
{
   return(I2C_0_read1ByteRegister(ADDRESS, KEY_DETECTION_INTEGRATOR_REGISTER(key)));
}

void FastOutDI(bool set)
{
   uint8_t s = I2C_0_read1ByteRegister(ADDRESS, FO_MO_GUARD_CHANNEL_REGISTER);
   if(set)
   {
	   s |= _BV(FAST_OUT_INTEGRATOR_BIT);
   }
   else
   {
	   s &= ~(_BV(FAST_OUT_INTEGRATOR_BIT));
   }
   I2C_0_write1ByteRegister(ADDRESS, FO_MO_GUARD_CHANNEL_REGISTER, s);
}

bool FastOutDI()
{
   uint8_t s = I2C_0_read1ByteRegister(ADDRESS, FO_MO_GUARD_CHANNEL_REGISTER);
   return(bit_is_set(s, FAST_OUT_INTEGRATOR_BIT));
}

void AllKeysRecalbrate(bool set)
{
   uint8_t s = I2C_0_read1ByteRegister(ADDRESS, FO_MO_GUARD_CHANNEL_REGISTER);
   if(set)
   {
	   s |= _BV(MAX_CALIBRATION_BIT);
   }
   else
   {
	   s &= ~(_BV(MAX_CALIBRATION_BIT));
   }
   I2C_0_write1ByteRegister(ADDRESS, FO_MO_GUARD_CHANNEL_REGISTER, s);
}

bool AllKeysRecalibrate()
{
   uint8_t s = I2C_0_read1ByteRegister(ADDRESS, FO_MO_GUARD_CHANNEL_REGISTER);
   return(bit_is_set(s, MAX_CALIBRATION_BIT));
}

void GuardChannel(uint8_t key)
{
   if(key <= GUARD_CHANNEL_DISABLE)
   {
      uint8_t s = I2C_0_read1ByteRegister(ADDRESS, FO_MO_GUARD_CHANNEL_REGISTER);
      I2C_0_write1ByteRegister(ADDRESS, FO_MO_GUARD_CHANNEL_REGISTER, s & (240 + key));
   }
}
uint8_t GuardChannel()
{
   uint8_t s = I2C_0_read1ByteRegister(ADDRESS, FO_MO_GUARD_CHANNEL_REGISTER);
   return(15 & s);
}

void DisableGuardChannel()
{
	GuardChannel(GUARD_CHANNEL_DISABLE);
}

void LowPowerMode(uint8_t mode)
{
   I2C_0_write1ByteRegister(ADDRESS, LOW_POWER_MODE_REGISTER, mode);
}

uint8_t LowPowerMode()
{
   return(I2C_0_read1ByteRegister(ADDRESS, LOW_POWER_MODE_REGISTER));
}

void MaxOnDuration(uint8_t duration)
{
   I2C_0_write1ByteRegister(ADDRESS, MAX_ON_DURATION_REGISTER, duration);
}

uint8_t MaxOnDuration()
{
   return(I2C_0_read1ByteRegister(ADDRESS, MAX_ON_DURATION_REGISTER));
}

void DisableMaxOnDuration()
{
	MaxOnDuration(0);
}

void StartCalibration()
{
   I2C_0_write1ByteRegister(ADDRESS, CALIBRATE_REGISTER, 0x01);
}

void Reset()
{
   I2C_0_write1ByteRegister(ADDRESS, RESET_REGISTER, 0x01);
}
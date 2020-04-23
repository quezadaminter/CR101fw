/*
 * AT42QT1070.h
 *
 * Created: 4/15/2020 8:12:00 PM
 *  Author: MQUEZADA
 */ 

#ifndef AT42QT1070_H_
#define AT42QT1070_H_

#include <inttypes.h>

// Register addresses
#define CHIP_ID_REGISTER 0
#define FIRMWARE_VERSION_REGISTER 1
#define DETECTION_STATUS_REGISTER 2
#define KEY_STATUS_REGISTER 3
#define KEY_SIGNAL_MSB_REGISTER(k) (4 + (2 * k))
#define KEY_SIGNAL_LSB_REGISTER(k) (5 + (2 * k))
#define KEY_REF_DATA_MSB_REGISTER(k) (18 + (2 * k))
#define KEY_REF_DATA_LSB_REGISTER(k) (19 + (2 * k))
#define KEY_NEGATIVE_THRESHOLD_REGISTER(k) (32 + k)
#define KEY_AVF_AKS_REGISTER(k) (39 + k)
#define KEY_DETECTION_INTEGRATOR_REGISTER(k) (46 + k)
#define FO_MO_GUARD_CHANNEL_REGISTER 53
#define LOW_POWER_MODE_REGISTER 54 // Values are from 0 to 255 which maps to 8 to 2040 milliseconds
#define MAX_ON_DURATION_REGISTER 55 // Values from 0 to 255 where 0 is OFF and 1 to 255 maps to 160 to 40800 milliseconds (160 ms * register value).
#define CALIBRATE_REGISTER 56 // Writing ANY non-zero value here triggers a recalibration
#define RESET_REGISTER 57 // Writing ANY non-zero value triggers a device reset.

#define TOUCH_DETECTED_BIT 0
#define OVERFLOW_BIT 6
#define CALIBRATE_BIT 7

#define KEY_STATUS_BIT(k) (k)
// Pass in the AVF/AKS register
#define KEY_AVERAGING_VALUE_1 1   //(byte)  (byte | (1 << 2))
#define KEY_AVERAGING_VALUE_2 2   //(byte)  (byte | (2 << 2))
#define KEY_AVERAGING_VALUE_4 4   //(byte)  (byte | (4 << 2))
#define KEY_AVERAGING_VALUE_8 8   //(byte)  (byte | (8 << 2))
#define KEY_AVERAGING_VALUE_16 16 //(byte) (byte | (16 << 2))
#define KEY_AVERAGING_VALUE_32 32 //(byte) (byte | (32 << 2))

#define KEY_ADJACENT_KEY_SUPPRESSION_GROUP_0 0  //(byte) (byte & 252)
#define KEY_ADJACENT_KEY_SUPPRESSION_GROUP_1 1  //(byte) (byte & 253)
#define KEY_ADJACENT_KEY_SUPPRESSION_GROUP_2 2  //(byte) (byte & 254)
#define KEY_ADJACENT_KEY_SUPPRESSION_GROUP_3 3  //(byte) (byte & 255)

#define FAST_OUT_INTEGRATOR_BIT 5
#define MAX_CALIBRATION_BIT 4
#define GUARD_CHANNEL_KEY_0 0 //(byte) (byte & 240)
#define GUARD_CHANNEL_KEY_1 1 //(byte) (byte & 241)
#define GUARD_CHANNEL_KEY_2 2 //(byte) (byte & 242)
#define GUARD_CHANNEL_KEY_3 3 //(byte) (byte & 243)
#define GUARD_CHANNEL_KEY_4 4 //(byte) (byte & 244)
#define GUARD_CHANNEL_KEY_5 5 //(byte) (byte & 245)
#define GUARD_CHANNEL_KEY_6 6 //(byte) (byte & 246)
#define GUARD_CHANNEL_DISABLE 7 //(byte) (byte & 247)

#define MAX_ON_DURATION_VALUE_OFF(byte) (byte & 0x00)

void Initialize();
bool KeyTouchDetected();
bool Calibrating();
bool Overflow();
uint8_t KeyStatus();
uint16_t KeySignalValue(uint8_t key);
uint16_t KeyReferenceData(uint8_t key);
void KeyNegativeThresholdLevel(uint8_t k, uint8_t count);
uint8_t KeyNegativeThresholdLevel(uint8_t key);
void KeyAveragingFactor(uint8_t key, uint8_t factor);
uint8_t KeyAveragingFactor(uint8_t key);
void AdjacentKeySupression(uint8_t key, uint8_t group);
uint8_t AdjacentKeySupression(uint8_t key);
void DetectionIntegrator(uint8_t key, uint8_t val);
uint8_t DetectionIntegrator(uint8_t key);
void FastOutDI(bool set);
bool FastOutDI();
void AllKeysRecalbrate(bool set);
bool AllKeysRecalibrate();
void GuardChannel(uint8_t key);
uint8_t GuardChannel();
void DisableGuardChannel();
void LowPowerMode(uint8_t mode);
uint8_t LowPowerMode();
void MaxOnDuration(uint8_t duration);
uint8_t MaxOnDuration();
void DisableMaxOnDuration();
void StartCalibration();
void Reset();
#endif /* AT42QT1070_H_ */
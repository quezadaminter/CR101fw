/*
 * OPT3001.cpp
 *
 * Created: 4/18/2020 4:36:15 PM
 *  Author: MQUEZADA
 */ 
/*

Arduino library for Texas Instruments OPT3001 Digital Ambient Light Sensor
Written by AA for ClosedCube
---

The MIT License (MIT)

Copyright (c) 2015 ClosedCube Limited

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
//#include <Wire.h>
//
//#include "ClosedCube_OPT3001.h"
//
//ClosedCube_OPT3001::ClosedCube_OPT3001()
//{
//}
//
//OPT3001_ErrorCode ClosedCube_OPT3001::begin(uint8_t address) {
	//OPT3001_ErrorCode error = NO_ERROR;
	//_address = address;
	//Wire.begin();
//
	//return NO_ERROR;
//}
//
//uint16_t ClosedCube_OPT3001::readManufacturerID() {
	//uint16_t result = 0;
	//OPT3001_ErrorCode error = writeData(MANUFACTURER_ID);
	//if (error == NO_ERROR)
	//error = readData(&result);
	//return result;
//}
//
//uint16_t ClosedCube_OPT3001::readDeviceID() {
	//uint16_t result = 0;
	//OPT3001_ErrorCode error = writeData(DEVICE_ID);
	//if (error == NO_ERROR)
	//error = readData(&result);
	//return result;
//}
//
//OPT3001_Config ClosedCube_OPT3001::readConfig() {
	//OPT3001_Config config;
	//OPT3001_ErrorCode error = writeData(CONFIG);
	//if (error == NO_ERROR)
	//error = readData(&config.rawData);
	//return config;
//}
//
//OPT3001_ErrorCode ClosedCube_OPT3001::writeConfig(OPT3001_Config config) {
	//Wire.beginTransmission(_address);
	//Wire.write(CONFIG);
	//Wire.write(config.rawData >> 8);
	//Wire.write(config.rawData & 0x00FF);
	//return (OPT3001_ErrorCode)(-10 * Wire.endTransmission());
//}
//
//OPT3001 ClosedCube_OPT3001::readResult() {
	//return readRegister(RESULT);
//}
//
//OPT3001 ClosedCube_OPT3001::readHighLimit() {
	//return readRegister(HIGH_LIMIT);
//}
//
//OPT3001 ClosedCube_OPT3001::readLowLimit() {
	//return readRegister(LOW_LIMIT);
//}
//
//OPT3001 ClosedCube_OPT3001::readRegister(OPT3001_Commands command) {
	//OPT3001_ErrorCode error = writeData(command);
	//if (error == NO_ERROR) {
		//OPT3001 result;
		//result.lux = 0;
		//result.error = NO_ERROR;
//
		//OPT3001_ER er;
		//error = readData(&er.rawData);
		//if (error == NO_ERROR) {
			//result.raw = er;
			//result.lux = 0.01*pow(2, er.Exponent)*er.Result;
		//}
		//else {
			//result.error = error;
		//}
//
		//return result;
	//}
	//else {
		//return returnError(error);
	//}
//}
//
//OPT3001_ErrorCode ClosedCube_OPT3001::writeData(OPT3001_Commands command)
//{
	//Wire.beginTransmission(_address);
	//Wire.write(command);
	//return (OPT3001_ErrorCode)(-10 * Wire.endTransmission(true));
//}
//
//OPT3001_ErrorCode ClosedCube_OPT3001::readData(uint16_t* data)
//{
	//uint8_t	buf[2];
//
	//Wire.requestFrom(_address, (uint8_t)2);
//
	//int counter = 0;
	//while (Wire.available() < 2)
	//{
		//counter++;
		//delay(10);
		//if (counter > 250)
		//return TIMEOUT_ERROR;
	//}
//
	//Wire.readBytes(buf, 2);
	//*data = (buf[0] << 8) | buf[1];
//
	//return NO_ERROR;
//}
//
//
//OPT3001 ClosedCube_OPT3001::returnError(OPT3001_ErrorCode error) {
	//OPT3001 result;
	//result.lux = 0;
	//result.error = error;
	//return result;
//}
//
///*
//
//This is example for ClosedCube OPT3001 Digital Ambient Light Sensor breakout board
//
//Initial Date: 02-Dec-2015
//
//Hardware connections for Arduino Uno:
//VDD to 3.3V DC
//SDA to A4
//SCL to A5
//GND to common ground
//
//Written by AA for ClosedCube
//
//MIT License
//
//*/
///*
//#include <Wire.h>
//#include <ClosedCube_OPT3001.h>
//
//ClosedCube_OPT3001 opt3001;
//
//#define OPT3001_ADDRESS 0x45
//
//void setup()
//{
	//Serial.begin(9600);
	//Serial.println("ClosedCube OPT3001 Arduino Test");
//
	//opt3001.begin(OPT3001_ADDRESS);
	//Serial.print("OPT3001 Manufacturer ID");
	//Serial.println(opt3001.readManufacturerID());
	//Serial.print("OPT3001 Device ID");
	//Serial.println(opt3001.readDeviceID());
//
	//configureSensor();
	//printResult("High-Limit", opt3001.readHighLimit());
	//printResult("Low-Limit", opt3001.readLowLimit());
	//Serial.println("----");
//}
//
//void loop()
//{
	//OPT3001 result = opt3001.readResult();
	//printResult("OPT3001", result);
	//delay(500);
//}
//
//void configureSensor() {
	//OPT3001_Config newConfig;
	//
	//newConfig.RangeNumber = B1100;
	//newConfig.ConvertionTime = B0;
	//newConfig.Latch = B1;
	//newConfig.ModeOfConversionOperation = B11;
//
	//OPT3001_ErrorCode errorConfig = opt3001.writeConfig(newConfig);
	//if (errorConfig != NO_ERROR)
	//printError("OPT3001 configuration", errorConfig);
	//else {
		//OPT3001_Config sensorConfig = opt3001.readConfig();
		//Serial.println("OPT3001 Current Config:");
		//Serial.println("------------------------------");
		//
		//Serial.print("Conversion ready (R):");
		//Serial.println(sensorConfig.ConversionReady,HEX);
//
		//Serial.print("Conversion time (R/W):");
		//Serial.println(sensorConfig.ConvertionTime, HEX);
//
		//Serial.print("Fault count field (R/W):");
		//Serial.println(sensorConfig.FaultCount, HEX);
//
		//Serial.print("Flag high field (R-only):");
		//Serial.println(sensorConfig.FlagHigh, HEX);
//
		//Serial.print("Flag low field (R-only):");
		//Serial.println(sensorConfig.FlagLow, HEX);
//
		//Serial.print("Latch field (R/W):");
		//Serial.println(sensorConfig.Latch, HEX);
//
		//Serial.print("Mask exponent field (R/W):");
		//Serial.println(sensorConfig.MaskExponent, HEX);
//
		//Serial.print("Mode of conversion operation (R/W):");
		//Serial.println(sensorConfig.ModeOfConversionOperation, HEX);
//
		//Serial.print("Polarity field (R/W):");
		//Serial.println(sensorConfig.Polarity, HEX);
//
		//Serial.print("Overflow flag (R-only):");
		//Serial.println(sensorConfig.OverflowFlag, HEX);
//
		//Serial.print("Range number (R/W):");
		//Serial.println(sensorConfig.RangeNumber, HEX);
//
		//Serial.println("------------------------------");
	//}
	//
//}
//
//void printResult(String text, OPT3001 result) {
	//if (result.error == NO_ERROR) {
		//Serial.print(text);
		//Serial.print(": ");
		//Serial.print(result.lux);
		//Serial.println(" lux");
	//}
	//else {
		//printError(text,result.error);
	//}
//}
//
//void printError(String text, OPT3001_ErrorCode error) {
	//Serial.print(text);
	//Serial.print(": [ERROR] Code #");
	//Serial.println(error);
//}
//*/
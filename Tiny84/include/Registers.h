/*
 * Registers.h
 *
 * Created: 5/3/2020 6:20:41 PM
 *  Author: MQUEZADA
 */ 


#ifndef REGISTERS_H_
#define REGISTERS_H_

#define T84_I2C_SLAVE_ADDRESS 0x04 // the 7-bit address (remember to change this when adapting this example)

typedef enum
{
	T84_REG_ID,
	T84_REG_SYSTEM,
	T84_REG_LED_STATE,
	T84_REG_WHITE_PWM_VALUE,
	T84_REG_SCROLL_CLICKS,
	T84_REG_SLEEP,
	T84_REG_I2C_TEST,
	T84_REG_RANGE
} Registers;

#define T84_BOOT_UP_BIT 0

#define T84_ORANGE_BIT 0
#define T84_RED_BIT 1
#define T84_GREEN_BIT 2
#define T84_RG_OFF_BIT 3

#endif /* REGISTERS_H_ */

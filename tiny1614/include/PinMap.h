/*
 * PinMap.h
 *
 * Created: 5/21/2020 9:45:00 PM
 *  Author: MQUEZADA
 */ 

 // PIN       PORT      FUNCTION
 // 1                   VCC
 // 2         PA4       RED
 // 3         PA5       GREEN
 // 4         PA6       AIN6/SCC
 // 5         PA7       SCROLL_INT
 // 6         PB3       LED_PWM
 // 7         PB2       ORANGE
 // 8         PB1       SDA
 // 9         PB0       SCL
 // 10        PA0       !RESET/UPDI
 // 11        PA1       SCR3
 // 12        PA2       SCR2
 // 13        PA3       SCR1
 // 14                  GND



#ifndef PINMAP_H_
#define PINMAP_H_

#define SCROLLINTb 7
#define SCROLLINTbm (1 << SCROLLINTb)
#define SCROLLINT_DIR_OUT (PORTA.DIRSET = SCROLLINTbm)
#define SCROLLINT_DIR_IN (PORTA.DIRCLR = SCROLLINTbm)
#define SCROLLINT_ON (PORTA.OUTSET = SCROLLINTbm)
#define SCROLLINT_OFF (PORTA.OUTCLR = SCROLLINTbm)
#define SCROLLINT_IS_ON (PORTA.OUT & SCROLLINTbm)
#define SCROLLINT_IS_OFF !(PORTA.OUT & SCROLLINTbm)

#define REDLEDb 4
#define REDLEDbm (1 << REDLEDb)
#define REDLED_OFF (PORTA.OUTSET = REDLEDbm)
#define REDLED_ON (PORTA.OUTCLR = REDLEDbm)
#define REDLED_IS_OFF (PORTA.OUT & REDLEDbm)
#define REDLED_IS_ON !(PORTA.OUT & REDLEDbm)
#define REDLED_TOGGLE (PORTA.OUTTGL = REDLEDbm)

#define GREENLEDb 5
#define GREENLEDbm (1 << GREENLEDb)
#define GREENLED_OFF (PORTA.OUTSET = GREENLEDbm)
#define GREENLED_ON (PORTA.OUTCLR = GREENLEDbm)
#define GREENLED_IS_OFF (PORTA.OUT & GREENLEDbm)
#define GREENLED_IS_ON !(PORTA.OUT & GREENLEDbm)
#define GREENLED_TOGGLE (PORTA.OUTTGL = GREENLEDbm)

#define LEDPWMb 3
#define LEDPWMbm (1 << LEDPWMb)
#define LEDPWM_ON (PORTB.OUTSET = LEDPWMbm)
#define LEDPWM_OFF (PORTB.OUTCLR = LEDPWMbm)
#define LEDPWM_IS_ON (PORTB.OUT & LEDPWMbm)
#define LEDPWM_IS_OFF !(PORTB.OUT & LEDPWMbm)
#define LEDPWM_TOGGLE (PORTB.OUTTGL = LEDPWMbm)

#define ORANGELEDb 2
#define ORANGELEDbm (1 << ORANGELEDb)
#define ORANGELED_OFF (PORTB.OUTSET = ORANGELEDbm)
#define ORANGELED_ON (PORTB.OUTCLR = ORANGELEDbm)
#define ORANGELED_IS_OFF (PORTB.OUT & ORANGELEDbm)
#define ORANGELED_IS_ON !(PORTB.OUT & ORANGELEDbm)
#define ORANGELED_TOGGLE (PORTB.OUTTGL = ORANGELEDbm)

#define BIT_SET(reg, bit) (reg & _BV(bit))
#define BIT_CLR(reg, bit) (!(reg & _BV(bit)))

#endif /* PINMAP_H_ */
/*
 * tiny1614.cpp
 *
 * Created: 5/16/2020 4:00:58 PM
 * Author : MQUEZADA
 */ 

 // PIN       PORT      FUNCTION
 // 1                   VCC
 // 2         PA4       GREEN
 // 3         PA5       RED
 // 4         PA4       AIN6/SCC
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "TWI_S.h"
#include "SerialIO.h"
#include "PWM.h"
#include "Timer0.h"
#include "Registers.h"
#include "SonosScrollWheel.h"

volatile uint8_t i2c_regs[T84_REG_RANGE] = { 0 };
volatile uint8_t reg_position = 0;
volatile uint8_t scroll_send = 0;
const uint8_t reg_size = sizeof(i2c_regs);

volatile uint32_t whiteLEDHold = 0;

#define SCROLLINTb 7
#define SCROLLINTbm (1 << SCROLLINTb)
#define SCROLLINT_DIR_OUT (PORTA.DIRSET = SCROLLINTbm)
#define SCROLLINT_DIR_IN (PORTA.DIRCLR = SCROLLINTbm)
#define SCROLLINT_ON (PORTA.OUTSET = SCROLLINTbm)
#define SCROLLINT_OFF (PORTA.OUTCLR = SCROLLINTbm)
#define SCROLLINT_IS_ON (PORTA.OUT & SCROLLINTbm)
#define SCROLLINT_IS_OFF !(PORTA.OUT & SCROLLINTbm)

#define REDLEDb 5
#define REDLEDbm (1 << REDLEDb)
#define REDLED_ON (PORTA.OUTSET = REDLEDbm)
#define REDLED_OFF (PORTA.OUTCLR = REDLEDbm)
#define REDLED_IS_ON (PORTA.OUT & REDLEDbm)
#define REDLED_IS_OFF !(PORTA.OUT & REDLEDbm)
#define REDLED_TOGGLE (PORTA.OUTTGL = REDLEDbm)

#define GREENLEDb 4
#define GREENLEDbm (1 << GREENLEDb)
#define GREENLED_ON (PORTA.OUTSET = GREENLEDbm)
#define GREENLED_OFF (PORTA.OUTCLR = GREENLEDbm)
#define GREENLED_IS_ON (PORTA.OUT & GREENLEDbm)
#define GREENLED_IS_OFF !(PORTA.OUT & GREENLEDbm)
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
#define ORANGELED_ON (PORTB.OUTSET = ORANGELEDbm)
#define ORANGELED_OFF (PORTB.OUTCLR = ORANGELEDbm)
#define ORANGELED_IS_ON (PORTB.OUT & ORANGELEDbm)
#define ORANGELED_IS_OFF !(PORTB.OUT & ORANGELEDbm)
#define ORANGELED_TOGGLE (PORTB.OUTTGL = ORANGELEDbm)

#define SCR3 1
#define SCR3bm (1 << SCR3)
#define SCR3_DIR_OUT (PORTA.DIRSET = SCR3bm)
#define SCR3_DIR_IN (PORTA.DIRCLR = SCR3bm)
#define SCR3_ON (PORTA.OUTSET = SCR3bm)
#define SCR3_OFF (PORTA.OUTCLR = SCR3bm)
#define SCR3_IS_ON (PORTA.OUT & SCR3bm)
#define SCR3_IS_OFF !(PORTA.OUT & SCR3bm)
#define SCR3_TOGGLE (PORTA.OUTTGL = SCR3bm)

#define SCR2 2
#define SCR2bm (1 << SCR2)
#define SCR2_DIR_OUT (PORTA.DIRSET = SCR2bm)
#define SCR2_DIR_IN (PORTA.DIRCLR = SCR2bm)
#define SCR2_ON (PORTA.OUTSET = SCR2bm)
#define SCR2_OFF (PORTA.OUTCLR = SCR2bm)
#define SCR2_IS_ON (PORTA.OUT & SCR2bm)
#define SCR2_IS_OFF !(PORTA.OUT & SCR2bm)
#define SCR2_TOGGLE (PORTA.OUTTGL = SCR2bm)

#define SCR1 3
#define SCR1bm (1 << SCR1)
#define SCR1_DIR_OUT (PORTA.DIRSET = SCR1bm)
#define SCR1_DIR_IN (PORTA.DIRCLR = SCR1bm)
#define SCR1_ON (PORTA.OUTSET = SCR1bm)
#define SCR1_OFF (PORTA.OUTCLR = SCR1bm)
#define SCR1_IS_ON (PORTA.OUT & SCR1bm)
#define SCR1_IS_OFF !(PORTA.OUT & SCR1bm)
#define SCR1_TOGGLE (PORTA.OUTTGL = SCR1bm)

#define BIT_SET(reg, bit) (reg & _BV(bit))
#define BIT_CLR(reg, bit) (!(reg & _BV(bit)))

uint32_t now = 0;

void UpdateRegisters()
{
	switch(reg_position)
	{
		case(T84_REG_LED_STATE):
         BIT_SET(i2c_regs[reg_position], T84_RED_BIT) ? REDLED_OFF : REDLED_ON;
         BIT_SET(i2c_regs[reg_position], T84_GREEN_BIT) ? GREENLED_OFF : GREENLED_ON;
         BIT_SET(i2c_regs[reg_position], T84_ORANGE_BIT) ? ORANGELED_OFF : ORANGELED_ON;
		break;
		case(T84_REG_WHITE_PWM_VALUE):
			pwm.Start();
			//WHITE_PWM_ON;
			if(whiteLEDHold > timer0.millis())
			{
				// In the middle of the hold,
				// extend it again.
				whiteLEDHold = timer0.millis() + 5000;
			}
		break;
		case(T84_REG_SLEEP):
			// Go to sleep!
         //if(i2c_regs[reg_position] != 0)
         //{
			   //Sleep();
			   //SignalUpdate();
         //}
		break;
		case(T84_REG_I2C_TEST):
			for(uint8_t i = 0; i < 4; ++i)
			{
            REDLED_TOGGLE;
				//RED_PIN.Toggle();
				_delay_ms(10);
			}
         if(i2c_regs[reg_position] > 128)
         {
			   i2c_regs[T84_REG_SCROLL_CLICKS] += 1;
         }
         else if(i2c_regs[reg_position] < 128)
         {
			   i2c_regs[T84_REG_SCROLL_CLICKS] -= 1;
         }
         //SignalUpdate();
		break;
		default:
		break;
	}
}

void onRequest()
{
   GREENLED_TOGGLE;
   if(scroll_send != 0)// == T84_REG_SCROLL_CLICKS)
   {
      scroll_send = 0;
      WireS.send(i2c_regs[T84_REG_SCROLL_CLICKS]);
      REDLED_TOGGLE;
      i2c_regs[T84_REG_SCROLL_CLICKS] = 0x80;
   }
   else
   {
      WireS.send(i2c_regs[reg_position]);
   }
   // Move to next register.
   reg_position++;
   if(reg_position > reg_size)
   {
      reg_position = 0;
   }
}

void onReceive(uint8_t data)
{
   if(data == 0 || data > TWI_S_RX_BUFFER_SIZE)
   {
      return;
   }

   // Series of register/value pairs
   //Serial.print("A:");Serial.println(data);
   while(data--)
   {
      reg_position = WireS.receive();
      if(reg_position == T84_REG_SCROLL_CLICKS)
      {
         scroll_send = 1;
      }
      //Serial.print("B:");Serial.println(reg_position);
      if(data > 0 && reg_position < T84_REG_RANGE)
      {
         // Get the next byte and assign it to the
         // selected register.
         i2c_regs[reg_position] = WireS.receive();
         //Serial.print("C:");Serial.println(i2c_regs[reg_position]);
          UpdateRegisters();
         data--;
      }
   }
}

void SignalUpdate()
{
   // Let the master know there is data ready to be sent.
   SCROLLINT_ON;
   SCROLLINT_OFF;
   Serial.println("I");
   _delay_ms(1);
   SCROLLINT_ON;
}

int16_t delta = 0;
void CheckWhiteLEDLevel(uint32_t now)
{
	delta = i2c_regs[T84_REG_WHITE_PWM_VALUE] - TCA0.SINGLE.CMP0;
	if(delta != 0)
	{
		// Move the brightness one step at a time
		// at a rate of 256 steps per 4096 milliseconds.
		// 4096 / 256 = 16ms between PWM steps.
		if(now % 16 == 0)
		{
		   TCA0.SINGLE.CMP0 += (delta > 0 ? 1 : -1);
		   if(TCA0.SINGLE.CMP0 == 255)
		   {
			   //WHITE_PWM_FULL;
			   pwm.setLevel(255);
			   whiteLEDHold = now + 5000;
		   }
		   else if(TCA0.SINGLE.CMP0 == 0)
		   {
			   pwm.setLevel(0);
			   //WHITE_PWM_OFF;
		   }
		}
	}
	else
	{
		// Reached the target.
		if(i2c_regs[T84_REG_WHITE_PWM_VALUE] == 255)
		{
			// At full...
		   if(now > whiteLEDHold)
		   {
			   // ... then fade them off.
			   i2c_regs[T84_REG_WHITE_PWM_VALUE] = 0;
			   //WHITE_PWM_ON;
			   pwm.Start();
		   }
		}
	}
	
}
void InitializeClock()
{
   // ccp_write_io((void*)&(CLKCTRL.OSC32KCTRLA),0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */);

   // ccp_write_io((void*)&(CLKCTRL.XOSC32KCTRLA),CLKCTRL_CSUT_1K_gc /* 1k cycles */
   //		 | 0 << CLKCTRL_ENABLE_bp /* Enable: disabled */
   //		 | 0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */
   //		 | 0 << CLKCTRL_SEL_bp /* Select: disabled */);

   // ccp_write_io((void*)&(CLKCTRL.OSC20MCTRLA),0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */);

   //ccp_write_io((void *)&(CLKCTRL.MCLKCTRLB),
   //             CLKCTRL_PDIV_2X_gc /* 2 */
   //             | 1 << CLKCTRL_PEN_bp /* Prescaler enable: enabled */);

   // 20MHz Oscillator
   _PROTECTED_WRITE(CLKCTRL_MCLKCTRLA, CLKCTRL_CLKSEL_OSC20M_gc);
   // Prescaler /2 for a 10MHz clock
   _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, CLKCTRL_PDIV_2X_gc | (1 << CLKCTRL_PEN_bp));

   // ccp_write_io((void*)&(CLKCTRL.MCLKCTRLA),CLKCTRL_CLKSEL_OSC20M_gc /* 20MHz Internal Oscillator (OSC20M) */
   //		 | 0 << CLKCTRL_CLKOUT_bp /* System clock out: disabled */);

   // ccp_write_io((void*)&(CLKCTRL.MCLKLOCK),0 << CLKCTRL_LOCKEN_bp /* lock enable: disabled */)
}

int main(void)
{
   /* On AVR devices all peripherals are enable from power on reset, this
	 * disables all peripherals to save power. Driver shall enable
	 * peripheral if used */

	/* Set all pins to low power mode */

	for (uint8_t i = 0; i < 8; i++) {
		*((uint8_t *)&PORTA + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
	}

	for (uint8_t i = 0; i < 8; i++) {
		*((uint8_t *)&PORTB + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
	}

   // Initialize IO
    PORTA.DIRSET = REDLEDbm | GREENLEDbm;
    REDLED_ON;
    GREENLED_ON;
    PORTB.DIRSET = LEDPWMbm;
    PORTB.OUTCLR = LEDPWMbm;
    SCROLLINT_DIR_OUT;SCROLLINT_ON;
    SCR1_DIR_OUT;SCR1_OFF;
    SCR2_DIR_OUT;SCR2_OFF;
    SCR3_DIR_OUT;SCR3_OFF;

   InitializeClock();

   Serial.begin(115200);

   WireS.setOnReceive(onReceive);
   WireS.setOnRequest(onRequest);
   WireS.begin(0x04);

   //SW.begin();
   pwm.begin();
   timer0.begin();

    /* Replace with your application code */

    sei();

    uint32_t s = 0;
    uint8_t x = 0;
    uint8_t d = 1;
    while (1) 
    {
       now = timer0.millis();
       //if(now % 1000 == 0)
       //{
          //if(now > s)
          //{
            ////Serial.println("Hello!");
             //s = now + 200;
             //PORTA.OUTTGL = (1 << 4) | (1 << 5);
          //}
       //}
       
       if(now % 5 == 0)
       {
          if(now > s)
          {
             s = now + 2;
             pwm.setLevel(x);
             if(d > 0)
             {
                x++;
             }
             else
             {
                x--;
             }
             if(x == 0xFF || x == 0x00)
             {
                d = !d;
             }
          }
       }
       //CheckWhiteLEDLevel(now);
       //SW.Check();
       // Push the bits out the door.
       //Serial.send();
    }
}


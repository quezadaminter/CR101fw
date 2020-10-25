/*
 * tiny1614.cpp
 *
 * Created: 5/16/2020 4:00:58 PM
 * Author : MQUEZADA
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include "PinMap.h"
#include "TWI_S.h"
#include "SerialIO.h"
#include "PWM.h"
#include "Timer0.h"
#include "Registers.h"
#include "SonosScrollWheel.h"
#include "Timeout.h"

volatile uint8_t i2c_regs[T84_REG_RANGE] = { 0 };
volatile uint8_t reg_position = 0;
volatile uint8_t scroll_send = 0;
uint8_t reg_size = sizeof(i2c_regs);

volatile uint32_t whiteLEDHold = 0;

volatile uint8_t messageReceived = 0;

uint32_t now = 0;

void ScrollWheelTimeoutHandler();
Timeout swTO(0, 250, ScrollWheelTimeoutHandler);


void SignalUpdate()
{
   //// Let the master know there is data ready to be sent.
   SCROLLINT_ON;
   SCROLLINT_OFF;
   ////Serial.println("I");
   _delay_us(100);
   SCROLLINT_ON;
}

void UpdateRegisters()
{
	switch(reg_position)
	{
		case(T84_REG_LED_STATE):
         BIT_SET(i2c_regs[reg_position], T84_RED_BIT) ? REDLED_ON : REDLED_OFF;
         BIT_SET(i2c_regs[reg_position], T84_GREEN_BIT) ? GREENLED_ON : GREENLED_OFF;
         BIT_SET(i2c_regs[reg_position], T84_ORANGE_BIT) ? ORANGELED_ON : ORANGELED_OFF;
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
         //if(i2c_regs[reg_position] > 128)
         //{
			   //i2c_regs[T84_REG_SCROLL_CLICKS] += 1;
         //}
         //else if(i2c_regs[reg_position] < 128)
         //{
			   //i2c_regs[T84_REG_SCROLL_CLICKS] -= 1;
         //}
		break;
		default:
		break;
	}
}

void onRequest()
{
   //GREENLED_TOGGLE;
   if(scroll_send != 0)// == T84_REG_SCROLL_CLICKS)
   {
      scroll_send = 0;
      WireS.send(i2c_regs[T84_REG_SCROLL_CLICKS]);
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
      else if(data > 0 && reg_position < T84_REG_RANGE)
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

int16_t delta = 0;
uint32_t pwmFilter = 0;
void CheckWhiteLEDLevel(uint32_t now)
{
	delta = i2c_regs[T84_REG_WHITE_PWM_VALUE] - TCA0.SINGLE.CMP0;
	if(delta != 0)
	{
		// Move the brightness one step at a time
		// at a rate of 256 steps per 4096 milliseconds.
		// 4096 / 256 = 16ms between PWM steps.
		if(now % 8 == 0 && now > pwmFilter)
		{
         pwmFilter = now + 1;
		   TCA0.SINGLE.CMP0 += (delta > 0 ? 1 : -1);
		   if(TCA0.SINGLE.CMP0 == 255)
		   {
			   pwm.setLevel(255);
            // Start the timeout counter
            // when reaching full brightness.
			   whiteLEDHold = now + 4000;
		   }
		   else if(TCA0.SINGLE.CMP0 == 0)
		   {
			   pwm.setLevel(0);
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

void ScrollWheelTimeoutHandler()
{
   int8_t steps = SW.Check();
   if(steps != 0)
   {
      Serial.println(steps);
      i2c_regs[T84_REG_SCROLL_CLICKS] += (steps > 0 ? 1 : -1);// 0x80;
      if(i2c_regs[T84_REG_SCROLL_CLICKS] > 132)
      {
         i2c_regs[T84_REG_SCROLL_CLICKS] = 129;
         SignalUpdate();
      }
      else if(i2c_regs[T84_REG_SCROLL_CLICKS] < 124)
      {
         i2c_regs[T84_REG_SCROLL_CLICKS] = 127;
         SignalUpdate();
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

ISR(PORTA_PORT_vect)
{
   // We are only using this to wake up from
   // power down sleep. No code here yet.
}

void InitializeIO()
{
   // Initialize IO
    PORTA.DIRSET = REDLEDbm | GREENLEDbm;
    PORTB.DIRSET |= ORANGELEDbm;
    REDLED_OFF;
    GREENLED_OFF;
    ORANGELED_ON;
    SCROLLINT_DIR_OUT;SCROLLINT_ON;

    // Restore the state when waking up.
   i2c_regs[T84_REG_SLEEP] = 0;
   i2c_regs[T84_REG_SCROLL_CLICKS] = 0x80;

   InitializeClock();

   //Serial.begin(115200);

   WireS.setOnReceive(onReceive);
   WireS.setOnRequest(onRequest);
   WireS.begin(0x04);

   SW.begin();
   pwm.begin();
   timer0.begin();

   swTO.Active(true);

    sei();
}

//https://www.avrfreaks.net/forum/attiny3217-1-series-wake-sleep
void Sleep()
{
   // Prepare for sleep
   WireS.Sleep();
   pwm.Sleep();
   Serial.Sleep();
   timer0.Sleep();
   SW.Sleep();
   // Power down peripherials.

   // On unused pins disable input buffer and enable pullup
   PORTA.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
   // Enable the wakeup interrupts
   // In this setup the TWI address match will wake up
   // the chip but we monitor the SCROLLINT pin as well.
   SCROLLINT_OFF;
   SCROLLINT_DIR_IN;
   PORTA.PIN7CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc; // Pullup and falling edge interrupt.
   // Enable global interrupts.
   sei();

   // Set mode and enable sleep.
   SLPCTRL.CTRLA = SLEEP_MODE_PWR_DOWN | SLEEP_ENABLED_gc;
   sleep_cpu();
   InitializeIO();
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

   InitializeIO();

    //uint32_t s = 0;
    //uint8_t x = 0;
    //uint8_t d = 1;

    //REDLED_ON;
    //GREENLED_OFF;
    //ORANGELED_OFF;
    while (1) 
    {
       now = timer0.millis();

       // If there are clicks pending from the scroll wheel
       // then send them.
       //swTO.RunOn(now);
       ScrollWheelTimeoutHandler();
		 //if(i2c_regs[T84_REG_SCROLL_CLICKS] != 128)// && scroll_send == 0)
       //{
			 //for(uint8_t i = 0; i < 4; ++i)
          //{
             //REDLED_TOGGLE;
             //_delay_ms(10);
          //}
          //SignalUpdate();
       //}
       //else if(i2c_regs[T84_REG_SLEEP] != 0)
       //{
          ////Sleep();
          ////SignalUpdate();
       //}
       //if(now % 1000 == 0)
       //{
          //if(now > s)
          //{
             //GREENLED_TOGGLE;
             //REDLED_TOGGLE;
             //ORANGELED_TOGGLE;
            ////Serial.println("Hello!");
             //s = now + 1;
             ////PORTA.OUTTGL = (1 << 4) | (1 << 5);
          //}
       //}
       
       //if(now % 5 == 0)
       //{
          //if(now > s)
          //{
             //s = now + 1;
             //pwm.setLevel(x);
             //if(d > 0)
             //{
                //x++;
             //}
             //else
             //{
                //x--;
             //}
             //if(x == 0xFF || x == 0x00)
             //{
                //d = !d;
             //}
          //}
       //}
       CheckWhiteLEDLevel(now);
       //SW.Check();
       // Push the bits out the door.
       //Serial.send();
    }
}


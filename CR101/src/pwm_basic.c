/**
 * \file
 *
 * \brief PWM Basic driver implementation.
 *
 (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/**
 * \defgroup doc_driver_pwm_basic PWM Basic Driver
 * \ingroup doc_driver_pwm
 *
 * \section doc_driver_pwm_normal_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <pwm_basic.h>

/** Function pointer to callback function called by IRQ.
    NULL=default value: No callback function is to be used.
*/
pwm_irq_cb_t PWM_0_cb = NULL;

/**
 * \brief Initialize PWM
 * If module is configured to disabled state, the clock to the PWM is disabled
 * if this is supported by the device's clock system.
 *
 * \return Initialization status.
 */
int8_t PWM_0_init()
{

	/* Enable TC0 */
	//PRR0 &= ~(1 << PRTIM0);
//
	//TCCR0A = (0 << COM0A1) | (0 << COM0A0)   /* Normal port operation, OCA disconnected */
	         //| (0 << COM0B1) | (0 << COM0B0) /* Normal port operation, OCB disconnected */
	         //| (0 << WGM01) | (1 << WGM00);  /* TC8 Mode 1 Phase Correct */
//
	//// TCCR0B = 0 /* TC8 Mode 1 Phase Correct */
	////		 | (0 << CS02) | (0 << CS01) | (0 << CS00); /* No clock source (Timer/Counter stopped) */
//
	//// TIMSK0 = 0 << OCIE0B /* Output Compare B Match Interrupt Enable: disabled */
	////		 | 0 << OCIE0A /* Output Compare A Match Interrupt Enable: disabled */
	////		 | 0 << TOIE0; /* Overflow Interrupt Enable: disabled */
//
	//// OCR0A = 0x0; /* Output compare A: 0x0 */
//
	//// OCR0B = 0x0; /* Output compare B: 0x0 */

	return 0;
}

/**
 * \brief Enable PWM_0
 * 1. If supported by the clock system, enables the clock to the PWM
 * 2. Enables the PWM module by setting the enable-bit in the PWM control register
 *
 * \return Nothing
 */
void PWM_0_enable()
{
	/* Enable TC0 */
	PRR0 &= ~(1 << PRTIM0);
}

/**
 * \brief Disable PWM_0
 * 1. Disables the PWM module by clearing the enable-bit in the PWM control register
 * 2. If supported by the clock system, disables the clock to the PWM
 *
 * \return Nothing
 */
void PWM_0_disable()
{
	/* Disable TC0 */
	PRR0 |= (1 << PRTIM0);
}

/**
 * \brief Enable PWM output on channel 0
 *
 * \return Nothing
 */
void PWM_0_enable_output_ch0()
{

	TCCR0A |= ((0 << COM0A1) | (0 << COM0A0));
}

/**
 * \brief Disable PWM output on channel 0
 *
 * \return Nothing
 */
void PWM_0_disable_output_ch0()
{

	TCCR0A &= ~((0 << COM0A1) | (0 << COM0A0));
}

/**
 * \brief Enable PWM output on channel 1
 *
 * \return Nothing
 */
void PWM_0_enable_output_ch1()
{

	TCCR0A |= ((0 << COM0B1) | (0 << COM0B0));
}

/**
 * \brief Disable PWM output on channel 1
 *
 * \return Nothing
 */
void PWM_0_disable_output_ch1()
{

	TCCR0A &= ~((0 << COM0B1) | (0 << COM0B0));
}

/**
 * \brief Load COUNTER register in PWM_0
 *
 * \param[in] counter_value The value to load into COUNTER
 *
 * \return Nothing
 */
void PWM_0_load_counter(PWM_0_register_t counter_value)
{
	TCNT0 = counter_value;
}

/**
 * \brief Load duty cycle register in for channel 0.
 * The physical register may have different names, depending on the hardware.
 * This is not the duty cycle as percentage of the whole period, but the actual
 * counter compare value.
 *
 * \param[in] counter_value The value to load into the duty cycle register.
 *
 * \return Nothing
 */
void PWM_0_load_duty_cycle_ch0(PWM_0_register_t duty_value)
{
	OCR0A = duty_value;
}

/**
 * \brief Load duty cycle register in for channel 1.
 * The physical register may have different names, depending on the hardware.
 * This is not the duty cycle as percentage of the whole period, but the actual
 * counter compare value.
 *
 * \param[in] counter_value The value to load into the duty cycle register.
 *
 * \return Nothing
 */
void PWM_0_load_duty_cycle_ch1(PWM_0_register_t duty_value)
{
	OCR0B = duty_value;
}

/**
 * \brief Register a callback function to be called at the end of the overflow ISR.
 *
 * \param[in] f Pointer to function to be called
 *
 * \return Nothing.
 */
void PWM_0_register_callback(pwm_irq_cb_t f)
{
	PWM_0_cb = f;
}

ISR(TIMER0_OVF_vect)
{
	static volatile uint8_t callback_count = 0;

	// Clear the interrupt flag
	TIFR0 |= (1 << TOV0);

	// callback function - called every 0 passes
	if ((++callback_count >= PWM_0_INTERRUPT_CB_RATE) && (PWM_0_INTERRUPT_CB_RATE != 0)) {
		if (PWM_0_cb != NULL) {
			PWM_0_cb();
		}
	}
}


//uint16_t divider = 256;
//
//void Duty( uint8_t percentage, uint16_t ICR1_value)
//{
	//percentage =  (percentage > 100 ? 100 : (percentage < 0 ? 0 : percentage));
	//uint16_t OCR = (uint16_t)(((uint32_t)percentage * (uint32_t)ICR1_value)/100) ;    // Set pwm percent of pwm period
//
	//OCR1AH = OCR >> 8;
	//OCR1AL = OCR & 0xFF;
//}
//
//void FrequencyPWM(uint8_t frequency, uint8_t percentage)
//{
	//uint16_t TOP = F_CPU/(divider*frequency) - 1;
	//ICR1H = TOP >> 8;
	//ICR1L = TOP & 0xFF;
	//Duty(percentage, TOP);
//}
//
//void PWM1_INIT()
//{
	//DDRB |= (1 << PINB1);
	////DDRB |= (1 << PINB2);
	//// Timer/Counter 1 initialization
	//// Clock source: System Clock
	//// Clock value: 62.500 kHz
	//// Mode: Fast PWM top=ICR1
	//// OC1A output: Non-Inverted PWM
	//// OC1B output: Non-Inverted PWM
	//// Noise Canceler: Off
	//// Input Capture on Falling Edge
	//// Timer Period: 1 s
	//// Output Pulse(s):
	//// OC1A Period: 1 s Width: 0.2 s
	//// OC1B Period: 1 s Width: 0.40001 s
	//// Timer1 Overflow Interrupt: Off
	//// Input Capture Interrupt: Off
	//// Compare A Match Interrupt: Off
	//// Compare B Match Interrupt: Off
	//TCCR1A=(1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (0<<WGM10);
	//TCCR1B=(0<<ICNC1) | (0<<ICES1) | (1<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
	//TCNT1H=0x00;
	//TCNT1L=0x00;
//
	//FrequencyPWM(50, 10);
//}
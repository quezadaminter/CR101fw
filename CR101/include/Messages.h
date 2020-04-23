/*
 * Messages.h
 *
 * Created: 4/12/2020 8:56:46 PM
 *  Author: MQUEZADA
 */ 


#ifndef MESSAGES_H_
#define MESSAGES_H_

#define SWITCH_EVENT 0
#define SCROLL_RIGHT_EVENT 1
#define SCROLL_LEFT_EVENT 2
#define REBOOT_EVENT 3

#define SWITCH_MUTE 0
#define SWITCH_VOL_UP 1
#define SWITCH_VOL_DN 2
#define SWITCH_A 3
#define SWITCH_B 4
#define SWITCH_C 5
#define SWITCH_ZONE 6
#define SWITCH_BACK 7
#define SWITCH_MUSIC 8
#define SWITCH_ENTER 9
#define SWITCH_REWIND 10
#define SWITCH_PLAY_PAUSE 11
#define SWITCH_FORWARD 12

#define TOUCH_I2C_ADDRESS 0x1B
#define TOUCH_RESET_REGISTER 0x39
#define TOUCH_RESET_DATA 0xFF
#define TOUCH_STATUS_REGISTER 0x02
#define TOUCH_INPUT_REGISTER 0x03

#define PI_I2C_ADDRESS 0x01
#define PI_EVENT_REGISTER 0x01

// PORTB
#define INT_VOL_DN 5
#define INT_A 4
#define INT_B 3
#define INT_C 2
#define INT_REWIND 1
#define INT_PLAY_PAUSE 0
// PORTC
#define INT_ZONE 2 // PCINT10
#define INT_BACK 3 // PCINT11
#define INT_ENTER 0// PCINT8
// PORTD
#define INT_FORWARD 7 // PCINT23
#define INT_MUSIC 4 // PCINT20
#define INT_TOUCH 2 // PCINT18
// PORTE
#define INT_MUTE 27
#define INT_VOL_UP 26

#define PRESSED 1
#define RELEASE 0

extern uint8_t EVENTS;
extern uint16_t SWITCH_STATES;

#endif /* MESSAGES_H_ */
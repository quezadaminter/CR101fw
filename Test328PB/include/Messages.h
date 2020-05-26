/*
 * Messages.h
 *
 * Created: 4/12/2020 8:56:46 PM
 *  Author: MQUEZADA
 */ 


#ifndef MESSAGES_H_
#define MESSAGES_H_

#define SWITCH_EVENT 0
//#define SCROLL_RIGHT_EVENT 1
//#define SCROLL_LEFT_EVENT 2
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
#define SWITCH_SCROLL_EVENT 13

#define PI_I2C_ADDRESS 0x01
#define PI_EVENT_REGISTER 0x01
#define PI_EVENT_SLEEP_BIT 0
#define PI_EVENT_SHUTDOWN_BIT 1
#define PI_EVENT_REBOOT_BIT 2
#define PI_EVENT_CHARGING_BIT 3
#define PI_BATTERY_LEVEL_REGISTER 0x02

// PORTB
#define INT_VOL_DN 5
#define INT_A 4
#define INT_B 3
#define INT_C 2
#define INT_REWIND 1
#define INT_PLAY_PAUSE 0
// PORTC
#define INT_BACK 3 // PCINT11
#define INT_ZONE 2 // PCINT10
#define INT_ENTER 0// PCINT8
// PORTD
#define INT_FORWARD 7 // PCINT23
#define INT_MUSIC 4 // PCINT20
#define T84_RESET_PIN 3//PORTD2 // PCINT18
#define T84_DATA_READY_INT 1 // PCINT17

// PORTE
#define INT_MUTE 27
#define INT_VOL_UP 26

#define PRESSED 1
#define RELEASE 0

// Key combinations
#define REBOOT_KEY_MASK (_BV(SWITCH_MUSIC) | _BV(SWITCH_MUTE))

extern uint8_t EVENTS;
extern uint16_t SWITCH_STATES;

#endif /* MESSAGES_H_ */
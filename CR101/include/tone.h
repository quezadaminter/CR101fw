/*
 * tone.h
 *
 * Created: 4/14/2020 1:06:31 AM
 *  Author: MQUEZADA
 */ 


#ifndef TONE_H_
#define TONE_H_

typedef struct s_note {
	uint8_t OCRxn; // 0..255
	uint8_t N;
} note_t;

#define C(o)  0,o
#define CS(o) 1,o
#define D(o)  2,o
#define DS(o) 3,o
#define E(o)  4,o
#define F(o)  5,o
#define FS(o) 6,o
#define G(o)  7,o
#define GS(o) 8,o
#define A(o)  9,o
#define AS(o) 10,o
#define B(o)  11,o

typedef struct s_octave {
	note_t note_C;
	note_t note_CS;
	note_t note_D;
	note_t note_DS;
	note_t note_E;
	note_t note_F;
	note_t note_FS;
	note_t note_G;
	note_t note_GS;
	note_t note_A;
	note_t note_AS;
	note_t note_B;
} octave_t;

void tone_setup();
void tone_play_block(uint8_t note, uint8_t octave, uint32_t length);
void tone_play(uint8_t note, uint8_t octave, uint32_t length);
void tone_update(uint32_t now);
void tone_stop(void);

#endif /* TONE_H_ */
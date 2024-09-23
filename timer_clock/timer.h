/*
 * timer.h
 *
 * Created: 9/20/2024 3:52:19 PM
 *  Author: muntasir
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef TIMER_H_
#define TIMER_H_

// in TCCR2
typedef enum {
	NORMAL = 0,
	PHASE_CORRECT_PWM = (1 << WGM20),
	CTC = (1 << WGM21),
	FAST_PWM = (1 << WGM21) | (1 << WGM20)
} timer2_mode_t;

// in TCCR2
typedef enum {
	TIMER2_PRESCALER_None = 0,
	TIMER2_PRESCALER_1 = (1 << CS20),
	TIMER2_PRESCALER_8 = (1 << CS21),
	TIMER2_PRESCALER_32 = (1 << CS21) | (1 << CS20),
	TIMER2_PRESCALER_64 = (1 << CS22),
	TIMER2_PRESCALER_128 = (1 << CS22) | (1 << CS20),
	TIMER2_PRESCALER_256 = (1 << CS22) | (1 << CS21),
	TIMER2_PRESCALER_1024 = (1 << CS22) | (1 << CS21) | (1 << CS20),
} timer2_prescaler_t;

void set_timer2_mode(timer2_mode_t mode);
void start_timer2(timer2_prescaler_t prescaler);
void set_tcnt2(uint8_t value);
void set_ocr2(uint8_t value);
void enable_timer2_comp_int();

extern void (*timer2_comp_callback)();
extern void (*timer2_ovf_callback)();

#endif /* TIMER_H_ */
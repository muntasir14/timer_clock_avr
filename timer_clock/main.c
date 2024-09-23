/*
 * timer_clock.c
 *
 * Created: 9/23/2024 1:16:37 AM
 * Author : muntasir
 */ 

#include <avr/io.h>
#include "lcd_lib.h"
#include "timer.h"

void handle_input_buttons();
void update_display();
void timer2_comp();
void handle_status();

// declare and define variables msc, sc
uint16_t msc = 0;
uint8_t sec = 0;
uint8_t timer_flag = 0;						

int main(void)
{
	// make pb0, pb6, pb7 input pins
	DDRB &= ~((1 << PB7) | (1 << PB6) | (1 << PB0));
	// pull-up input pins
    PORTB |= (1 << PB7) | (1 << PB6) | (1 << PB0);
	
	// pd2 output. buzzer
	DDRD |= (1 << PD2);
	
	LCDinit();
	_delay_ms(50);
	
	// display initial LCD strings
	LCDWriteStringXY(10, 0, "SC:mSC");
	LCDWriteStringXY(10, 1, "00:000");
	
	// ocr = 32, ctc, ps-256
	timer2_comp_callback = timer2_comp;
	set_timer2_mode(CTC);
	enable_timer2_comp_int();
	set_tcnt2(0);
	set_ocr2(31);
	
    while (1) 
    {
		// write input pin conditions
		handle_input_buttons();
		update_display();
		handle_status();
    }
}

void handle_status() {
	if(timer_flag == 2) {
		PORTD |= (1 << PD2);
		start_timer2(TIMER2_PRESCALER_None);
	}
	else						PORTD &= ~(1 << PD2);
}

void handle_input_buttons() {
	if(!(PINB & (1 << PB6)) && (sec >= 5)) {
		sec -= 5;
		_delay_ms(300);
	}
	if(!(PINB & (1 << PB7)) && (sec < 55)) {
		sec += 5;
		_delay_ms(300);
	}
	if(!(PINB & (1 << PB0)) && (sec < 55)) {
		if(!timer_flag) {
			if(sec > 0) {
				start_timer2(TIMER2_PRESCALER_256);
				timer_flag = 1;
			}
		} else {
			start_timer2(TIMER2_PRESCALER_None);
			timer_flag = 0;
		}
		_delay_ms(300);
	}
}

void update_display() {
	char sec_unit = (sec % 10) + '0';
	char sec_dec = (sec / 10) + '0';
	
	char msc_unit = (msc % 10) + '0';
	char msc_dec = ((msc / 10) % 10) + '0';
	char msc_hun = (msc / 100) + '0';
	
	char str[] = {sec_dec, sec_unit, ':', msc_hun, msc_dec, msc_unit, '\0'};
	LCDWriteStringXY(10, 1, str);
}

void timer2_comp() {
	if(sec != 0) {
		if(msc == 0) {
			sec--;
			msc = 999;
		} else			msc--;
	} else {
		if(msc == 0) {
			timer_flag = 2;
		} else			msc --;
	}
}
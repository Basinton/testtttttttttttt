#ifndef _LED7SEG_H_
#define _LED7SEG_H_

#include <p18f4620.h>

// Define pinout
#define DATA_PIN PORTDbits.RD2
#define CLK_PIN PORTDbits.RD3
#define LATCH_PIN PORTDbits.RD4
#define ROOT 10

extern unsigned int LED7_mask[11];

void control_led(int num1, int num2);
void display_led(int data);
void write_8bit(int num);
void delay_ms_7seg(int value);


#endif
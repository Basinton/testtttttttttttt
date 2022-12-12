#ifndef _TRAFFIC_LED_H_
#define _TRAFFIC_LED_H_

#include <p18f4620.h>

#define DATA_PIN_TRAFFIC        PORTBbits.RB0
#define CLK_PIN_TRAFFIC         PORTBbits.RB1
#define LATCH_PIN_TRAFFIC       PORTDbits.RD0

#define     MASK1       0xF0
#define     MASK2       0x0F

#define     RED1        0x08
#define     YELLOW1     0x04
#define     GREEN1      0x02

#define     RED2        0x40
#define     YELLOW2     0x20
#define     GREEN2      0x10

extern unsigned char cur_status;

void write_8bit_traffic(int num);
void control_traffic_led(int signal);

void setAllLeds(void);
void offAllLeds(void);

void blinkLeds(void);
void blinkReds(void);
void blinkYellows(void);
void blinkGreens(void);

void setRed1(void);
void setYellow1(void);
void setGreen1(void);

void setRed2(void);
void setYellow2(void);
void setGreen2(void);

void delay_ms_traffic(int value);

#endif
#include "traffic_led.h"

unsigned char cur_status = 0x00;

void setAllLeds(void) {
    //set on traffic leds
    control_traffic_led(0xFF);
}

void offAllLeds(void) {
    //set off traffic leds
    cur_status = 0x00;
    control_traffic_led(cur_status);
}

void blinkLeds(void) {
    offAllLeds();
    control_traffic_led(~cur_status);
}

void blinkReds(void){
    
    cur_status = cur_status ^ 0x48;
    control_traffic_led(cur_status);
}

void blinkYellows(void){
    offAllLeds();
    cur_status = cur_status ^ 0x24;
    control_traffic_led(cur_status);
}

void blinkGreens(void){
    offAllLeds();
    cur_status = cur_status ^ 0x12;
    control_traffic_led(cur_status);
}

void setRed1(void) {
    cur_status &= MASK1;
    cur_status |= RED1;
    control_traffic_led(cur_status);
}

void setYellow1(void) {
    cur_status &= MASK1;
    cur_status |= YELLOW1;
    control_traffic_led(cur_status);
}

void setGreen1(void) {
    cur_status &= MASK1;
    cur_status |= GREEN1;
    control_traffic_led(cur_status);
}

void setRed2(void) {
    cur_status &= MASK2;
    cur_status |= RED2;
    control_traffic_led(cur_status);
}

void setYellow2(void) {
    cur_status &= MASK2;
    cur_status |= YELLOW2;
    control_traffic_led(cur_status);
}

void setGreen2(void) {
    cur_status &= MASK2;
    cur_status |= GREEN2;
    control_traffic_led(cur_status);
}

void write_8bit_traffic(int num) {
    int i = 0;
    int Data_bit;
    unsigned int Temp = num;
    CLK_PIN_TRAFFIC = 0;
    for (i; i < 8; i++) {
        Data_bit = Temp & 0x80;
        Temp = Temp << 1;
        DATA_PIN_TRAFFIC = (Data_bit == 0x80);
        CLK_PIN_TRAFFIC = 1;
        delay_ms_traffic(1);
        CLK_PIN_TRAFFIC = 0;
    }
}

void control_traffic_led(int signal) {
    // Write Led 1
    LATCH_PIN_TRAFFIC = 0;
    write_8bit_traffic(signal);
    LATCH_PIN_TRAFFIC = 1;
    delay_ms_traffic(1);
    LATCH_PIN_TRAFFIC = 0;
}

void delay_ms_traffic(int value) {
    int i, j;
    for (i = 0; i < value; i++)
        for (j = 0; j < 238; j++);
}
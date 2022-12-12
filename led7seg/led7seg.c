#include "led7seg.h"

void write_8bit(int num) {
    int i = 0;
    int Data_bit;
    unsigned int Temp = LED7_mask[num];
    CLK_PIN = 0;
    for (i; i < 8; i++) {
        Data_bit = Temp & 0x80;
        Temp = Temp << 1;
        DATA_PIN = (Data_bit == 0x80);
        CLK_PIN = 1;
        delay_ms_7seg(1);
        CLK_PIN = 0;
    }
}

void display_led(int data) {
    int data1 = data / 10;
    int data2 = data % 10;
    
    if (data1 == 0)
    {
        data1 = ROOT;
    }

    control_led(data1, data2);
}

void control_led(int num1, int num2) {
    LATCH_PIN = 0;
    // Write Led 1
    write_8bit(ROOT);
    write_8bit(num1);
    
    LATCH_PIN = 1;
    delay_ms_7seg(1);
    LATCH_PIN = 0;
    delay_ms_7seg(1);
    
    //Write root signal
    write_8bit(num2);
    write_8bit(ROOT);
    
    LATCH_PIN = 1;
    delay_ms_7seg(1);
    LATCH_PIN = 0;
    delay_ms_7seg(1);
}

void delay_ms_7seg(int value) {
    int i, j;
    for (i = 0; i < value; i++)
        for (j = 0; j < 238; j++);
}


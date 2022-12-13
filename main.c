#include "main.h"
// Noi khai bao hang so
#define PORTD_OUT   PORTD
#define TRISD_OUT   TRISD

void init_output(void);
// Noi khai bao bien toan cuc
unsigned int LED7_mask[11] = {~0x3F, ~0x06, ~0x5B, ~0x4F, ~0x66, ~0x6D, ~0x7D, ~0x07, ~0x7F, ~0x6F, ~0x00};
// Khai bao cac ham co ban IO
void init_system(void);
void init_leds(void);
void delay_ms(int value);

// Den giao thong
#define     INIT_SYSTEM         255
#define     PHASE1              0
#define     PHASE2              1
#define     PHASE3              2
#define     PHASE4              3
#define     WAIT                4

#define     AUTO_RED1_GREEN2    100
#define     AUTO_RED1_YELLOW2   101
#define     AUTO_GREEN1_RED2    102
#define     AUTO_YELLOW1_RED2   103

#define     MAN_RED1_GREEN2     200
#define     MAN_RED1_YELLOW2    201
#define     MAN_GREEN1_RED2     202
#define     MAN_YELLOW1_RED2    203

#define     TUN_RED             300
#define     TUN_GREEN           301
#define     TUN_YELLOW          302

#define     BASE_GREEN_TIME     3
#define     BASE_YELLOW_TIME    2
#define     BASE_RED_TIME       5
#define     WAIT_TIME           10

unsigned char statusOfLight = INIT_SYSTEM;
unsigned char timeOfGreen = BASE_GREEN_TIME;
unsigned char timeOfYellow = BASE_YELLOW_TIME;
unsigned char timeOf7Seg1 = BASE_RED_TIME;
unsigned char timeOf7Seg2 = BASE_GREEN_TIME;

unsigned char timeCounter = 0;
unsigned char trafficCounter = 0;
unsigned char secCounter = 20;
unsigned char waitCounter = 0;
unsigned char tempTrafficCounter = 0;

void AppTrafficLight();
void fsm_automatic();
void fsm_manual();
void fsm_tuning();
void UartDataReceiveProcess();
void UartDataReceiveProcess_ElectronicDeviceControl();
////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////

void main(void) {
    unsigned int k = 0;
    init_system();
    delay_ms(1000);
    while (1) {
        if (flag_timer3 == 1) {
            SetTimer3_ms(50);

            AppTrafficLight();
            fsm_manual();

            scan_key_matrix_with_uart(); // 8 button
//            DisplayDataReceive();
//            UartDataReceiveProcess();
            DisplayLcdScreen();
        }


    }
}
// Hien thuc cac module co ban cua chuong trinh

void delay_ms(int value) {
    int i, j;
    for (i = 0; i < value; i++)
        for (j = 0; j < 238; j++);
}

void init_output(void) {
    TRISD_OUT = 0x00;
    PORTD_OUT = 0x00;
    TRISB = 0x00;
    PORTB = 0x00;

}

void init_system(void) {
    statusOfLight = INIT_SYSTEM;
    init_output();
    offAllLeds();
    control_led(10, 10);
    lcd_init();
    init_key_matrix_with_uart();
    init_interrupt();
    init_uart();
    lcd_clear();
    LcdClearS();
    delay_ms(500);
    init_timer3(46950); //dinh thoi 10ms
    SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input, proccess, output
}

void AppTrafficLight() {
    switch (statusOfLight) {
        case INIT_SYSTEM: //RED1 GREEN2
            trafficCounter = timeOfGreen;
            timeOf7Seg1 = timeOfGreen + timeOfYellow;
            timeOf7Seg2 = timeOfGreen;
            setRed1();
            setGreen2();
            display_led(timeOf7Seg1);
            LcdPrintStringS(0,0,"TRAFFIC PROJECT");
            LcdPrintStringS(1,0,"SYSTEM'S WORKING");
            statusOfLight = PHASE1;
            break;
        case PHASE1: //RED1 YELLOW2 
            display_led(timeOf7Seg1);
            if (secCounter <= 0) {
                timeOf7Seg1--;
                timeOf7Seg2--;
                secCounter = 20;
                trafficCounter--;
            }
            secCounter--;

            if (trafficCounter <= 0) {
                setYellow2();
                trafficCounter = timeOfYellow;
                timeOf7Seg2 = timeOfYellow;
                statusOfLight = PHASE2;
            }

            if (isButtonEnter(7)) {
                offAllLeds();
                setRed1();
                setRed2();
                waitCounter = WAIT_TIME;
                statusOfLight = MAN_RED1_GREEN2;
            }
            break;
        case PHASE2: //GREEN1 RED2
            display_led(timeOf7Seg1);
            if (secCounter <= 0) {
                timeOf7Seg1--;
                timeOf7Seg2--;
                secCounter = 20;
                trafficCounter--;
            }
            secCounter--;

            if (trafficCounter <= 0) {
                trafficCounter = timeOfGreen;
                setGreen1();
                setRed2();
                timeOf7Seg1 = timeOfGreen;
                timeOf7Seg2 = timeOfGreen + timeOfYellow;
                statusOfLight = PHASE3;
            }
            break;
        case PHASE3: //YELLOW1 RED2
            display_led(timeOf7Seg1);
            if (secCounter <= 0) {
                timeOf7Seg1--;
                timeOf7Seg2--;
                secCounter = 20;
                trafficCounter--;
            }
            secCounter--;

            if (trafficCounter <= 0) {
                trafficCounter = timeOfYellow;
                SetTimer3_ms(50);
                setYellow1();
                timeOf7Seg1 = timeOfYellow;
                statusOfLight = PHASE4;
            }
            break;
        case PHASE4: //RED1 GREEN2
            display_led(timeOf7Seg1);
            if (secCounter <= 0) {
                timeOf7Seg1--;
                timeOf7Seg2--;
                secCounter = 20;
                trafficCounter--;
            }
            secCounter--;

            if (trafficCounter <= 0) {
                trafficCounter = timeOfGreen;
                setRed1();
                setGreen2();
                timeOf7Seg1 = timeOfGreen + timeOfYellow;
                timeOf7Seg2 = timeOfGreen;
                statusOfLight = PHASE1;
            }
            break;
        default:
            //            statusOfLight = INIT_SYSTEM;
            break;
    }
}

void fsm_manual() {
    switch (statusOfLight) {
        case MAN_RED1_GREEN2:
            display_led(waitCounter);
            if (secCounter <= 0) {
                secCounter = 20;
                blinkReds();
                waitCounter--;
            }
            secCounter--;

            if (waitCounter <= 0) {
                statusOfLight = INIT_SYSTEM;
            }
            break;

        case MAN_RED1_YELLOW2:
            setRed1();
            setYellow2();
            break;

        case MAN_GREEN1_RED2:
            setGreen1();
            setRed2();
            break;

        case MAN_YELLOW1_RED2:
            setYellow1();
            setRed2();
            break;

        default:
            break;
    }
}

//void fsm_tuning() {
//    switch (statusOfLight) {
//        case TUN_RED:
//            break;
//
//        case TUN_GREEN:
//            break;
//
//        case TUN_YELLOW:
//            break;
//
//        default:
//            break;
//    }
//}

void UartDataReceiveProcess() {
    if (flagOfDataReceiveComplete == 1) {
        flagOfDataReceiveComplete = 0;
        if (dataReceive[4] == 0) {
            timeOfGreen = dataReceive[0];
            timeOfYellow = dataReceive[1];
        }
    }
    LcdPrintNumS(0, 15, statusReceive);
}
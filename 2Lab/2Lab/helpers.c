#include "states.h"
#include "helpers.h"
#include <msp430.h>
#include <stdlib.h>
#include "peripherals.h"

#define A 440
#define Bflat 466
#define B 494
#define C 523
#define Csharp 554
#define D 587
#define Eflat 622
#define E 659
#define F 698
#define Fsharp 740
#define G 784
#define Aflat 831

#define S1PRSSD 0x01
#define S2PRSSD 0x02
#define S3PRSSD 0x04
#define S4PRSSD 0x08

#define R1REDLED BIT2
#define R2YELLOWLED BIT1
#define R3BLUELED BIT3
#define R4GREENLED BIT4

unsigned char getButtonState(void) {

    unsigned char ret_val = 0x00;

    /* checks if S1 has been pressed */
    if ((P7IN & BIT0) == 0x00) {
        ret_val += 0x01;
    }
    /* checks if S2 has been pressed */
    if ((P3IN & BIT6) == 0x00) {
        ret_val += 0x02;
    }
    /* checks if S3 has been pressed */
    if ((P2IN & BIT2) == 0x00) {
        ret_val += 0x04;
    }
    /* checks if S4 has been pressed */
    if ((P7IN & BIT4) == 0x00) {
        ret_val += 0x08;
    }

    return ret_val;
}

void configButtons(void) {
    /* configures buttons for IO */
    P7SEL &= ~(BIT4|BIT0);
    P3SEL &= ~(BIT6); // configures button for IO
    P2SEL &= ~(BIT2);

    /* configures buttons for input */
    P7DIR &= ~(BIT4|BIT0);
    P3DIR &= ~(BIT6);
    P2DIR &= ~(BIT2);

    /* configures buttons for pull resistors */
    P7REN |= (BIT4|BIT0);
    P3REN |= (BIT6);
    P2REN |= (BIT2);

    /* configures buttons for pull down resistors */
    P7OUT |= (BIT4|BIT0);
    P3OUT |= (BIT6);
    P2OUT |= (BIT2);
}


void displayLeds(char button_state) {
    //takes the output of the button state and configures corresponding LEDs to light up

    // Turn all LEDs off to start
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);

    switch (button_state) {
        case (S1PRSSD):
            P6OUT |= R1REDLED;
            break;
        case (S2PRSSD):
            P6OUT |= R2YELLOWLED;
            break;
        case (S3PRSSD):
            P6OUT |= R3BLUELED;
            break;
        case (S4PRSSD):
            P6OUT |= R4GREENLED;
            break;
        case (S1PRSSD | S2PRSSD):
            P6OUT |= (R1REDLED | R2YELLOWLED);
            break;
        case (S1PRSSD | S3PRSSD):
            P6OUT |= (R1REDLED | R3BLUELED);
            break;
        case (S1PRSSD | S4PRSSD):
            P6OUT |= (R1REDLED | R4GREENLED);
            break;
        case (S2PRSSD | S3PRSSD):
            P6OUT |= (R2YELLOWLED | R3BLUELED);
            break;
        case (S2PRSSD | S4PRSSD):
            P6OUT |= (R2YELLOWLED | R4GREENLED);
            break;
        case (S3PRSSD | S4PRSSD):
            P6OUT |= (R3BLUELED | R4GREENLED);
            break;
        default:
            P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);
    }
}

void displayMessage(char* message) {

    Graphics_clearDisplay(&g_sContext); // clears the display

    // writes message
    Graphics_drawStringCentered(&g_sContext, message, AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);

    // flushes text to the display
    Graphics_flushBuffer(&g_sContext);
}

enum ret_codes check_keypad(void) {
    unsigned char currKey = 0;
    currKey = getKey();
    if (currKey == '*') {
            return pass;
    } else {
            return repeat;
    }
}







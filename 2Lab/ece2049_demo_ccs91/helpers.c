#include "helpers.h"
#include <msp430.h>

/*
#define S1PRSSD 0x01
#define S2PRSSD 0x40
#define S3PRSSD 0x04
#define S4PRSSD 0x10
*/

unsigned char get_button_state(void) {

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

void config_buttons(void) {
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


void configUserLED(char inbits) {
    //takes the output of the button state and configures corresponding LEDs to light up
    //should light up 2 LEDs- does not do that
    // if hex value = inputed bit

    // Turn all LEDs off to start
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);

    if ((BIT4 & inbits) == 0) {
    // inbits |= BIT4;   // Right most LED P6.4 //YELLOW LED
    P6OUT |= BIT4;

    }

    if (( BIT3 & inbits) == 0) {
    //  inbits |= BIT3;   //LED that is one in from far right P6.3 //BLUE LED
    P6OUT |= BIT3;
    }

    if((BIT1 & inbits) == 0) {
    // inbits |= BIT1;   // LED that is one in from far left, P6.1 //GREEN LED
    P6OUT |= BIT1;
    }

    if ((BIT2 & inbits) == 0) {
    // inbits |= BIT2;   // Left most LED on P6.2 // RED LED
    P6OUT |= BIT2;
    }

    timeDelay(1);
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);
}







#include "helpers.h"
#include <msp430.h>
#include "peripherals.h"
#include <stdio.h>

// global variables
#define CALADC12_15V_30C *((unsigned int *) 0x1A1A) //address from datasheet, storing 30 C
#define CALADC12_15V_85C *((unsigned int *) 0x1A1C) //address from datasheet, storing 85 C
long unsigned int in_temp;
long unsigned int timer_cnt;

#pragma vector = TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR(void) {
    timer_cnt++;
}

void main (void) {

    WDTCTL = WDTPW | WDTHOLD; /* stops watchdog timer. Always need to stop this!! */
    _BIS_SR(GIE); // enables global interrupts

    timer_cnt = 0;

    /* initializes board */
    setA2();
    runTimerA2();
    configDisplay();

    /* checks that the game only runs when an error has not occured */
    while (1) {
        displayTime(timer_cnt);
    }
}

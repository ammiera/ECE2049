#include "helpers.h"
#include <msp430.h>
#include "peripherals.h"
#include <stdio.h>

// global variables
#define CALADC12_15V_celcius_30 *((unsigned int *) 0x1A1A) // address from datasheet, storing 30 C
#define CALADC12_15V_celcius_85 *((unsigned int *) 0x1A1C) // address from datasheet, storing 85 C
long unsigned int timer_cnt;

#pragma vector = TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR(void) {
    timer_cnt++;
}

void main (void) {
    // declare temp and time variables
    volatile float degC_temp;
    float degC_temp_avg;
    volatile float degC_per_bit;
    volatile unsigned int degC_30, degC_85;
    long unsigned int temp_code;
    float degC_temp_history[TEMPHISTORYLEN];
    float* ptr_degC_temp_history = degC_temp_history;
    int num_temp_samples;

    timer_cnt = 0;
    num_temp_samples = 0;

    // defines temp and time variables
    degC_30 = CALADC12_15V_celcius_30;
    degC_85 = CALADC12_15V_celcius_85;
    degC_per_bit = ((float)(85.0 - 30.0))/((float)(degC_85-degC_30));
    degC_temp_history[TEMPHISTORYLEN - 1] = ENDOFARRAY; // null terminates the array with number that cannot be read by the sensor

    WDTCTL = WDTPW | WDTHOLD; // stops watchdog timer
    _BIS_SR(GIE); // enables global interrupts

    // initializes the board
    enableADC12();
    setA2();
    runTimerA2();
    configDisplay();

    while (1) {
        displayTime(timer_cnt);
        temp_code = sampleTemp();

        // calculates temperature in Celsius
        // please see the Device Descriptor Table section in the
        // "System Resets, Interrupts, and Operating Modes, System Control Module"
        // chapter in the device user's guide for background information on the formula.
        degC_temp = (float)((long)temp_code - CALADC12_15V_celcius_30) * degC_per_bit +30.0;
        num_temp_samples++;
        ptr_degC_temp_history = insertTemp(degC_temp, ptr_degC_temp_history, TEMPHISTORYLEN);
        degC_temp_avg = calcAvgTemp(degC_temp_history, num_temp_samples, TEMPHISTORYLEN);

        displayTemp(degC_temp_avg);

        __no_operation();
    }
}

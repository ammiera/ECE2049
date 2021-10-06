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
    unsigned int time_state = 0;

    unsigned int button_pressed;
    unsigned int potentiometer_state;
    unsigned int cur_time_unit = 0;

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
    configPotentiometer();
    configLaunchPadButtons(); //I assume

    while (1) {
        startADC();
        Graphics_clearDisplay(&g_sContext); //flushes text to the display, updating it

        button_pressed = checkButton();
        if (button_pressed == LEFTBUTTON) {

            // keeps current time unit as months at first button press
            if (cur_time_unit != SECOND) {
                cur_time_unit++;
            }

            if (cur_time_unit > MONTH) {
                cur_time_unit = SECOND;
            }

            potentiometer_state = checkPotentiometer();


        }

        if (button_pressed == RIGHTBUTTON) {
        }



        displayTime(timer_cnt, 0, 0);



        time_state = editTime(time_state, timer_cnt);
        temp_code = sampleTemp();

        // calculates temperature in Celsius
        // please see the Device Descriptor Table section in the
        // "System Resets, Interrupts, and Operating Modes, System Control Module"
        // chapter in the device user's guide for background information on the formula.
        degC_temp = (float)((long)temp_code - CALADC12_15V_celcius_30) * degC_per_bit +30.0;
        num_temp_samples++; // keeps track of the number of temp samples taken
        ptr_degC_temp_history = insertTemp(degC_temp, ptr_degC_temp_history, TEMPHISTORYLEN); // inserts temp into temp history array
        degC_temp_avg = calcAvgTemp(degC_temp_history, num_temp_samples, TEMPHISTORYLEN); // caculates the average temperature

        displayTemp(degC_temp_avg); // displays average temp


        __no_operation();
    }
}

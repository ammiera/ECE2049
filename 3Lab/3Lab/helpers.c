#include "helpers.h"
#include <msp430.h>
#include <stdlib.h>
#include "peripherals.h"
#include <math.h>

// configures and enables the ADC
void enableADC12(void) {
    REFCTL0 &= ~REFMSTR; // resets REFMSTR to hand over control of internal
                         // reference voltages to ADC12_A control registers

    // ADC12SHT0_9 = 384 ADC12CLK (ACLK) cycles between each sample
    // ADC12REFON = turns reference generator on with default 1.5 V
    // ADC12ON = turns ADC on
    // ADC12MSC = enables multiple conversions
    ADC12CTL0 = ADC12SHT0_9 | ADC12REFON | ADC12ON | ADC12MSC;

    // ADC12SHP = enables sample timer
    // ADC12CONSEQ_1 = enables multiple channel conversions
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;

    // ADC12SREF_1 = uses internal reference 1.5 V
    // ADC12INCH_10 = A2 input channel of temperture sensor
    // ADC12MCTL0 = memory location to store conversion
    ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;

    // ADC12SREF_0 = uses Vref+ = VCC
    // ADC12INCH_5 = A5 input channel of scroll wheel
    // ADC12MCTL1 = memory location to store conversion
    ADC12MCTL1 = ADC12SREF_0 + ADC12INCH_5 + ADC12EOS;

    __delay_cycles(100); // give refernce voltage time to settle to 1.5 v
    ADC12CTL0 |= ADC12ENC; // enable conversion
}

// configures the potentiometer port settings
void configPotentiometer(void) {
    P8SEL &= ~BIT0;
    P8DIR |= BIT0;
    P8OUT |= BIT0;
}

// starts the ADC and waits for a sample conversion to complete
void startADC(void) {
    ADC12CTL0 &= ~ADC12SC; // clears the start bit
    ADC12CTL0 |= ADC12SC; // starts sampling and conversion

    while (ADC12CTL1 & ADC12BUSY) // waits for conversion to complete
        __no_operation();
}

// samples the temperature from the temperature sensor
long unsigned int sampleTemp(void) {
    long unsigned int temp_code;
    temp_code = ADC12MEM0; // read in results if conversion

    return temp_code;
}

// sets the A2 clock to its default settings
void setA2(void) {
    P5SEL |= (BIT5 | BIT4 | BIT3 | BIT2 | BIT1);

    // defaults
    UCSCTL0 = 0x14B8;
    UCSCTL1 = 0x0020;
    UCSCTL2 = 0x101F;
    UCSCTL3 = 0x0000;
    UCSCTL4 = 0x0044;
    UCSCTL5 = 0x0000;
    UCSCTL6 = 0xC1CD;
    UCSCTL7 = 0x0403;
    UCSCTL8 = 0x0707;
}

// enables the timer count interrupt
void runTimerA2(void) {
    TA2CTL = (TASSEL_1 | ID_0 | MC_1); // chooses ACLK as the source clock,
                                       // chooses input divider to be divide by 1,
                                       // chooses up count mode
    TA2CCR0 = MAXCOUNT + 1; // sets interrupt time
    TA2CCTL0 = CCIE; // enables TA2CCRO interrupt
}

// displays the time according to the timer_cnt
void displayTime(long unsigned int timer_cnt) {
    unsigned char dateToDisplay[7]; // will be month[3] + '-' + day[2] + null terminator
    unsigned char timeToDisplay[9]; // will be hour[2] + ':' minute[2] + ':' + second[2] + null terminator
    unsigned long daysPast, hoursPast, minutesPast, secondsPast;

    secondsPast = timer_cnt%60; // calculates seconds passed within 60 seconds
                                // for example if timer_cnt = 61 then secondsPast = 1;
    minutesPast = ((timer_cnt - secondsPast)/60)%60;
    hoursPast = ((timer_cnt - secondsPast - minutesPast * 60)/3600)%24;
    daysPast = ((timer_cnt - secondsPast - minutesPast * 60L - hoursPast * 3600L)/(24L*3600L));

    //determines what month to display
    if (daysPast < 31)              //January
        {
            month[0] = 0x4A;
            month[1] = 0x41;
            month[2] = 0x4E;
        }
        else if (31 <= daysPast && daysPast < 59)   //February
        {
            daysPast -= 31;
            month[0] = 0x46;
            month[1] = 0x45;
            month[2] = 0x42;
        }
        else if (59 <= daysPast && daysPast < 90)   //March
        {
            daysPast -= 59;
            month[0] = 0x4D;
            month[1] = 0x41;
            month[2] = 0x52;
        }
        else if (90 <= daysPast && daysPast < 120)   //April
        {
            daysPast -= 90;
            month[0] = 0x41;
            month[1] = 0x50;
            month[2] = 0x52;
        }
        else if (120 <= daysPast && daysPast < 151)   //May
        {
            daysPast -= 120;
            month[0] = 0x4D;
            month[1] = 0x41;
            month[2] = 0x59;
        }
        else if (151 <= daysPast && daysPast < 181)   //June
        {
            daysPast -= 151;
            month[0] = 0x4A;
            month[1] = 0x55;
            month[2] = 0x4E;
        }
        else if (181 <= daysPast && daysPast < 212)   //July
        {
            daysPast -= 181;
            month[0] = 0x4A;
            month[1] = 0x55;
            month[2] = 0x4C;
        }
        else if (212 <= daysPast && daysPast < 243)   //August
        {
            daysPast -= 212;
            month[0] = 0x41;
            month[1] = 0x55;
            month[2] = 0x47;
        }
        else if (243 <= daysPast && daysPast < 273)   //September
        {
            daysPast -= 243;
            month[0] = 0x53;
            month[1] = 0x45;
            month[2] = 0x50;
        }
        else if (273 <= daysPast && daysPast < 304)   //October
        {
            daysPast -= 273;
            month[0] = 0x4F;
            month[1] = 0x43;
            month[2] = 0x54;
        }
        else if (304 <= daysPast && daysPast < 334)   //November
        {
            daysPast -= 304;
            month[0] = 0x4E;
            month[1] = 0x4F;
            month[2] = 0x56;
        }
        else if (334 <= daysPast && daysPast < 365)   //December
        {
            daysPast -= 334;
            month[0] = 0x44;
            month[1] = 0x45;
            month[2] = 0x43;
        }
    daysPast++; //Need to increment
    //Set the ASCII array for the day, minute, second
    day[0] = 0x30 + (floor)(daysPast/10);
    day[1] = 0x30 + daysPast % 10;
    minute[0] = 0x30 + (floor)(minutesPast/10);
    minute[1] = 0x30 + minutesPast % 10;
    second[0] = 0x30 + (floor)(secondsPast/10);
    second[1] = 0x30 + secondsPast % 10;
    hour[0] = 0x30 + (floor)(hoursPast / 10);
    hour[1] = 0x30 + hoursPast % 10;
    //Combine month and day into dateToDisplay[7]
    //Should be month[3] + '-' + day[2] + null terminator
    dateToDisplay[0] = month[0];
    dateToDisplay[1] = month[1];
    dateToDisplay[2] = month[2];
    dateToDisplay[3] = 0x2D; // '-' character
    dateToDisplay[4] = day[0];
    dateToDisplay[5] = day[1];
    dateToDisplay[6] = 0x00; //null terminator
    //combine hour, minute, and second into timeToDisplay[9]
    //will be hour[2] + ':' minute[2] + ':' + second[2] + null terminator
    timeToDisplay[0] = hour[0];
    timeToDisplay[1] = hour[1];
    timeToDisplay[2] = 0x3A; // ':' character
    timeToDisplay[3] = minute[0];
    timeToDisplay[4] = minute[1];
    timeToDisplay[5] = 0x3A; // ':' character
    timeToDisplay[6] = second[0];
    timeToDisplay[7] = second[1];
    timeToDisplay[8] = 0x00; //null terminator
    //Print these two arrays

    Graphics_drawStringCentered(&g_sContext, dateToDisplay, AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, timeToDisplay, AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

// displays the temperature according to the converted temperature value
void displayTemp(float degC_temperature) {

    //Take in C temperature & Makes a copy of it
    float inAvgTempF = (degC_temperature * 1.8F) + 32; //Convert temperature to C and or F
    unsigned char tempDisplay[17]; // In format: ddd.f for C, ddd.f for F (ex. 25.3C , 74.5F)
    unsigned char celsiusArray[5]; //The temp in celcius, stored as an ASCII array (ddd.f)
    unsigned char fahrenheitArray[5]; //The temp in Fahrenheit, stored as an ASCII array (ddd.f)

    if(degC_temperature < 100)//100 because temp comes in as Celcius
        celsiusArray[0] = 0x20; //will set the hundred's spot as ' ' (an empty space), if not needed
    else
        celsiusArray[0] = (floor)(degC_temperature / 100) + 30; //This is maybe supposed to be +30 or 0x30
        celsiusArray[1] = (floor)(fmod(degC_temperature, 100) / 10) + 0x30;
        celsiusArray[2] = (floor)(fmod(degC_temperature, 10)) + 0x30;
        celsiusArray[3] = 0x2E; //Sets this value to be the decimal point '.'
        celsiusArray[4] = (floor)(fmod((degC_temperature * 10), 10)) + 0x30;

    if (inAvgTempF < 100) {
        if (inAvgTempF < 0) {
            fahrenheitArray[0] = 0x2D; //This will set the initial character to be '-' for Negative temps
            inAvgTempF *= -1; //A sign correction done to ensure the ACSII conversion is correct
        } else
            fahrenheitArray[0] = 0x20; //will set the hundred's spot as ' ' (an empty space), if not needed
    } else
    fahrenheitArray[0] = (floor)(inAvgTempF / 100) + 30;
    fahrenheitArray[1] = (floor)(fmod(inAvgTempF, 100) / 10) + 0x30;
    fahrenheitArray[2] = (floor)(fmod(inAvgTempF, 10)) + 0x30;
    fahrenheitArray[3] = 0x2E; //Sets this value to be the decimal point '.'
    fahrenheitArray[4] = (floor)(fmod((inAvgTempF * 10), 10)) + 0x30;

    //Display Temp C to LCD Screen
    tempDisplay[0] = celsiusArray[0];
    tempDisplay[1] = celsiusArray[1];
    tempDisplay[2] = celsiusArray[2];
    tempDisplay[3] = celsiusArray[3];
    tempDisplay[4] = celsiusArray[4];
    tempDisplay[5] = 0x20; //The ASCII Character for a space ' '
    tempDisplay[6] = 0x43; //The ASCII Character for the letter 'C'
    tempDisplay[7] = 0x20; //The ASCII Character for a space ' '
    tempDisplay[8] = 0x2F; //The ASCII Character for a forward slash '/'
    //Display the temp in F to LCD Screen
    tempDisplay[9]  = fahrenheitArray[0];
    tempDisplay[10] = fahrenheitArray[1];
    tempDisplay[11] = fahrenheitArray[2];
    tempDisplay[12] = fahrenheitArray[3];
    tempDisplay[13] = fahrenheitArray[4];
    tempDisplay[14] = 0x20; //The ASCII Character for a space ' '
    tempDisplay[15] = 0x46; //The ASCII Character for the letter 'F'
    tempDisplay[16] = 0x00; //The ASCII Character for the null terminator

    Graphics_drawStringCentered(&g_sContext, tempDisplay, AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext); //flushes text to the display, updating it
}

// displays a generic message
void displayMessage(char* message) {

    Graphics_clearDisplay(&g_sContext); // clears the display

    // writes message
    Graphics_drawStringCentered(&g_sContext, message, AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);

    // flushes text to the display
    Graphics_flushBuffer(&g_sContext);
}

// inserts temp into temp array
float* insertTemp(float degC_temp, float* ptr_degC_temp_history, int degC_temp_history_len) {
    // if the temperature the pointer is pointing to is equal to the end of array code
    if (*ptr_degC_temp_history == ENDOFARRAY) {
        ptr_degC_temp_history -= (degC_temp_history_len - 1); // then the pointer must be set back to the beginning of the array
        *ptr_degC_temp_history = degC_temp; // overrides the old temperature value with the new temperature value
    } else {
        *ptr_degC_temp_history = degC_temp; // overrides the old temperature value with the new temperature value
    }

    ptr_degC_temp_history++; // increases the pointer to the next memory location in the array

    return ptr_degC_temp_history;
}

// calculates the average temperature according to the current and past 36 values
float calcAvgTemp(float* ptr_degC_temp_history, int num_temp_samples, int degC_temp_history_len) {
    float degC_temp_avg = 0;
    int sample_size;
    int i;

    // if the entire array has not been filled with temperature samples
    if (num_temp_samples < degC_temp_history_len - 1) {
        sample_size = num_temp_samples; // then the sample size is based on the number of samples taken thus far
    } else {
        sample_size = degC_temp_history_len - 1; // otherwise, the sample size is equal to the size of the array
    }

    // adds samples up
    for (i = 0; i < sample_size; i++) {
        degC_temp_avg += *ptr_degC_temp_history;
        ptr_degC_temp_history++;
    }

    // calculates average
    degC_temp_avg = (degC_temp_avg/(sample_size));

    return degC_temp_avg;
}


void stopTimerA2(void) {
    TA2CTL = MC_0;
    TA2CCTL0 &= ~CCIE;
}



void configLaunchPadButtons(void) {//Guess that these are for the Launch-pad

    P1SEL &= ~(BIT1);
    P2SEL &= ~(BIT1);

    P1DIR &= ~(BIT1);
    P2DIR &= ~(BIT1);

    P1REN |= (BIT1);
    P2REN |= (BIT1);

    P1OUT |= (BIT1);
    P2OUT |= (BIT1);
}

unsigned int checkButton(void) {
    unsigned int launch_code;

    if ((P1IN & BIT1) == 0x00) {
        launch_code = RIGHTBUTTON;
    }

    if ((P2IN & BIT1) == 0x00) {
        launch_code = LEFTBUTTON;
    }
}

void checkPotentiometer(unsigned int* timeArray, unsigned int cur_time_unit) {
    unsigned int pot_code = ADC12MEM1;
    long unsigned int ret_seconds;
    unsigned int days_in_month;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int seconds;

    switch(cur_time_unit) {
        case MONTH:
            month = pot_code%12;
            days_in_month = getDaysInMonth(month);
            ret_seconds = days_in_month*24*60*60;
            timeArray[MONTH] = ret_seconds;
            break;
        case DAY:
            day =  pot_code%60;
            ret_seconds = day*60*60;
            timeArray[DAY] = ret_seconds;
            break;
        case HOUR:
            hour = pot_code%60;
            ret_seconds = hour*60;
            timeArray[HOUR] = ret_seconds;
            break;
        case SEC:
            seconds = pot_code%60;
            timeArray[SEC] = seconds;
            break;
    }
}

unsigned int getDaysInMonth(unsigned int month) {
    unsigned int days;
    switch(month) {
        case JAN:
            days = 30;
            break;
        case FEB:
            days = 28;
            break;
        case MAR:
            days = 31;
            break;
        case APR:
            days = 30;
            break;
        case MAY:
            days = 31;
            break;
        case JUN:
            days = 30;
            break;
        case JUL:
            days = 31;
            break;
        case AUG:
            days = 31;
            break;
        case SEP:
            days = 30;
            break;
        case OCT:
            days = 31;
            break;
        case NOV:
            days = 30;
            break;
        case DEC:
            days = 31;
            break;
    }

    return days;
}

unsigned int editTime(unsigned int cur_time_time_state, long unsigned int timer_cnt) {
    unsigned int time_state = cur_time_time_state;
    unsigned int time_change;

    if ((P1IN & BIT1) == 0x00) {
        stopTimerA2();
        switch(time_state) {
            case MONTH:
                displayMessage("Edit Month"); //Should this be Month or Day?
                break;
            case DAY:
                displayMessage("Edit Day");
                break;
            case HOUR:
                displayMessage("Edit Hour");
                break;
            case MINUTE:
                displayMessage("Edit Min");
            case SEC:
                displayMessage("Edit Sec");
                break;
        }

        displayTime(timer_cnt);

        if (time_state == SEC) {
            time_state = MONTH;
        } else {
            time_state++;
        }

    }

    if ((P2IN & BIT1) == 0x00) {
        time_state = END;
        runTimerA2();
    }

    return time_state;
}







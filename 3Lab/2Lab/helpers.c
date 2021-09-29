#include "helpers.h"
#include <msp430.h>
#include <stdlib.h>
#include "peripherals.h"
#include <math.h>

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



void runTimerA2(void) {
    TA2CTL = (TASSEL_1 | ID_0 | MC_1); // chooses ACLK as the source clock,
                                       // chooses input divider to be divide by 1,
                                       // chooses up count mode
    TA2CCR0 = MAXCOUNT + 1; // sets interrupt time
    TA2CCTL0 = CCIE; // enables TA2CCRO interrupt
}



void displayTime(long unsigned int inTime)
{
    unsigned char dateToDisplay[7]; //Will be month[3] + '-' + day[2] + null terminator
    unsigned char timeToDisplay[9]; //Will be hour[2] + ':' minute[2] + ':' + second[2] + null terminator
    unsigned long daysPast, hoursPast, minutesPast, secondsPast;

    secondsPast = inTime % 60;
    minutesPast = ((inTime - secondsPast)/60)%60;
    hoursPast = ((inTime - secondsPast - minutesPast * 60)/3600)%24;
    daysPast = ((inTime - secondsPast - minutesPast * 60L - hoursPast * 3600L))/(24L*3600L);
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
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, dateToDisplay, AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, timeToDisplay, AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
    Graphics_flushBuffer(&g_sContext);
}



char* convertHexToDecimal(char* hex) {

}



void displayTemp(float inAvgTempC) {

    //Take in C temperature & Makes a copy of it
    float outAvgTempC = inAvgTempC;
    //Convert temperature to C and or F


    //Conver Float into list of char*
//    char* tempdisp = outAvgTempC;
    //Displays temperature to the LCD Screen
    Graphics_clearDisplay(&g_sContext); // clears the display
    //writes message
//    Graphics_drawStringCentered(&g_sContext, tempdisp, AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    //flushes text to the display
    Graphics_flushBuffer(&g_sContext);
}



void displayMessage(char* message) {

    Graphics_clearDisplay(&g_sContext); // clears the display

    // writes message
    Graphics_drawStringCentered(&g_sContext, message, AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);

    // flushes text to the display
    Graphics_flushBuffer(&g_sContext);
}





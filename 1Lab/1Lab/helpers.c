#include "helpers.h"
#include "states.h"
#include <msp430.h>
#include <stdlib.h>
/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"

void display_message(char* message) {

    // *** Intro Screen ***
    Graphics_clearDisplay(&g_sContext); // Clear the display

    // Write some text to the display
    Graphics_drawStringCentered(&g_sContext, message, AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);


    // We are now done writing to the display.  However, if we stopped here, we would not
    // see any changes on the actual LCD.  This is because we need to send our changes
    // to the LCD, which then refreshes the display.
    // Since this is a slow operation, it is best to refresh (or "flush") only after
    // we are done drawing everything we need.
    Graphics_flushBuffer(&g_sContext);
}

void initiate_countdown(void) {
    //The Count Down at the start of a new Game
     display_message("3");
     timeDelay(1);
     BuzzerOn();
     timeDelay(1);
     BuzzerOff();

     display_message("2");
     timeDelay(1);
     BuzzerOn();
     timeDelay(1);
     BuzzerOff();

     display_message("1");
     timeDelay(1);
     BuzzerOn();
     timeDelay(1);
     BuzzerOff();

     // *** GAME SHOULD NOW START - Display "Aliens" ***
}

void timeDelay(char numLoops) {
    /* Determines the number of times that the main loop has iterated through,
     *  Returns void, but also a time delay */
    volatile unsigned int i,j;  // volatile to prevent removal in optimization
    for (j=0; j<numLoops; j++)
    {
        i = 50000 ;
        while (i > 0)
           i--;
    }
}

unsigned char randNumGen (int numOfLoops, unsigned char *numInArray) {
    /* Will generate a number from 1 to 5,
     * this can be used to place an alien in the corresponding column */
    /*
    int col;
    for (col = 0, col < numOfLoops, col++) {
        numInArray[col] = (rand(), %(5) + 1) + 0x30;
    }
    numInArray[numOfLoops] = '\0'
    return numInArray[numOfLoops];
    */
}

enum state_codes check_keypad(void) {
    unsigned char currKey = 0;

    currKey = getKey();
    if (currKey == '*') {
        initiate_countdown();
        return pass;
    } else {
        return repeat;
    }

}

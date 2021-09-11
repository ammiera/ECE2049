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

enum ret_codes display_aliens(int y) {
    int i = 0;
    int num_aliens = gen_rand_int();

    for (i = 0; i < num_aliens; i++) {
        // rand = gen_rand_char();
        Graphics_drawStringCentered(&g_sContext, "D", AUTO_STRING_LENGTH, 14 + i*17, y, TRANSPARENT_TEXT);

    }
    Graphics_flushBuffer(&g_sContext);
    timeDelay(1);

    if (y > 90) {
        return fail;
    } else {
        return pass;
    }

}

int gen_rand_int(void) {
    return (rand() %5 + 1);
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
    int col;
    for (col = 0; col < numOfLoops; col++) {
        numInArray[col] = (rand() %5 + 1) + 0x30;
    }
    numInArray[numOfLoops] = '\0';
    return numInArray[numOfLoops];
}

enum state_codes check_keypad_welcome(void) {
    unsigned char currKey = 0;
    currKey = getKey();
    if (currKey == '*') {
        initiate_countdown();
        return pass;
    } else {
        return repeat;
    }
}

enum state_codes check_keypad_game(float counter) {
    unsigned char currKey = 0;
    int i = 0;
    enum ret_codes rc;

    currKey = getKey();
    if (currKey == '#') {
        while(rc != fail)
        {
            Graphics_clearDisplay(&g_sContext); // Clear the display
            rc = display_aliens(i);
            timeDelay(1);
            i++;
        }
    }
    return repeat;
}

void draw_border(void) {
    // *** Intro Screen ***
   // Graphics_clearDisplay(&g_sContext); // Clear the display
    // Creates a one pixel border around the board. (0, 0) is the top left, (95, 95) is the bottom right.
    Graphics_Rectangle box = {.xMin = 0, .xMax = 95, .yMin = 0, .yMax = 95 };
    Graphics_drawRectangle(&g_sContext, &box);
    Graphics_flushBuffer(&g_sContext);
}

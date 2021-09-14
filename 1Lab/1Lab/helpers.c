#include "states.h"
#include "helpers.h"
#include <msp430.h>
#include <stdlib.h>
#include "peripherals.h"

enum ret_codes check_keypad(void) {
    unsigned char currKey = 0;
    currKey = getKey();

    if (currKey == '*') {
            return pass;
    } else {
            return repeat;
    }

}

void display_welcome_screen(void) {

    Graphics_clearDisplay(&g_sContext); /* clears the display */

    /* writes welcome text */
    Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "to", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Space Invaders!", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);

    /* flushes text to the display */
    Graphics_flushBuffer(&g_sContext);

}

enum ret_codes display_aliens(enum speeds speed, unsigned int y) {
    int x = 0;
    int num_aliens = gen_rand_int();

    Graphics_clearDisplay(&g_sContext); // clears the display

    for (x = 0; x < num_aliens; x++) {
        Graphics_drawStringCentered(&g_sContext, "D", AUTO_STRING_LENGTH, (14 + x*17), y, TRANSPARENT_TEXT);

    }

    Graphics_flushBuffer(&g_sContext);
    timeDelay(speed/4);

    if (y > 90) {
        return repeat;
    } else {
        return pass;
    }
}

int gen_rand_int(void) {
    return (rand() %5 + 1);
}

void display_message(char* message) {

    Graphics_clearDisplay(&g_sContext); /* clears the display */

    /* writes message */
    Graphics_drawStringCentered(&g_sContext, message, AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);

    /* flushes text to the display */
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

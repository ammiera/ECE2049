/* derived from https://stackoverflow.com/questions/1371460/state-machines-tutorials/1371654#1371654 */

#include "states.h"
#include "helpers.h"
#include <msp430.h>
#include "peripherals.h"

#define EXIT_SUCCESS 0
#define MAX_LEVELS 5;

enum ret_codes intro_state(void) {
    display_welcome_screen();
    return pass;
}



enum ret_codes waiting_state(void) {
    enum ret_codes rc;
    rc = check_keypad();

    if (rc == pass) {
        initiate_countdown();
    }

    return rc;
}


enum ret_codes game_state(int level) {
    enum ret_codes rc = pass;
    enum speeds speed;
    unsigned int height = 0;
    unsigned int num_aliens = gen_rand_int();
    char rand_char = gen_rand_char();
    char *aliens = malloc(sizeof(char) * (num_aliens + 1));

    gen_rand_aliens(aliens, num_aliens);

    switch(level) {
        case 0:
            speed = slowest;
            break;
        case 1:
            speed = slow;
            break;
        case 2:
            speed = medium;
            break;
        case 3:
            speed = fast;
            break;
        case 4:
            speed = fastest;
            break;
    }

    while(rc != repeat)
    {
        rc = display_aliens(height, num_aliens, aliens);
        height = height + speed;
    }
    display_message("Nachster Schritt");
    timeDelay(5);

    return rc;
}



enum ret_codes exit_state(void) {
    display_message("Game failed :(");
    return fail;
}



void main (void) {

    WDTCTL = WDTPW | WDTHOLD; /* stops watchdog timer. Always need to stop this!! */

    enum state_codes cur_state = intro; /* starts the game */
    enum ret_codes rc; /* declares return codes */
    unsigned int level;

    srand(time(0));

    /* initializes board */
    initLeds();
    configDisplay();
    configKeypad();

    /* checks that the game only runs when an error has not occured */
    while (cur_state != end) {

        switch(cur_state) {
            case intro:
                // display_message("intro");
                rc = intro_state();
                if (rc == pass) {
                    level = 0;
                    cur_state = waiting;
                    break;
                } else {
                    cur_state = end;
                    break;
                }

            case waiting:
                // display_message("waiting");
                rc = waiting_state();
                if (rc == repeat) {
                    cur_state = waiting;
                    break;
                } else if (rc == pass) {
                    cur_state = game;
                    break;
                } else {
                    cur_state = end;
                    break;
                }

            case game:
                // display_message("game");
                rc = game_state(level);
                if (rc == repeat) {
                    if (level < 5)
                    {
                        cur_state = game;
                        level++;
                    } else {
                        cur_state = intro;
                    }
                    break;

                } else {
                    cur_state = end;
                    break;
                }
            }
    }

    return EXIT_SUCCESS;
}
   /*
   //When an "alien" reaches a Y position of 90, we can consider that as game over.
   // So we could do an if (alien.y >= 90) { disp("Failure, loser")}
   Graphics_drawStringCentered(&g_sContext, "Z", AUTO_STRING_LENGTH, 92, 90, TRANSPARENT_TEXT);
   Graphics_drawStringCentered(&g_sContext, "A", AUTO_STRING_LENGTH, 5, 5, TRANSPARENT_TEXT);

   //Use these X values for the position of the columns
   Graphics_drawStringCentered(&g_sContext, "D", AUTO_STRING_LENGTH, 14, 15, TRANSPARENT_TEXT);
   Graphics_drawStringCentered(&g_sContext, "E", AUTO_STRING_LENGTH, 31, 15, TRANSPARENT_TEXT);
   Graphics_drawStringCentered(&g_sContext, "F", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
   Graphics_drawStringCentered(&g_sContext, "G", AUTO_STRING_LENGTH, 65, 15, TRANSPARENT_TEXT);
   Graphics_drawStringCentered(&g_sContext, "H", AUTO_STRING_LENGTH, 82, 15, TRANSPARENT_TEXT);
   //The border of usable space is (

   // Draw a box around everything because it looks nice
   // Creates a one pixel border around the board. (0, 0) is the top left, (95, 95) is the bottom right.
   Graphics_Rectangle box = {.xMin = 0, .xMax = 95, .yMin = 0, .yMax = 95 };
   Graphics_drawRectangle(&g_sContext, &box);
   */

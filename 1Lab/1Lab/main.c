/* derived from https://stackoverflow.com/questions/1371460/state-machines-tutorials/1371654#1371654 */

#include "states.h"

#define EXIT_SUCCESS 0
#define NUM_TRANSITIONS 5

int main (int argc, char *argv[]) {

    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired

    enum state_codes cur_state = entry; /* starts the game */
    enum ret_codes rc; /* declares return codes */
    int (* state_fun)(void); /* declares state function pointer */

    srand(time(0));

    initLeds();
    configDisplay();
    configKeypad();

    display_message("Welcome");
    timeDelay(1);
    display_message("To");
    timeDelay(1);
    display_message("Space Invaders!");

    /* while there is no error in the game */
    while (cur_state != end) {
        switch(cur_state) {
        case game:
            rc = game_state();
            if (rc == repeat) {
                cur_state = game;
                break;
            } else if (rc == pass) {
                cur_state = entry;
                break;
            } else {
                cur_state = end;
                break;
            }

        case entry:
            rc = entry_state();
            if (rc == repeat) {
                cur_state = entry;
                break;
            } else if (rc == pass) {
                cur_state = game;
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

/* welcome screen state */
int entry_state(void) {
    enum ret_codes rc;
    rc = check_keypad_welcome();
    if (rc == pass) {
        initiate_countdown();
    }

    return rc;
}

/* game screen state */
int game_state(void) {
    enum ret_codes rc;
    rc = check_keypad_game();

    //display_message("Help me"); //Place holder for aliens

    return rc;
}

/* fail state */
int exit_state(void) {
    return fail;
}


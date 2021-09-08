/* derived from https://stackoverflow.com/questions/1371460/state-machines-tutorials/1371654#1371654 */

#include "states.h"

#define EXIT_STATE end
#define ENTRY_STATE entry
#define EXIT_SUCCESS 0
#define NUM_TRANSITIONS 5

int (* state[])(void) = {entry_state, game_state, exit_state};

int main (int argc, char *argv[]) {
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired

    enum state_codes cur_state = ENTRY_STATE; /* starts the game */
    enum ret_codes rc; /* declares return codes */
    int (* state_fun)(void); /* declares state function pointer */

    /* while there is no error in the game */
    while (1) {
        state_fun = state[cur_state]; /* gets current state*/
        rc = state_fun(); /* runs current state code */

        /* exits from program if there was a failure in the code */
        if (EXIT_STATE == cur_state)
            break;

        /* updates current state */
        cur_state = lookup_transitions(cur_state, rc);
        printf("var = %i", cur_state);
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

enum state_codes lookup_transitions (enum state_codes cur_state, enum ret_codes rc) {
    /* determines next state based on the current state and its transition */
    int i;
    for ( i = 0; i < NUM_TRANSITIONS; i++) {
        if (state_transitions[i].src_state == cur_state &&
            state_transitions[i].ret_code == rc) {

            return state_transitions[i].dst_state; /* next state was found */
        }
    }
    return end; /* next state was not found */
}

int entry_state(void) {

    unsigned char currKey=0, dispSz = 3;
    unsigned char dispThree[3];

    // Define some local variables
    float a_flt = 190.68;
    int  test = 0x0600, i=0;     // In C prefix 0x means the number that follows is in hex
    long unsigned X= 123456;    // No prefix so number is assumed to be in decimal
    unsigned char myGrade='A';
    unsigned char initial='S';
    //unsigned char your_name[14] = "Your Name Here";
                                    // What happens when you change the array length?
                                    // What should it be? Do you need null terminator /n ?


    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired

    // Some utterly useless instructions -- Step through them
    // What size does the Code Composer MSP430 Compiler use for the
    // following variable types? A float, an int, a long integer and a char?
    a_flt = a_flt*test;
    X = test+X;
    test = test-myGrade;    // A number minus a letter?? What's actually going on here?
                            // What value stored in myGrade (i.e. what's the ASCII code for "A")?
                            // Thus, what is the new value of test? Explain?

    // Useful code starts here
    initLeds();

    configDisplay();
    configKeypad();

    // *** Intro Screen ***
    Graphics_clearDisplay(&g_sContext); // Clear the display

    // Write some text to the display
    Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "to", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Space invaders", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);

    // Draw a box around everything because it looks nice
    Graphics_Rectangle box = {.xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
    Graphics_drawRectangle(&g_sContext, &box);

    // We are now done writing to the display.  However, if we stopped here, we would not
    // see any changes on the actual LCD.  This is because we need to send our changes
    // to the LCD, which then refreshes the display.
    // Since this is a slow operation, it is best to refresh (or "flush") only after
    // we are done drawing everything we need.
    Graphics_flushBuffer(&g_sContext);

    /*
    is_pressed = check_keypad();

    if (is_pressed) {
        play_intro_music();
        return pass;
    } else {
        return repeat;
    }
    */

    return fail;
}

int game_state(void) {
    int exit_state;

    /*
    int is_gameover = 0;

    while (!is_gameover)
    {
        enum game_codes game_state;

        switch (game_state)
        {
            case draw:
                draw_aliens();
                game_state = check
                break;
            case check:
                is_pressed = check_keypad();
                if (is_pressed) {
                    game_state = update;
                    break;
                } else {
                    game_state = draw;
                    break;
                }
            case update:
                update_aliens();
                game_state = draw;
                break;
            case restart:
                is_gameover = 1;
                exit_state = pass;
                break;
            case error:
                is_gameover = 1;
                exit_state = fail;
                break;
        }
    }
    */
    exit_state = fail;
    return exit_state;
}

int exit_state(void) {
    return fail;
}


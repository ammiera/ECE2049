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
    enum columns col;
    unsigned int height = 3;
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

    while(1)
    {
        kill_alien_scum(aliens);
        rc = display_aliens(height, num_aliens, aliens);
        height = height + speed;

        if (rc == repeat) {
            break;
        }

        if (rc == lose) {
            break;
        }
    }

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

                } else if (rc == lose) {
                    cur_state = intro;
                } else {
                    cur_state = end;
                    break;
                }
            }
    }

    return EXIT_SUCCESS;
}

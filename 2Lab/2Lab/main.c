#include "states.h"
#include "helpers.h"
#include <msp430.h>
#include "peripherals.h"

enum ret_codes intro_state(void) {
    displayIntroMessage();

    return pass;
}

enum ret_codes waiting_state(void) {
    enum ret_codes rc;

    rc = check_keypad();

    if (rc == pass) {
        displayCountdown();
    }

    return rc;
}


enum ret_codes game_state(void) {
    unsigned char button_state = 0x00;
    enum ret_codes rc = repeat;
    unsigned int i = 0;
    int song[] = {10, 110, 210, 310, 410, 510, 610, 0};

    while(song[i] != 0 && rc != restart) {
        playNote(song[i]);
        timeDelay(1);
        stopPlayingNote();
        timeDelay(1);
        rc = check_keypad();

        i++;
    }

    button_state = getButtonState();
    displayLeds(button_state);

    return rc;
}

enum ret_codes exit_state(void) {
    displayMessage("Game failed, code better");
    return fail;
}

void main (void) {

    WDTCTL = WDTPW | WDTHOLD; /* stops watchdog timer. Always need to stop this!! */

    enum state_codes cur_state = intro; /* starts the game */
    enum ret_codes rc; /* declares return codes */

    /* initializes board */
    configDisplay();
    configButtons();
    configKeypad();
    initLeds();

    /* checks that the game only runs when an error has not occured */
    while (cur_state != end) {

        switch(cur_state) {
            case intro:
                rc = intro_state();
                if (rc == pass) {
                    cur_state = waiting;
                    break;
                } else {
                    cur_state = end;
                    break;
                }

            case waiting:
                rc = waiting_state();
                if (rc == repeat) {
                    cur_state = waiting;
                    break;
                } else if (rc == pass) {
                    cur_state = game;
                    break;
                } else if (rc == restart) {
                    cur_state = intro;
                    break;
                } else {
                    cur_state = end;
                    break;
                }

            case game:
                rc = game_state();
                if (rc == repeat) {
                    cur_state = game;
                    break;
                } else if (rc == lose) {
                    cur_state = intro;
                    break;
                } else if (rc == restart) {
                    cur_state = intro;
                    break;
                } else {
                    cur_state = end;
                    break;
                }

        }
    }
}

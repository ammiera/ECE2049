#include "states.h"
#include "helpers.h"
#include <msp430.h>
#include "peripherals.h"

// global variables for interrupts
unsigned int timer_cnt = 0; // holds current time with 0.005 accuracy
int song[] = {Cnorm, D, Eflat, Cnorm, D, F, Eflat, Cnorm, D, Eflat, D, Cnorm, 0};
int cur_state = INTRO; /* starts the game */

#pragma vector = TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR(void) {
    if (cur_state == GAME && song[timer_cnt] != 0) {
        playNote(song[timer_cnt]);
        timer_cnt++;
    }
}

int intro_state(void) {
    displayIntroMessage();

    return PASS;
}

int waiting_state(void) {
    int rc;

    rc = check_keypad();

    if (rc == PASS) {
        displayCountdown();
    }

    return rc;
}


int game_state(void) {
    unsigned char button_state = 0x00;
    int rc = REPEAT;
    unsigned int i = 0;

    while(rc != RESTART) {
        rc = check_keypad();
        button_state = getButtonState();
        displayLeds(button_state);
        i++;
    }

    return rc;
}

int exit_state(void) {
    displayMessage("Game failed, code better");
    return FAIL;
}

void main (void) {

    WDTCTL = WDTPW | WDTHOLD; /* stops watchdog timer. Always need to stop this!! */
    _BIS_SR(GIE); // enables global interrupts

    int rc; /* declares return codes */

    /* initializes board */
    setAclk();
    runTimerA2();
    configDisplay();
    configButtons();
    configKeypad();
    initLeds();

    /* checks that the game only runs when an error has not occured */
    while (cur_state != END) {

        switch(cur_state) {
            case INTRO:
                rc = intro_state();
                if (rc == PASS) {
                    cur_state = WAITING;
                    break;
                } else {
                    cur_state = END;
                    break;
                }

            case WAITING:
                rc = waiting_state();
                if (rc == REPEAT) {
                    cur_state = WAITING;
                    break;
                } else if (rc == PASS) {
                    cur_state = GAME;
                    break;
                } else if (rc == RESTART) {
                    cur_state = INTRO;
                    break;
                } else {
                    cur_state = END;
                    break;
                }

            case GAME:
                rc = game_state();
                if (rc == REPEAT) {
                    cur_state = GAME;
                    break;
                } else if (rc == LOSE) {
                    cur_state = INTRO;
                    break;
                } else if (rc == RESTART) {
                    cur_state = INTRO;
                    break;
                } else {
                    cur_state = END;
                    break;
                }

        }
    }
}

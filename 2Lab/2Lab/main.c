#include "states.h"
#include "helpers.h"
#include <msp430.h>
#include "peripherals.h"

// global variables for interrupts
int cur_note = -1;;
unsigned int timer_cnt; // holds current time with 0.005 accuracy
struct Note {
    int note;
    int tics;
};
struct Note song[] = {{E, 1}, {E, 1}, {E, 1}, {Cnorm, 2},
{E, 1}, {G, 5}, {G, 5}, {0,0}};

#pragma vector = TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR(void) {
    if (cur_note != -1) {
        if (timer_cnt < song[cur_note].tics) {
            timer_cnt++;
        } else {
            timer_cnt = 0;
            cur_note++;
        }

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
    unsigned int rc = REPEAT;
    unsigned char button_state = 0x00;
    cur_note = 0;

    while(rc != RESTART) {
        playNote(song[cur_note].note);
        button_state = getButtonState();
        rc = check_keypad();
        displayLeds(button_state);

        if(song[cur_note].note == 0) {
            rc = RESTART;
            turnOffLeds();
            stopPlayingNote();
        }

        if (timer_cnt == 0) {
            stopPlayingNote();
        }
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

    unsigned int rc; /* declares return codes */
    unsigned int cur_state = INTRO; /* starts the game */

    /* initializes board */
    setAclk();
    runTimerA2();
    configDisplay();
    configButtons();
    configKeypad();
    configNote();
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

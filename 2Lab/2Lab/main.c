#include "states.h"
#include "helpers.h"
#include <msp430.h>
#include "peripherals.h"

// global song variables
unsigned song_len = 58; // to do, figure this out automatically in main function with for loop
/*
struct Note song[] = {{E, 1000}, {E, 1000}, {E, 1000},
                      {Cnorm, 2000}, {E, 2000}, {G, 2000}, {G, 2000},
                      {Cnorm, 2000}, {G, 2000}, {E, 2000},
                      {A, 2000}, {Bflat, 2000}, {B, 2000},
                      {Cnorm, 2000}, {E, 2000}, {G, 2000}, {A, 2000},
                      {F, 2000}, {G, 2000}, {E, 2000}, {Cnorm, 2000}, {D, 2000}, {B, 2000},
                      {Cnorm, 2000}, {G, 2000}, {E, 2000},
                      {A, 2000}, {Bflat, 2000}, {B, 2000},
                      {Cnorm, 2000}, {E, 2000}, {G, 2000}, {A, 2000},
                      {F, 2000}, {G, 2000}, {E, 2000}, {Cnorm, 2000}, {D, 2000}, {B, 2000},
                      {G, 2000}, {Fsharp, 2000}, {F, 2000}, {D, 2000}, {E, 2000},
                      {G, 2000}, {A, 2000}, {Cnorm, 2000},
                      {A, 2000}, {Cnorm, 2000}, {D, 2000},
                      {G, 2000}, {Fsharp, 2000}, {F, 2000}, {D, 2000}, {E, 2000},
                      {Cnorm, 2000}, {Cnorm, 2000}, {Cnorm, 2000}};
*/
struct Note song[] = {{A, 2000}, {F, 2000}, {G, 2000}, {G, 2000}, {A, 2000},
                      {D, 2000}, {F, 2000}, {F, 2000}, {C, 2000}, {A, 2000},
                      {D, 2000}, {A, 2000}, {F, 2000}, {G, 2000}, {A, 2000},
                      {G, 2000}, {F, 2000}, {G, 2000}, {A, 2000}, {F, 2000}};

// global variables for interrupts
int cur_note;
unsigned int timer_cnt; // holds current time with 0.005 accuracy

#pragma vector = TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR(void) {
    if (cur_note >= 0 && cur_note < song_len) {
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

        if(cur_note == song_len) {
            rc = RESTART;
        }
    }

    turnOffLeds();
    stopPlayingNote();

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
    timer_cnt = 0;
    cur_note = -1;

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

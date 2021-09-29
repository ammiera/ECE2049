#include "states.h"
#include "helpers.h"
#include <msp430.h>
#include "peripherals.h"
#include <stdio.h>

// global variables
int song[] = {A, Cnorm, Cnorm, Cnorm, D, E, E, D, E, F, Aflat, Cnorm, Cnorm, Cnorm, Aflat, Aflat, Aflat, E, E, E, Cnorm, Cnorm, Cnorm, Aflat, F, E, D, Cnorm, 0};

unsigned int song_len = 28; // to do, figure this out automatically in main function with for loop
unsigned int timer_cnt; // holds current time with 0.01 accuracy
unsigned int cur_state; /* starts the game */

#pragma vector = TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR(void) {
    if (cur_state == GAME) {
        timer_cnt++;
    }
}

void setLEDState(int cur_note) {
    //checks if any of the 12 notes are played, based on which note was played,
    //it will then light up the LED that has been assigned to that note

    // Turn all LEDs off to start
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);

    switch(song[cur_note]) {
    case A: case Bflat: case B:
        P6OUT |= R1REDLED;
        break;
    case Cnorm: case Csharp: case D:
        P6OUT |= R2YELLOWLED;
        break;
    case Eflat: case E: case F:
        P6OUT |= R3BLUELED;
        break;
    case Fsharp: case G: case Aflat:
        P6OUT |= R4GREENLED;
    default:
        P6OUT &= ~(BIT4|BIT3|BIT2|BIT1); //Turns LEDS off by default
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
    int cur_score = song_len + 1;
    unsigned char button_state = 0x00;
    int cur_note = 0;

    while(1) {
        if (song[cur_note] == 0) {
            rc = WIN;
            break;
        } else {
            button_state = getButtonState();

            if(timer_cnt - cur_note*6000 < 6000) {
                playNote(song[cur_note]);
            } else {
                cur_note++;
                stopPlayingNote();
                timeDelay(0.5);
                cur_score = trackScore(button_state, cur_score);
            }

            rc = check_keypad();
            if (rc == RESTART) {
                break;
            }

            setLEDState(cur_note);
            if (cur_score <= song_len/2) {
                rc = LOSE;
                break;
            }
        }
    }

    turnOffLeds();
    stopPlayingNote();

    if (rc == WIN) {
            displayMessage("Winner Winner!");
            timeDelay(3);
            rc = RESTART;
    }

    if (rc == LOSE) {
        displayLosingMessages(); // when player lost, this plays
        timeDelay(3);
    }

    timer_cnt = 0;

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
    cur_state = INTRO; /* starts the game */
    timer_cnt = 0;

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

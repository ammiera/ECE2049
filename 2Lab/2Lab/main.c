#include "states.h"
#include "helpers.h"
#include <msp430.h>
#include "peripherals.h"

// global song variables
unsigned int song_len = 58; // to do, figure this out automatically in main function with for loop
int cur_score;

struct Note song[] = {{E, 6000}, {E, 6000}, {E, 6000},
                      {Cnorm, 6000}, {E, 6000}, {G, 6000}, {G, 6000},
                      {Cnorm, 6000}, {G, 6000}, {E, 6000},
                      {A, 6000}, {Bflat, 6000}, {B, 6000},
                      {Cnorm, 6000}, {E, 6000}, {G, 6000}, {A, 6000},
                      {F, 6000}, {G, 6000}, {E, 6000}, {Cnorm, 6000}, {D, 6000}, {B, 6000},
                      {Cnorm, 6000}, {G, 6000}, {E, 6000},
                      {A, 6000}, {Bflat, 6000}, {B, 6000},
                      {Cnorm, 6000}, {E, 6000}, {G, 6000}, {A, 6000},
                      {F, 6000}, {G, 6000}, {E, 6000}, {Cnorm, 6000}, {D, 6000}, {B, 6000},
                      {G, 6000}, {Fsharp, 6000}, {F, 6000}, {D, 6000}, {E, 6000},
                      {G, 6000}, {A, 6000}, {Cnorm, 6000},
                      {A, 6000}, {Cnorm, 6000}, {D, 6000},
                      {G, 6000}, {Fsharp, 6000}, {F, 6000}, {D, 6000}, {E, 6000},
                      {Cnorm, 6000}, {Cnorm, 6000}, {Cnorm, 6000}};

// global variables for interrupts
int cur_note;
unsigned int timer_cnt; // holds current time with 0.01 accuracy

#pragma vector = TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR(void) {
    if (cur_note >= 0 && cur_note < song_len) {
        if (timer_cnt < song[cur_note].tics) {
            timer_cnt++;
        } else {
            timer_cnt = 0;
            cur_note++;
            cur_score--;
        }
    }
}

void setLEDState(void) {
    //checks if any of the 12 notes are played, based on which note was played,
    //it will then light up the LED that has been assigned to that note

    // Turn all LEDs off to start
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);

    switch(song[cur_note].note) {
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

int trackScore(unsigned char button_state, int cur_score) {
    if ((button_state == S1PRSSD) && (P6OUT & R1REDLED == 0x04)) {
        cur_score++;
    } else if ((button_state == S2PRSSD) && (P6OUT & R2YELLOWLED == 0x02)) {
        cur_score++;
    } else if ((button_state == S3PRSSD) && (P6OUT & R3BLUELED == 0x08)) {
        cur_score++;
    } else if ((button_state == S4PRSSD) && (P6OUT & R4GREENLED == 0x10)) {
        cur_score++;
    } else {
        cur_score = cur_score;
    }

    return cur_score;
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
    cur_score = song_len;
    cur_note = 0;

    while(rc != RESTART && rc != LOSE) {
        playNote(song[cur_note].note);
        button_state = getButtonState();
        rc = check_keypad();
        setLEDState();
        cur_score = trackScore(button_state, cur_score);

        if (cur_note == song_len) {
            rc = RESTART;
        }

        if (cur_score < song_len/2) {
            rc = LOSE;
        }
    }

    turnOffLeds();
    stopPlayingNote();

    if (rc == LOSE) {
        displayLosingMessages(); // when player lost, this plays
        timeDelay(3);
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

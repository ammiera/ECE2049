#include "states.h"
#include "helpers.h"
#include <msp430.h>
#include <stdlib.h>
#include "peripherals.h"
#include "math.h"

/* gets the hex value that corresponds to each user button */
unsigned char getButtonState(void) {

    unsigned char ret_val = 0x00;

    // checks if S1 has been pressed
    if ((P7IN & BIT0) == 0x00) {
        ret_val += 0x01;
    }
    // checks if S2 has been pressed
    if ((P3IN & BIT6) == 0x00) {
        ret_val += 0x02;
    }
    // checks if S3 has been pressed
    if ((P2IN & BIT2) == 0x00) {
        ret_val += 0x04;
    }
    // checks if S4 has been pressed
    if ((P7IN & BIT4) == 0x00) {
        ret_val += 0x08;
    }

    return ret_val;
}

/* configures buttons for input */
void configButtons(void) {
    /* configures buttons for IO */
    P7SEL &= ~(BIT4|BIT0);
    P3SEL &= ~(BIT6); // configures button for IO
    P2SEL &= ~(BIT2);

    /* configures buttons for input */
    P7DIR &= ~(BIT4|BIT0);
    P3DIR &= ~(BIT6);
    P2DIR &= ~(BIT2);

    /* configures buttons for pull resistors */
    P7REN |= (BIT4|BIT0);
    P3REN |= (BIT6);
    P2REN |= (BIT2);

    /* configures buttons for pull down resistors */
    P7OUT |= (BIT4|BIT0);
    P3OUT |= (BIT6);
    P2OUT |= (BIT2);
}


void displayLeds(char button_state) {
    //takes the output of the button state and configures corresponding LEDs to light up

    // Turn all LEDs off to start
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);

    switch (button_state) {
        case (S1PRSSD):
            P6OUT |= R1REDLED;
            break;
        case (S2PRSSD):
            P6OUT |= R2YELLOWLED;
            break;
        case (S3PRSSD):
            P6OUT |= R3BLUELED;
            break;
        case (S4PRSSD):
            P6OUT |= R4GREENLED;
            break;
        case (S1PRSSD | S2PRSSD):
            P6OUT |= (R1REDLED | R2YELLOWLED);
            break;
        case (S1PRSSD | S3PRSSD):
            P6OUT |= (R1REDLED | R3BLUELED);
            break;
        case (S1PRSSD | S4PRSSD):
            P6OUT |= (R1REDLED | R4GREENLED);
            break;
        case (S2PRSSD | S3PRSSD):
            P6OUT |= (R2YELLOWLED | R3BLUELED);
            break;
        case (S2PRSSD | S4PRSSD):
            P6OUT |= (R2YELLOWLED | R4GREENLED);
            break;
        case (S3PRSSD | S4PRSSD):
            P6OUT |= (R3BLUELED | R4GREENLED);
            break;
        default:
            P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);
    }
}

void turnOffLeds(void) {
    // Turn all LEDs off to start
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);
}

void displayMessage(char* message) {

    Graphics_clearDisplay(&g_sContext); // clears the display

    // writes message
    Graphics_drawStringCentered(&g_sContext, message, AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);

    // flushes text to the display
    Graphics_flushBuffer(&g_sContext);
}

void displayIntroMessage(void) {
    Graphics_clearDisplay(&g_sContext); // clears the display

    // writes welcome message
    Graphics_drawStringCentered(&g_sContext, "Welcome to", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Guitar Hero!", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);

    //writes the message to press the '#' to start
    Graphics_drawStringCentered(&g_sContext, "Press * to play", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);

    // flushes text to the display
    Graphics_flushBuffer(&g_sContext);
}

void displayCountdown(void) {
    // Turn all LEDs off to start
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);

    displayMessage("3");
    P6OUT |= (R1REDLED | R2YELLOWLED | R3BLUELED); // light up left 3 LEDs
    playNote(E);
    timeDelay(2);// timer delay
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1); // LEDs OFF
    stopPlayingNote();

    displayMessage("2");
    P6OUT |= (R1REDLED | R2YELLOWLED); // light up left 2 LEDs
    playNote(E);
    timeDelay(2);//timer delay
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1);// LEDs OFF
    stopPlayingNote();

    displayMessage("1");
    P6OUT |= (R1REDLED); // light up left most LED
    playNote(E);
    timeDelay(2); // timer delay
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1); // LED OFF
    stopPlayingNote();

    displayMessage("GO!");
    P6OUT |= (R1REDLED | R2YELLOWLED | R3BLUELED | R4GREENLED); // light up left all LEDs
    playNote(A);
    timeDelay(2); // timer delay
    P6OUT &= ~(BIT4|BIT3|BIT2|BIT1); // LEDs OFF
    stopPlayingNote();
    timeDelay(2); // timer delay
}

int check_keypad(void) {
    unsigned char currKey = 0;
    currKey = getKey();

    if (currKey == '*') {
        return PASS;
    }

    if (currKey == '#') {
        return RESTART;
    }

    return REPEAT;
}

void configNote(void) {
    // Initialize PWM output on P3.5, which corresponds to TB0.5
    P3SEL |= BIT5; // Select peripheral output mode for P3.5
    P3DIR |= BIT5;

    TB0CTL  = (TBSSEL__ACLK|ID__1|MC__UP);  // Configure Timer B0 to use ACLK, divide by 1, up mode
    TB0CTL  &= ~TBIE;                       // Explicitly Disable timer interrupts for safety

    // Now configure the timer period, which controls the PWM period
    // Doing this with a hard coded values is NOT the best method
    // We do it here only as an example. You will fix this in Lab 2.
    TB0CCTL0 &= ~CCIE;                  // Disable timer interrupts
}

void playNote(unsigned int frequency) {
    int period = (frequency/ACLCKFQ);

    TB0CCR0 = period; // Set the PWM period in ACLK ticks

    // Configure CC register 5, which is connected to our PWM pin TB0.5
    TB0CCTL5  = OUTMOD_7;                   // Set/reset mode for PWM
    TB0CCTL5 &= ~CCIE;                      // Disable capture/compare interrupts
    TB0CCR5   = TB0CCR0/2;                  // Configure a 50% duty cycle
}

void stopPlayingNote(void)
{
    // Disable both capture/compare periods
    TB0CCTL0 = 0;
    TB0CCTL5 = 0;
}

void timeDelay(unsigned int numLoops) {
    /* Determines the number of times that the main loop has iterated through,
     *  Returns void, but also a time delay */
    volatile unsigned int i,j;  // volatile to prevent removal in optimization
    for (j=0; j<numLoops; j++)
    {
        i = 50000 ;
        while (i > 0)
           i--;
    }
}

void setAclk(void) {
    P5SEL |= (BIT5 | BIT4 | BIT3 | BIT2 | BIT1);

    // defaults
    UCSCTL0 = 0x14B8;
    UCSCTL1 = 0x0020;
    UCSCTL2 = 0x101F;
    UCSCTL3 = 0x0000;
    UCSCTL4 = 0x0044;
    UCSCTL5 = 0x0000;
    UCSCTL6 = 0xC1CD;
    UCSCTL7 = 0x0403;
    UCSCTL8 = 0x0707;
}

/* leap counting is NOT needed because accuracy is not a concern*/
void runTimerA2(void) {
    TA2CTL = (TASSEL_2 | ID_0 | MC_1); // chooses SMCLK as the source clock,
                                       // chooses input divider to be divide by 1,
                                       // chooses up count mode
    TA2CCR0 = MAXCOUNT; // sets interrupt time
    TA2CCTL0 = CCIE; // enables TA2CCRO interrupt
}


void displayLosingMessages(void) {
    // clears the display
    Graphics_clearDisplay(&g_sContext);
    // writes welcome message
    Graphics_drawStringCentered(&g_sContext, "BOO, You Suck!", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Music is not", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "your calling!", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
    // flushes text to the display
    Graphics_flushBuffer(&g_sContext);
}

int trackScore(char button_state, int cur_score) {
    if ((button_state == S1PRSSD) && ((P6OUT & R1REDLED) == 0x04)) {
        cur_score++;
    } else if ((button_state == S2PRSSD) && ((P6OUT & R2YELLOWLED) == 0x02)) {
        cur_score++;
    } else if ((button_state == S3PRSSD) && ((P6OUT & R3BLUELED) == 0x08)) {
        cur_score++;
    } else if ((button_state == S4PRSSD) && ((P6OUT & R4GREENLED) == 0x10)) {
        cur_score++;
    } else {
        cur_score--;
        displayMessage("Minus 1 point");
    }

    return cur_score;
}









/************** ECE2049 DEMO CODE ******************/
/**************  25 August 2021   ******************/
/***************************************************/

#include <msp430.h>
#include "peripherals.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>

void main(void)

{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired

    unsigned char button_state = 0x00;

    configDisplay();
    config_buttons();

    //config_userLED();

    Graphics_clearDisplay(&g_sContext); // clear the display
    Graphics_drawStringCentered(&g_sContext, "Lab 2", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT); // write to display
    Graphics_flushBuffer(&g_sContext); // flush

    while (1)
    {
        button_state = get_button_state();
    }
}

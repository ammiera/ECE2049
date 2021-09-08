#include <helper.h>

void display_text(char* text) {
    configDisplay();

    /* Intro Screen */
    Graphics_clearDisplay(&g_sContext); // clears the display

    // Write some text to the display
    Graphics_drawStringCentered(&g_sContext, "Space Invaders", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
}

int check_keypad(void) {

}

void play_music(enum music_types music) {

}
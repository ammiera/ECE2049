#ifndef HELPER_H
#define HELPER_H

#include <peripherals.h>
#include <stdio.h>
#include <msp430.h>

enum music_types { intro, win, lose}; /* key words for return codes */

void display_text(char* text);
int check_keypad(void);
void play_music(enum music_types music);

#endif

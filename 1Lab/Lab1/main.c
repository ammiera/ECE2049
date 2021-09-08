/* derived from https://stackoverflow.com/questions/1371460/state-machines-tutorials/1371654#1371654 */

#include <states.h>
#include <stdio.h>

int entry_state(void) {
    display_text("Welcome to the game");

    /*
    is_pressed = check_keypad();

    if (is_pressed) {
        play_intro_music();
        return pass;
    } else {
        return repeat;
    }
    */

    return fail;
}

int game_state(void) {
    int exit_state;

    /*
    int is_gameover = 0;

    while (!is_gameover)
    {
        enum game_codes game_state;

        switch (game_state)
        {
            case draw:
                draw_aliens();
                game_state = check
                break;
            case check:
                is_pressed = check_keypad();
                if (is_pressed) {
                    game_state = update;
                    break;
                } else {
                    game_state = draw;
                    break;
                }
            case update:
                update_aliens();
                game_state = draw;
                break;
            case restart:
                is_gameover = 1;
                exit_state = pass;
                break;
            case error:
                is_gameover = 1;
                exit_state = fail;
                break;
        }
    }
    */
    exit_state = fail;
    return exit_state;
}

int exit_state(void) {
    display_text("Space invaders failed, come back again later");
}

int (* state[])(void) = {entry_state, game_state, exit_state};

enum state_codes lookup_transitions (enum state_codes cur_state, enum ret_codes rc) {
    /* determines next state based on the current state and its transition */
    int i;
    for ( i = 0; i < NUM_TRANSITIONS; i++) {
        if (state_transitions[i].src_state == cur_state &&
            state_transitions[i].ret_code == rc) {

            return state_transitions[i].dst_state; /* next state was found */
        }
    }
    return end; /* next state was not found */
}

int main (int argc, char *argv[]) {
    enum state_codes cur_state = ENTRY_STATE; /* starts the game */
    enum ret_codes rc; /* declares return codes */
    int (* state_fun)(void); /* declares state function pointer */

    /* while there is no error in the game */
    while (1) {
        state_fun = state[cur_state]; /* gets current state*/
        rc = state_fun(); /* runs current state code */
        
        /* exits from program if there was a failure in the code */
        if (EXIT_STATE == cur_state) 
            break;

        /* updates current state */
        cur_state = lookup_transitions(cur_state, rc);
    }

    return EXIT_SUCCESS;
}
   /*
   //When an "alien" reaches a Y position of 90, we can consider that as game over.
   // So we could do an if (alien.y >= 90) { disp("Failure, loser")}
   Graphics_drawStringCentered(&g_sContext, "Z", AUTO_STRING_LENGTH, 92, 90, TRANSPARENT_TEXT);
   Graphics_drawStringCentered(&g_sContext, "A", AUTO_STRING_LENGTH, 5, 5, TRANSPARENT_TEXT);

   //Use these X values for the position of the columns
   Graphics_drawStringCentered(&g_sContext, "D", AUTO_STRING_LENGTH, 14, 15, TRANSPARENT_TEXT);
   Graphics_drawStringCentered(&g_sContext, "E", AUTO_STRING_LENGTH, 31, 15, TRANSPARENT_TEXT);
   Graphics_drawStringCentered(&g_sContext, "F", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
   Graphics_drawStringCentered(&g_sContext, "G", AUTO_STRING_LENGTH, 65, 15, TRANSPARENT_TEXT);
   Graphics_drawStringCentered(&g_sContext, "H", AUTO_STRING_LENGTH, 82, 15, TRANSPARENT_TEXT);
   //The border of usable space is (

   // Draw a box around everything because it looks nice
   // Creates a one pixel border around the board. (0, 0) is the top left, (95, 95) is the bottom right.
   Graphics_Rectangle box = {.xMin = 0, .xMax = 95, .yMin = 0, .yMax = 95 };
   Graphics_drawRectangle(&g_sContext, &box);
   */


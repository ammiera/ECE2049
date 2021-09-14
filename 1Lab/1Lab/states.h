#ifndef STATES_H
#define STATES_H

enum state_codes { intro, waiting, game, end}; /* key words for states */
enum ret_codes { pass, fail, repeat }; /* key words for return codes */
enum speeds {slowest = 1, slow = 2, medium = 4, fast = 8, fastest = 16}; /* speeds at which aliens fall from the screen */

enum ret_codes entry_state(void);
enum ret_codes waiting_state(void);
enum ret_codes game_state(int level);
enum ret_codes exit_state(void);

#endif

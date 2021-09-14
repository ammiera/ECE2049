#ifndef STATES_H
#define STATES_H

enum state_codes { intro, waiting, game, end}; /* key words for states */
enum ret_codes { pass, fail, repeat }; /* key words for return codes */
enum speeds {slowest = 4, slow = 3, medium = 2, fast = 2, fastest = 1}; /* speeds at which aliens fall from the screen */

enum ret_codes entry_state(void);
enum ret_codes waiting_state(void);
enum ret_codes game_state(int level);
enum ret_codes exit_state(void);

#endif

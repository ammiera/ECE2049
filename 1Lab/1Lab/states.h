#ifndef STATES_H
#define STATES_H

enum state_codes { intro, waiting, game, end}; /* key words for states */
enum ret_codes { pass, fail, repeat, lose}; /* key words for return codes */
enum speeds {slowest = 5, slow = 9, medium = 13, fast = 16, fastest = 20}; /* speeds at which aliens fall from the screen */
enum columns {col1 = 1, col2 = 2, col3 = 3, col4 = 4, col5 = 5}; /* specifies the column based on the keypad input */

enum ret_codes entry_state(void);
enum ret_codes waiting_state(void);
enum ret_codes game_state(int level);
enum ret_codes exit_state(void);

#endif

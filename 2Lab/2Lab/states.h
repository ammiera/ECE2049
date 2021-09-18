#ifndef STATES_H_
#define STATES_H_

enum state_codes { intro, waiting, game, end}; /* key words for states */
enum ret_codes { pass, fail, repeat, lose}; /* key words for return codes */

enum ret_codes entry_state(void);
enum ret_codes waiting_state(void);
enum ret_codes game_state(void);
enum ret_codes exit_state(void);

#endif /* STATES_H_ */

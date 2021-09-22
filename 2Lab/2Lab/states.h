#ifndef STATES_H_
#define STATES_H_

#define INTRO 0
#define WAITING 1
#define GAME 2
#define END 3

#define PASS 0
#define FAIL 1
#define REPEAT 2
#define RESTART 3
#define LOSE 4

int entry_state(void);
int waiting_state(void);
int game_state(void);
int exit_state(void);

#endif /* STATES_H_ */

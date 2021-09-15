#ifndef HELPERS_H_
#define HELPERS_H_

enum ret_codes check_keypad(void);
void display_welcome_screen(void);
enum ret_codes display_aliens(unsigned int y, unsigned int num_aliens, char* aliens);
unsigned int gen_rand_int(void);
void display_message(char* message);
void initiate_countdown(void);
void timeDelay(char numLoops);
char gen_rand_char(void);
void gen_rand_aliens(char* aliens, unsigned int num_aliens);
void kill_alien_scum(char* aliens);
unsigned int gen_rad_sound(void);


#endif /* HELPERS_H_ */

#ifndef HELPERS_H_
#define HELPERS_H_

void display_message(char* message);
void initiate_countdown(void);
void timeDelay(char numLoops);
unsigned char randNumGen (int numOfLoops, unsigned char *numInArray);
enum state_codes check_keypad(void);
void draw_border(void);
enum ret_codes display_aliens(int y);

#endif /* HELPERS_H_ */

#ifndef HELPERS_H_
#define HELPERS_H_

unsigned char getButtonState(void);
void configButtons(void);
void displayLeds(char button_state);
void display_message(char* message);
enum ret_codes check_keypad(void);

#endif /* HELPERS_H_ */

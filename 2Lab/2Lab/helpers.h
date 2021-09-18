#ifndef HELPERS_H_
#define HELPERS_H_

void configButtons(void);
unsigned char getButtonState(void);

void displayLeds(char button_state);

void displayMessage(char* message);
void displayIntroMessage(void);
void displayCountdown(void);

enum ret_codes check_keypad(void);

#endif /* HELPERS_H_ */

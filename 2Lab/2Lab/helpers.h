#ifndef HELPERS_H_
#define HELPERS_H_

// defines the pitches
#define A 440
#define Bflat 466
#define B 494
#define Cnorm 523
#define Csharp 554
#define D 587
#define Eflat 622
#define E 659
#define F 698
#define Fsharp 740
#define G 784
#define Aflat 831

// defines the hex values that correspond to the user buttons
#define S1PRSSD 0x01
#define S2PRSSD 0x02
#define S3PRSSD 0x04
#define S4PRSSD 0x08

// defines the LEDs
#define R1REDLED BIT2
#define R2YELLOWLED BIT1
#define R3BLUELED BIT3
#define R4GREENLED BIT4

// defines ACLCK frequency
#define ACLCKFQ 32.768

// defines the number of tics that have occured when 0.005 seconds have passed
#define MAXCOUNT 163

void configButtons(void);
unsigned char getButtonState(void);

void displayLeds(char button_state);

void displayMessage(char* message);
void displayIntroMessage(void);
void displayCountdown(void);

int check_keypad(void);

void configNote(void);
void playNote(unsigned int frequency);
void stopPlayingNote(void);

void timeDelay(unsigned int numLoops);

#endif /* HELPERS_H_ */

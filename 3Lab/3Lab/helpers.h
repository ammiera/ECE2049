#ifndef HELPERS_H_
#define HELPERS_H_

#define DayPerMonth 30 // the number of days in a month //Exact: 29.53059
#define HourPerDay 24 // the number of hours in a day
#define MinPerHour 60 // the num of minutes in an hour
#define SecPerMin 60 // the num of seconds in a minute
#define TicksPerSec 1 //the number of ticks per second
#define MAXCOUNT 32767 // number of clock ticks in 1 second
#define DAYSTOBIRTHDAY 320 // the number of days until Nov 17 (Alex's Birthday) starting at Jan 1
#define TEMPHISTORYLEN 36 + 1 // there will be 36 temp history values saved and 1 value that will represent the end of array
#define ENDOFARRAY 404 // coldest temperature the temperature sensor can measure

unsigned char month[3];
unsigned char day[2];
unsigned char hour[2];
unsigned char minute[2];
unsigned char second[2];


//Defining these for when using the scroll wheel
#define MONTH 0
#define DAY 1
#define HOUR 2
#define MINUTE 3
#define SEC 4
#define END 5

#define JAN 1
#define FEB 2
#define MAR 3
#define APR 4
#define MAY 5
#define JUN 6
#define JUL 7
#define AUG 8
#define SEP 9
#define OCT 10
#define NOV 11
#define DEC 12

#define LEFTBUTTON 0x01
#define RIGHTBUTTON 0x02

// ADC
void enableADC12(void);
void startADC(void);

// potentiometer
void configPotentiometer(void);
void checkPotentiometer(unsigned int* timeArray, unsigned int cur_time_unit);

// temp
long unsigned int sampleTemp(void);
void displayTemp(float inAvgTempC);
float* insertTemp(float degC_temp, float* ptr_degC_temp_history, int degC_temp_history_len);
float calcAvgTemp(float* ptr_degC_temp_history, int num_temp_data, int degC_temp_history_len);

// time
void setA2(void);
void runTimerA2(void);
void displayTime(long unsigned int timer_cnt);
void stopTimerA2(void);
void updateTime(unsigned int edit_state, unsigned int unitOfTime);
unsigned int editTime(unsigned int cur_time_time_state, long unsigned int timer_cnt);
unsigned int getDaysInMonth(unsigned int month);

// general
void displayMessage(char* message);
void configLaunchPadButtons(void);
unsigned int checkButton(void);

#endif /* HELPERS_H_ */

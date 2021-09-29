#ifndef HELPERS_H_
#define HELPERS_H_

#define DayPerMonth 30 //The number of days in a month //Exact: 29.53059
#define HourPerDay 24 //the number of hours in a day
#define MinPerHour 60 //the num of minutes in an hour
#define SecPerMin 60 //the num of seconds in a minute
#define TicksPerSec 1 //the number of ticks per second
#define MAXCOUNT 32768

unsigned char month[3];
unsigned char day[2];
unsigned char hour[2];
unsigned char minute[2];
unsigned char second[2];

void setA2(void);
void runTimerA2(void);
void displayTime(long unsigned int inTime);
void displayTemp(float inAvgTempC);
void displayMessage(char* message);

#endif /* HELPERS_H_ */

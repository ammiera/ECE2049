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

void enableADC12(void);
long unsigned int sampleTemp(void);
void setA2(void);
void runTimerA2(void);
void displayTime(long unsigned int inTime);
void displayMessage(char* message);
void displayTemp(float inAvgTempC);
float* insertTemp(float degC_temp, float* ptr_degC_temp_history, int degC_temp_history_len);
float calcAvgTemp(float* ptr_degC_temp_history, int num_temp_data, int degC_temp_history_len);

#endif /* HELPERS_H_ */

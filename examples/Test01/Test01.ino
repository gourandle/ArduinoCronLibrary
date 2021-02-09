#include <MemoryFree.h>
// Do not remove the include below

#include "RTClib.h"
#include "TimedCommand.h"
#include "Cron.h"


#define SIZETC 28
#define Serial SerialUSB

RTC_DS1307 DS_RTC;

void execute(bool);

TimedCommand tCommands[SIZETC];
// create a cron object and pass it the array of timed commands
// and the count of timed commands
Cron cron(tCommands,SIZETC );

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Starting ArduinoCronLibrary Example");

  DS_RTC.begin();

  Serial.println(sizeof(tCommands));
  Serial.println(freeMemory());
  
  // sets the time to be the date and time of the compilation
//  cron.setTime(DateTime(__DATE__, __TIME__));
  cron.setTime(DateTime("Dec 25 2021", "19:29:50"));

//  tCommands[5] = TimedCommand("50.*.10.*.*.0,1,2,3,4,5,6", &execute, false); 
//  tCommands[5] = TimedCommand("50.*.15.*.*.4", &execute, false); //expected: Input: 29/04/2021 16:59:00, OUTPUT: 6 5 2021 15:0:50 4 

  tCommands[0] = TimedCommand("00.00.1.*.*.*", &execute, true);
  tCommands[1] = TimedCommand("00.00.3.*.*.*", &execute, false);
  tCommands[2] = TimedCommand("00.00.6.*.*.*", &execute, true);
  tCommands[3] = TimedCommand("00.00.9.*.*.*", &execute, true);
  tCommands[4] = TimedCommand("00.30.19.25.12.*", &execute, false);
  tCommands[5] = TimedCommand("00.00.12.*.*.1,3,5", &execute, true);
  tCommands[6] = TimedCommand("00.00.15.*.*.0,2,4,6", &execute, false);
}

// The loop function is called in an endless loop
void loop()
{
  // the loop funtion checks if a timed command 
  // is due to be executed and executes if it is
  cron.loop();
}

void execute(bool state){
  Serial.print("STATE: ");
  Serial.println(state);
}

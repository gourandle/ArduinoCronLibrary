/*
 * Cron.cpp
 *
 *  Created on: 7 Jun 2012
 *      Author: fletcherb
 */

#include "Cron.h"

extern RTC_DS1307 DS_RTC;

Cron:: Cron(){
}

Cron::Cron(StartStopTimer *iTimedCommands, int iTimedCommandsSize) {
  timedCommands = iTimedCommands;
  timedCommandsSize = iTimedCommandsSize;
  loopInterval = 0;
  
  nxtTrig = DateTime(2000,1,1,0,0,0);
}

Cron::~Cron() {
}

void Cron::resetNextTrig(){
  nxtTrig = DateTime(2000,1,1,0,0,0);
  loop(true);
}

void Cron::loop(bool force){
  unsigned long cur = millis();
  bool excStartIds[timedCommandsSize], excStopIds[timedCommandsSize];

  //initialize all index in arrays to be false
  for (uint8_t i = 0; i < timedCommandsSize; i++){
    excStartIds[i] = false;
    excStopIds[i] = false;
  }

  if ((cur - loopInterval) >= 500 || force){
    loopInterval = cur;
    DateTime time = getTime();
    
    // SerialUSB.print("CUR TIME: ");
    // printTime();

    for (int i = 0; i< timedCommandsSize; i++){

      // if (timedCommands[i].getEnabled()){
      //   SerialUSB.print(timedCommands[i].getEnabled());
      //   SerialUSB.print(" ");
      //   SerialUSB.print(i);
      //   SerialUSB.println(" ENABLED!");
      // } else {
      //   SerialUSB.print(timedCommands[i].getEnabled());
      //   SerialUSB.print(" ");
      //   SerialUSB.print(i);
      //   SerialUSB.println(" DISABLED!");
      // }

      if (timedCommands[i].getEnabled() && validateCronString(timedCommands[i].getStartCronString()) && matchCron(timedCommands[i].getStartCronString(),time)){
        excStartIds[i] = true;
      }

      if (timedCommands[i].getEnabled() && validateCronString(timedCommands[i].getStopCronString()) && matchCron(timedCommands[i].getStopCronString(),time)){
        excStopIds[i] = true;
      }
    }

    // SerialUSB.println("--------");
    // SerialUSB.print("NXT TRIG: ");
    // printTimeInput(nxtTrig);

    //FIXME: this can be executed twice within the same second.
    for (int i=0; i < timedCommandsSize; i++){
      if (excStartIds[i]) {
        excStartIds[i] = false;
        timedCommands[i].executeStartCommand();
      }
      if (excStopIds[i]) {
        excStopIds[i] = false;
        timedCommands[i].executeStopCommand();
      }
    }
  } //end first if
}

DateTime Cron::getTime(){
  DateTime now = DS_RTC.now();
  return now;
}

/**
 * *.*.*.*.*.* - min length
 * 59.59.23.31.12.0,1,2,3,4,5,6 - max length
 */
boolean Cron::validateCronString(String cronString){
  int len = cronString.length();
  uint8_t dotCount = 0;

  for (int i=0;i<len-1;i++){
    if (cronString[i] == '.') dotCount++;
  }

  if (len < 11 || dotCount != 5) {
    return false;
  } else {
    return true;
  }
}

boolean Cron::matchCron(String cronString, DateTime time){
  boolean secMatch,minMatch,hourMatch,dayMatch,monMatch,dotwMatch;
  uint8_t sec, min, hour, day = time.day(), month = time.month(), daysTillNextTrigger=0;
  uint16_t year = time.year();
  String tempTimeString;
  String commandString;
  DateTime trigTime, nxtDateTime;
  bool timeIsLess = false, daysChanged = false;

  int cronTime[6];
  cronString.trim();
  tempTimeString = cronString.substring(0,cronString.indexOf('.'));
  if (tempTimeString.equals("*")) {
    cronTime[0] = -1;
  } else {
    cronTime[0] = tempTimeString.toInt(); 
  }
  cronString.trim();
  cronString = cronString.substring(cronString.indexOf('.') + 1);
  tempTimeString = cronString.substring(0,cronString.indexOf('.'));
  if (tempTimeString.equals("*")) {
    cronTime[1] = -1;
  } else {
    cronTime[1] = tempTimeString.toInt();
  }
  cronString = cronString.substring(cronString.indexOf('.') + 1);
  tempTimeString = cronString.substring(0,cronString.indexOf('.'));
  if (tempTimeString.equals("*")) {
    cronTime[2] = -1;
  } else {
    cronTime[2] = tempTimeString.toInt();
  }
  cronString = cronString.substring(cronString.indexOf('.') + 1);
  tempTimeString = cronString.substring(0,cronString.indexOf('.'));
  if (tempTimeString.equals("*")) {
    cronTime[3] = -1;
  } else {
    cronTime[3] = tempTimeString.toInt();
  }
  cronString = cronString.substring(cronString.indexOf('.') + 1);
  tempTimeString = cronString.substring(0,cronString.indexOf('.'));
  if (tempTimeString.equals("*")) {
    cronTime[4] = -1;
  } else {
    cronTime[4] = tempTimeString.toInt();
  }
  cronString = cronString.substring(cronString.indexOf('.') + 1);
  tempTimeString = cronString.substring(0,cronString.indexOf('.'));
  if (tempTimeString.equals("*")) {
    cronTime[5] = -1;
  } else {
    uint8_t dotw = 0;
    // SerialUSB.println(tempTimeString);
    while (uint8_t i = tempTimeString.indexOf(',') != -1){
      dotw = dotw | 1 << tempTimeString.substring(0, i).toInt();
      tempTimeString = tempTimeString.substring(i+1);
    }

    dotw = dotw | 1 << tempTimeString.substring(0).toInt();
    // SerialUSB.println(dotw);

    cronTime[5] = dotw;
  }
  cronString = cronString.substring(cronString.indexOf('.') + 1);
  commandString = cronString.substring(0,cronString.indexOf('.'));

  cronString = "";

  //SECOND
  if (cronTime[0] == -1){
    secMatch = true; 
    sec = DateTime(time + TimeSpan(0,0,0,1)).second();
  } else {

    if (time.second() == cronTime[0]){
      secMatch = true;
    } else{
      secMatch = false;
    }

    sec = cronTime[0];
  }

  //MINUTE
  if (cronTime[1] == -1){
    minMatch = true;
    if (time.second() > sec )
      min = DateTime(time + TimeSpan(0,0,1,0)).minute();
    else 
      min = time.minute();

  } else {

    if (time.minute() == cronTime[1]){
      minMatch = true;
    } else{
      minMatch = false;
    }

    min = cronTime[1];
  }

  //HOUR
  if (cronTime[2] == -1){
    hourMatch = true;
    if (time.minute() > min )
      hour = DateTime(time + TimeSpan(0,1,0,0)).hour();
    else
      hour = time.hour();

  } else {

    if (time.hour() == cronTime[2]){
      hourMatch = true;
    } else{
      hourMatch = false;
    }

    hour = cronTime[2];
  }

  nxtDateTime = DateTime(year,month,day,hour,min,sec);
  // SerialUSB.print("NEXT TIME: ");
  // printTimeInput(nxtDateTime);

  if (nxtDateTime.unixtime() < time.unixtime()){
    timeIsLess = true;
    // SerialUSB.println("TRIGGER MODIFY DAYS");
  }

  //DOTW
  if (cronTime[5] == -1){
    if (timeIsLess){
      dotwMatch = false;
      daysTillNextTrigger = 1;
    } else {
      dotwMatch = true;
    }
    
    // SerialUSB.print("DAYS: ");
    // SerialUSB.println(daysTillNextTrigger);
  } else {
    uint8_t bitIndex = time.dayOfTheWeek();
    uint8_t dotw = (1 << bitIndex) & cronTime[5];
    daysTillNextTrigger = 0;

    if (dotw != 0 && timeIsLess)
      dotw = 0;

    while (dotw == 0){
      //avoid infinite loop. Max is a cycle of 7 days in a week.
      if (daysTillNextTrigger+1 > 7) break;

      bitIndex++;
      if (bitIndex > 6) bitIndex = 0;
      dotw = (1 << bitIndex) & cronTime[5];
      daysTillNextTrigger++;
    }

    if (daysTillNextTrigger == 0){
      dotwMatch = true;
    } else {
      dotwMatch = false;
    }
    
    // SerialUSB.print("DAYS: ");
    // SerialUSB.println(daysTillNextTrigger);
  }

  //DAY
  if (cronTime[3] == -1){
    dayMatch = true;

    day = (daysTillNextTrigger == 0 ) ? time.day() : DateTime(time + TimeSpan(daysTillNextTrigger,0,0,0)).day();
  } else {

    if (time.day() == cronTime[3]){
      dayMatch = true;
    } else {
      dayMatch = false;
    }

    day = cronTime[3];
  }
  
  //MONTH
  if (cronTime[4] == -1){
    monMatch = true;
    if (time.day() > day)
      month = time.month() + 1;
    else
      month = time.month();

    if (month > 12){
      month = 1;
      year++;
    }
  } else {

    if (time.month() == cronTime[4]){
      monMatch = true;
    } else{
      monMatch = false;
    }

    month = cronTime[4];
    if (time.month() > month){
      year++;
    }
  }

  nxtDateTime = DateTime(year,month,day,hour,min,sec);
  // SerialUSB.print("NEXT DATE: ");
  // printTimeInput(nxtDateTime);

  if (nxtDateTime.month() != time.month() || nxtDateTime.day() != time.day()){
    daysChanged = true;
  }

  if (daysChanged){
    if (cronTime[0] == -1){
      sec = 0;
    }

    if (cronTime[1] == -1 ){
      min = 0;
    }

    if (cronTime[2] == -1){
      hour = 0;
    }

    if (nxtDateTime.month() != time.month() && cronTime[3] == -1 && daysTillNextTrigger <= 1){
      day = 1;
    }
  }
  
  // if (month == time.month() && day == time.day() && (time.hour() > hour || (time.hour() == hour && time.minute() >= min && time.second() > sec))){
  //   if (cronTime[3] == -1)
  //     day = DateTime(time + TimeSpan(1,0,0,0)).day();
  //   else if (cronTime[4] == -1)
  //     month++;
  //     if (month > 12){
  //       month = 1;
  //       year++;
  //     }
  // }

  trigTime = DateTime(year,month,day,hour,min,sec);
  // SerialUSB.println("=====");
  // printTimeInput(trigTime);

  TimeSpan diff = trigTime - nxtTrig;
  // SerialUSB.println(diff.totalseconds());
  // printTimeInput(nxtTrig);
  // printTimeInput(trigTime);

  if ((diff.totalseconds() < 0 && trigTime.unixtime() > getTime().unixtime()) 
      || nxtTrig.unixtime() == DateTime(2000,1,1,0,0,0).unixtime() //overwrite the default nxtTrig timestamp
      || (diff.totalseconds() >= 0 && nxtTrig.unixtime() <= getTime().unixtime()) //
  ){
    nxtTrig = trigTime;
  }

  if (secMatch && minMatch && hourMatch && dayMatch && monMatch && dotwMatch){
    return true;
  } else {
    return false;
  }
}

void Cron::printTime(){
  printTimeInput(getTime());
}

void Cron::printTimeInput(DateTime time){
  SerialUSB.print(time.year(), DEC);
  SerialUSB.print('/');
  SerialUSB.print(time.month(), DEC);
  SerialUSB.print('/');
  SerialUSB.print(time.day(), DEC);
  SerialUSB.print(' ');
  SerialUSB.print(time.hour(), DEC);
  SerialUSB.print(':');
  SerialUSB.print(time.minute(), DEC);
  SerialUSB.print(':');
  SerialUSB.print(time.second(), DEC);
  SerialUSB.print(" ");
  SerialUSB.print(time.dayOfTheWeek(), DEC);
  SerialUSB.println();
}

void Cron::setTime(DateTime time){
  DS_RTC.adjust(time);
}

/**
 * @return difference between the time it was executed and next trigger time in seconds
 */ 
unsigned long Cron::getNextTrigDiff(){
  unsigned long nxtTrigDiff = 0;

  unsigned long nxtTrigU = nxtTrig.unixtime();
  unsigned long curTimeU = getTime().unixtime();

  // SerialUSB.println(nxtTrigU);
  // SerialUSB.println(curTimeU);

  if (nxtTrigU > curTimeU){
    nxtTrigDiff = nxtTrigU - curTimeU;
  }

  return nxtTrigDiff;
}
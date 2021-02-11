/*
 * Cron.h
 *
 *  Created on: 7 Jun 2012
 *      Author: fletcherb
 */

#ifndef CRON_H_
#define CRON_H_

#include <RTClib.h>
#include <Arduino.h>
#include "StartStopTimer.h"

class Cron {
public:
  Cron();
  Cron(StartStopTimer *,int); // Get pointer to array of pointers
  virtual ~Cron();
  void loop(bool force = false);
  void printTime();
  void printTimeInput(DateTime);
  void setTime(DateTime);
  unsigned long getNextTrigDiff();
  void resetNextTrig();
private:
  boolean matchCron(String,DateTime);
  boolean validateCronString(String);

  StartStopTimer* timedCommands;
  int timedCommandsSize;
  DateTime getTime();
  DateTime nxtTrig;
  unsigned long loopInterval;
};

#endif /* CRON_H_ */

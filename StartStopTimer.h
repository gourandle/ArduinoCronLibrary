/*
 * 
 * Very specific class for pin_handler control and device_handler control function
 * 
 * To-do:
 *  - make this a generic class that can control variety of external function via function pointers and also the parameters somehow?
 * 
 */

#ifndef STARTSTOPTIMER_H_
#define STARTSTOPTIMER_H_

#include "Arduino.h"
#include "command_definitions/X2_V1.pb.h"

//FIXME: find out how to make this more generic
typedef bool (*ctrlPtr)(uint8_t, bool, char *, bool);

// typedef struct{
//   char startCronString[28];
//   char stopCronString[28];
//   uint8_t address; //NOTE: differs from .proto is uint32_t
//   char originatingEventId[50];
//   _X2_V1_Command cmd;
// } Timer_Configuration;

class StartStopTimer{
public:
  StartStopTimer();
  StartStopTimer(Timers_Configuration, ctrlPtr);
  StartStopTimer(char *, char *, ctrlPtr, uint8_t, char *, _X2_V1_Command);
  virtual ~StartStopTimer();

  void setStartTimer(char *);
  void setStopTimer(char *);
  void setCommandFunction(ctrlPtr);
  void setParameters(uint8_t _addr, char * _eventId);
  void executeStopCommand();
  void executeStartCommand();

  Timers_Configuration getConfig();
  void setConfig(Timers_Configuration);

  char * getStartCronString();
  char * getStopCronString();
protected:
  Timers_Configuration config;
  ctrlPtr cmdFunc;
};

#endif

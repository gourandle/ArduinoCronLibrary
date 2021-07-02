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

class StartStopTimer{
public:
  StartStopTimer();
  StartStopTimer(Timers_Configuration, ctrlPtr);
  StartStopTimer(char *, char *, ctrlPtr, uint8_t, char *, _X2_V1_Command, bool);
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
  bool getEnabled();
protected:
  Timers_Configuration config = Timers_Configuration_init_zero;
  ctrlPtr cmdFunc;
};

#endif

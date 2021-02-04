#include "StartStopTimer.h"

StartStopTimer::StartStopTimer(){
  config.startCronString[0] = '\0';
  config.stopCronString[0] = '\0';
  config.originatingEventId[0] = '\0';
}

StartStopTimer::StartStopTimer(Timers_Configuration _config, ctrlPtr _cmdFunc){
  config = _config;
  cmdFunc = _cmdFunc;
}

StartStopTimer::StartStopTimer(char * startCron, char * stopCron, ctrlPtr _cmdFunc, uint8_t _addr, char * _eventId, _X2_V1_Command cmd) {
  cmdFunc = _cmdFunc;
  config.address = _addr;
  strcpy(config.originatingEventId, _eventId);
  config.cmd = cmd;

  //unused - kept here for reference
  // timers[0] = TimedCommand(startCron, (cmdPtr)(&StartStopTimer::executeStartCommand));
  // timers[1] = TimedCommand(stopCron, (cmdPtr)(&StartStopTimer::executeStopCommand));

  strcpy(config.startCronString, startCron);
  strcpy(config.stopCronString, stopCron);
}

StartStopTimer::~StartStopTimer() {
}

void StartStopTimer::setStartTimer(char * cronString){
  strcpy(config.startCronString, cronString);
}

void StartStopTimer::setStopTimer(char * cronString){
  strcpy(config.stopCronString,cronString);
}

void StartStopTimer::setCommandFunction(ctrlPtr _cmdFunc){
  cmdFunc = _cmdFunc;
}

void StartStopTimer::setParameters(uint8_t _addr, char * _eventId){
  config.address = _addr;
  strcpy(config.originatingEventId, _eventId);
}

void StartStopTimer::setConfig(Timers_Configuration _config){
  config = _config;
}

char * StartStopTimer::getStartCronString(){
  return config.startCronString;
}

char * StartStopTimer::getStopCronString(){
  return config.stopCronString;
}

Timers_Configuration StartStopTimer::getConfig(){
  return config;
}

void StartStopTimer::executeStartCommand(){
  cmdFunc(config.address, true, config.originatingEventId, false);
}

void StartStopTimer::executeStopCommand(){
  cmdFunc(config.address, false, config.originatingEventId, false); 
}
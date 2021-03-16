/*
changes
    2021-02-01 11:09:51 Initial coding
*/

#include <SchedTask.h>

// Constructor definitions
SchedTask::SchedTask (unsigned long nxt, unsigned long intval, pFunc fnc) : SchedBase(nxt, intval), func(fnc) {} // constructor definition
SchedTask::SchedTask (unsigned long nxt, unsigned long intval, long iters, pFunc fnc) : SchedBase(nxt, intval, iters), func(fnc) {} // constructor definition
SchedTask::SchedTask () : func(NULL) {} 									// default constructor
SchedTask::~SchedTask() {;}																// destructor

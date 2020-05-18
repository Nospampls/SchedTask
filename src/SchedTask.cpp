// SchedTask.cpp

/*
Change log
   05/17/2020 17:53 initial coding
*/
#include <SchedTask.h>

SchedTask::SchedTask (unsigned long nxt, unsigned long intval, pFunc fnc) : func(fnc), SchedBase(nxt, intval) { // constructor definition
	addTask(this);																					// add this new task to the dispatch list
}

SchedTask::SchedTask() : func(NULL), SchedBase(NEVER, ONESHOT) {					// default constructor definition
	addTask(this);
}

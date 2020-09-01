// SchedTask.cpp

/*
Change log
   05/17/2020 17:53 initial coding
*/
#include <SchedTask.h>

SchedTask::SchedTask (unsigned long nxt, unsigned long intval, pFunc fnc) : SchedBase(nxt, intval), func(fnc) { // constructor definition
	addTask(this);																					// add this new task to the dispatch list
}

SchedTask::SchedTask (unsigned long nxt, unsigned long intval, long iters, pFunc fnc) : SchedBase(nxt, intval, iters), func(fnc) { // constructor definition
	addTask(this);																					// add this new task to the dispatch list
}
SchedTask::SchedTask() : SchedBase(NEVER, ONESHOT), func(NULL) {					// default constructor definition
	addTask(this);
}

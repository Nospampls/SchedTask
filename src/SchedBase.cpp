/*
For a complete series of tutorials see:
https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

To make a small (secure) appreciation donation see:
https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=A2J54W4JEHZ6C

	SchedBase.cpp

	changes:

		10/19/2017 6:27PM initial coding
		10/01/2020 14:38 revisions for release 1.1.0
*/

#include <SchedBase.h>

// only used for testing
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } // allow use of Serial <<

// initialize static member(s) of SchedBase class
SchedBase* SchedBase::tasksHead = nullptr;
int SchedBase::taskCount = 0;

// constructor definitions
SchedBase::SchedBase (unsigned long nxt, unsigned long intval) : next(nxt), period(intval), iterations(-1)  {	// constructor definition
	taskID = taskCount;
	addTask(this);											// add this task to the list to be dispatched
}
SchedBase::SchedBase (unsigned long nxt, unsigned long intval, long iters) : next(nxt), period(intval), iterations(iters)  {	// constructor definition
	taskID = taskCount;
	addTask(this);											// add this task to the list to be dispatched
}
SchedBase::SchedBase () : next(NEVER), period(ONESHOT), iterations(-1) {	// default constructor definition
	taskID = taskCount;
	addTask(this);											// add this task to the list to be dispatched
}
// Dispatcher
void SchedBase::dispatcher() {										// dispatcher
	SchedBase* pTask = tasksHead;										// point to the first task in the list
	while (pTask) {														// loop thru the task linked list
		if (pTask->checkFunc()) { 										// only if the function to call is valid
			if (pTask->next != NEVER)  {								// do not dispatch if Next is NEVER
				unsigned long now = millis();							// get the current time
				if (pTask->iterations == 0) {							// iterations were specified and went to zero
					pTask->next = NEVER;									// prevent future dispatches
					pTask->iterations = -1;								// no more iterations
					break;													// done with this task, do not dispatch
				}
// proceed if iterations not specified or some remaining
				if ((signed long)(pTask->next - now) <= 0) {		// time to run the next task in the array? (see https://arduino.stackexchange.com/questions/12587/how-can-i-handle-the-millis-rollover/12588#12588)
					if (pTask->period == ONESHOT) {					// one-shot task?
						pTask->next = NEVER;								// ensure it won't run again
					}
					else {													// periodic task
						pTask->next = pTask->next + pTask->period; // compute the next time to dispatch it (when it should have run + period)
					}
					if (pTask->iterations > 0) {						// iterations specified and some remaining
						pTask->iterations--;								// decrement iterations remaining
					}
					pTask->callFunc();									// call the derived class function to dispatch the task
				}
			}
		}
		pTask = pTask->taskLink;										// get link to the next task, if any
	}
}
// addTask() to the linked list
int SchedBase::addTask(SchedBase* pBase) {						// add a new task to the dispatch list
		pBase->taskLink = tasksHead;									// link this task to previous head task
		tasksHead = pBase;												// this task is now at the head
		taskID = taskCount++;											// assign task ID and bump task count
		return taskCount;													// update the task count and return it
}
// setNext()
void SchedBase::setNext(unsigned long nxt) {						// set a new NEXT value
	if (nxt == NOW) {														// NOW?
		next = millis();													// use current time in ms
	}
	else {
		if (nxt == NEVER) {												// NEVER?
			next = NEVER;													// use 0xFFFFFFFF
		}
		else {																// neither NOW nor NEVER
			next = millis() + nxt;										// add it to current millis() time
		}
	}
}
SchedBase::~SchedBase() {												// destructor
	SchedBase* prev = tasksHead;										// init ptr to previous task
	for (int i=0; i<taskCount; i++) {								// loop through the linked tasks
		if (this == tasksHead) {										// destructing the first task?
			tasksHead = taskLink;										// head is now the second task
			taskCount--;													// decr task count
			break;															// we're done
		}
		else {																// not first task
			if (prev->taskLink == this) {								// if this is the one to destruct
				prev->taskLink = this->taskLink;						// point prev task to next task (remove this one)
				taskCount--;												// decr task count
				break;														// we're done
			}
			else {															// this is not the one to destruct
				prev = this;												// so update the prev task pointer to this one
			}
		}
	}																			// and go around again
}

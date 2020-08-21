/*
For a complete series of tutorials see:
https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

To make a small (secure) appreciation donation see:
https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=A2J54W4JEHZ6C

	SchedBase.cpp

	changes:

		10/19/2017 6:27PM initial coding
		10/21/2017 1:11PM clean up
		3/12/2020 1:55PM callFunc virtual
		05/17/2020 17:56 move definitions
		7/4/2020 10:25AM correct handling of NEVER if passed in setNext()
		7/6/2020 10:54PM handle millis() overflow at 49 days
		08/20/2020 23:48 use linked list of tasks rather than vector of tasks
*/

#include <SchedBase.h>

// initialize static member(s) of SchedBase class
SchedBase* SchedBase::tasksHead = NULL;
int SchedBase::taskCount = 0;

SchedBase::SchedBase (unsigned long nxt, unsigned long intval) : period(intval), next(nxt) {}	// constructor

void SchedBase::dispatcher() {										// dispatcher

	SchedBase* pTask = tasksHead;										// point to the first task in the list

	while (pTask) {														// loop thru the task linked list
		if (pTask->getFunc() != NULL) { 								// only if the function to call is valid
			if (pTask->next != NEVER)  {								// do not dispatch if Next is NEVER
				unsigned long now = millis();							// get the current time
				if ((signed long)(pTask->next - now) <= 0) {		// time to run the next task in the array? (see https://arduino.stackexchange.com/questions/12587/how-can-i-handle-the-millis-rollover/12588#12588)
					if (pTask->period == ONESHOT) {					// one-shot task?
						pTask->next = NEVER;								// ensure it won't run again
					}
					else {													// periodic task
						pTask->next = pTask->next + pTask->period; // compute the next time to dispatch it (when it should have run + period)
					}
					pTask->callFunc();									// call the derived class function to dispatch the task
				}
			}
		}
		pTask = pTask->taskLink;										// get link to the next task, if any
	}
}

int SchedBase::addTask(SchedBase* pBase) {						// add a new task to the dispatch list
		pBase->taskLink = tasksHead;									// link this task to previous head task
		tasksHead = pBase;												// this task is now at the head
		taskCount++;														// update the task count
}

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

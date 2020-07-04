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
*/

#include <SchedBase.h>

// initialize static member(s) of SchedBase class

vector<SchedBase*> SchedBase::tasks;								// not sure why this is necessary


SchedBase::SchedBase (unsigned long nxt, unsigned long intval) : period(intval), next(nxt) {}	// constructor

void SchedBase::dispatcher() {										// dispatcher

	for (int i=0; i<tasks.size(); i++) {							// loop thru the tasks in the array

		if (tasks[i]->getFunc() != NULL) { 							// only if the function to call is valid

			unsigned long now = millis();								// get the current time

			if (now >= tasks[i]->next) {								// time to run the next task in the array?

				if (tasks[i]->period == ONESHOT) {					// one-shot task?
					tasks[i]->next = NEVER;								// ensure it won't run again
				}
				else {														// periodic task
					tasks[i]->next = tasks[i]->next + tasks[i]->period; // compute the next time to dispatch it (when it should have run + period)
				}
				tasks[i]->callFunc();									// call the derived class function to dispatch the task
			}
		}
	}
}

int SchedBase::addTask(SchedBase* pBase) {						// add a new task to the dispatch list
		tasks.push_back(pBase);
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


/*
For a complete series of tutorials see:
https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

To make a small (secure) appreciation donation see:
https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=A2J54W4JEHZ6C

	SchedBase.h - header file for SchedTask base class

	SchedTaskT and SchedTask are derived from SchedBase
	See those header files for usage

	changes:
		10/19/2017 6:17PM initial coding
		3/9/2020 12:47PM updated
		3/12/2020 9:04AM more virtual functions
		4/24/2020 5:07PM changed #include for vector.h
		05/17/2020 14:26 remove NEVER, NOW, ONESHOT consts and make them #defines
		7/4/2020 10:44AM remove obsolete commented out setNext definition, added comment to declaration
		08/20/2020 23:54 use linked list of tasks rather than vector of tasks
*/

#ifndef SchedBase_h
#define SchedBase_h

// sched task value for next = never (wait for a change to dispatch)
#define NEVER 0xFFFFFFFF

// sched task value for next = now (dispatch immediately)
#define NOW 0UL

// sched task value for period = one shot (dispatch only once at t=next)
#define ONESHOT 0UL

#include <Arduino.h>

typedef void (*pFunc)();  // pFunc is of Type pointer to a function that takes no argument and returns void

class SchedBase {

	public:

		SchedBase (unsigned long next, unsigned long period);	// constructor declaration

		static void dispatcher ();																// see if any task is ready for dispatch (static -- no object required); call as SchedBase::dispatcher() in loop()
		void setNext(unsigned long nxt);														// set new Next declaration
		void setPeriod(unsigned long per) {period = per;} 								// set a new period
		void setIterations(unsigned long iter) {iterations = iter;}
		virtual void setFunc(pFunc) {;};														// set the function to dispatch (virtual -- actual is in the derived SchedTaskT class
		unsigned long getNext() {return next;}												// get Next
		unsigned long getPeriod() {return period;}										// get Period
		unsigned long getIterations() {return iterations;}
		virtual pFunc getFunc() {;};															// get function pointer  - don't see a use for this
		int getTaskCount() {return taskCount;}												// get task count

	protected:

		// only one copy of static members for the class, not one per object/instance
		static SchedBase* tasksHead;															// head of linked list of tasks
		static int taskCount;																	// task taskCount

		unsigned long next;																		// next
		unsigned long period;																	// period
		unsigned long iterations;																// iterations
		virtual void callFunc() {;};															// have the derived class call the task
		SchedBase* taskLink;																		// link to next task in list
		int addTask(SchedBase* pBase);														// add another task to the list
};

#endif

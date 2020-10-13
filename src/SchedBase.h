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
	10/01/2020 14:37 revisions for release 1.1.0
	10/12/2020 14:09 make callFunc and checkFunc pure virtual
*/

#ifndef SchedBase_h
#define SchedBase_h

#include <Arduino.h>

// sched task value for next = never (wait for a change to dispatch)
#define NEVER 0xFFFFFFFF
#define SCHED_NEVER 0xffffffff

// sched task value for next = now (dispatch immediately)
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
// this is required for this library to compile for the above platforms
// this will undefine NOW() used by those platforms perhaps causing some minor degradation in functionality
#undef NOW
#endif
// sched task value for next = now (dispatch immediately)
#define NOW 0UL

// sched task value for period = one shot (dispatch only once at t=next)
#define ONESHOT 0UL

class SchedBase {
	typedef void (*pFunc)();

	public:

		SchedBase (unsigned long next, unsigned long period);	// constructor declaration
		SchedBase (unsigned long next, unsigned long period, long iterations); // constructor declaration with iterations
		virtual ~SchedBase ();											// destructor

		static void dispatcher ();										// see if any task is ready for dispatch (static -- no object required); call as SchedBase::dispatcher() in loop()

		void setNext(unsigned long nxt);								// set new Next declaration
		void setPeriod(unsigned long per) {period = per;} 		// set a new period
		void setIterations(int iter) {iterations = iter;}		// set the iterations
		unsigned long getNext() {return next;}						// get Next
		unsigned long getPeriod() {return period;}				// get Period
		int getIterations() {return iterations;}					// return iterations
		int getTaskCount() {return taskCount;}						// get task count
		int getTaskID() {return taskID;}								// 0, 1, ... in order of instantiation
		virtual void setFunc(pFunc) =0;								// set function
		virtual pFunc getFunc() =0;									// get function

	protected:
		SchedBase ();														// default constructor declaration protected to prevent instantiation of abstract base class
		// only one copy of static members for the class, not one per object/instance
		static SchedBase* tasksHead;									// head of linked list of tasks
		static int taskCount;											// task taskCount

		SchedBase* taskLink;												// link to next task in list

		unsigned long next;												// next
		unsigned long period;											// period
		int iterations;													// iterations (-1 means not specified)
		int taskID;															// 0, 1, ... in order of instatiation

		int addTask(SchedBase*);										// add another task to the linked list								
		virtual void callFunc() =0;									// have the derived class call the task
		virtual bool checkFunc() =0;									// whether func is non-NULL
};

#endif

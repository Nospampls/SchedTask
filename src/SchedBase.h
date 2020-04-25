/*
	SchedBase.h - header file for SchedTask base class
	
	SchedTaskT and SchedTask are derived from SchedBase
	See those header files for usage
	
	changes:
		10/19/2017 6:17PM initial coding
		3/9/2020 12:47PM updated
		3/12/2020 9:04AM more virtual functions
		4/24/2020 5:07PM changed #include for vector.h

*/

#ifndef SchedBase_h
#define SchedBase_h

#include <Arduino.h>
#include <SchedTaskLib\vector.h>

const unsigned long NEVER		= -1;			// sched task value for next = never (wait for a change to dispatch)
const unsigned long NOW			= 0UL;		// sched task value for next = now (dispatch immediately)
const unsigned long ONESHOT	= 0UL;		// sched task value for period = one shot (dispatch only once at t=next)

typedef void (*pFunc)();  // pFunc is of Type pointer to a function that takes no argument and returns void

class SchedBase {
	
	public:
	
		SchedBase (unsigned long next, unsigned long period);	// constructor declaration
		
		static void dispatcher ();																// see if any task is ready for dispatch (static -- no object required); call as SchedBase::dispatcher() in loop()
		void setNext(unsigned long nxt) {next = nxt==NOW?millis():millis() + nxt;}	// set new Next
		void setPeriod(unsigned long per) {period = per;} 								// set a new period
		virtual void setFunc(pFunc) {;};														// set the function to dispatch (virtual -- actual is in the derived SchedTaskT class
		unsigned long getNext() {return next;}												// get Next
		unsigned long getPeriod() {return period;}										// get Period
		virtual pFunc getFunc() {;};															// get function pointer  - don't see a use for this
		int getTaskCount() {return tasks.size();};										// get task count

			
	protected:
	
		// only one copy of static members for the class, not one per object/instance
		static vector<SchedBase*> tasks;														// the array of tasks to dispatch
	
		unsigned long period;																	// period
		unsigned long next;																		// next
		virtual void callFunc() {;};															// have the derived class call the task
		int addTask(SchedBase* pBase);														// add another task to the list		
};

#endif
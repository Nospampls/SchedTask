/*
For a complete series of tutorials see:
https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

To make a small (secure) appreciation donation see:
https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=A2J54W4JEHZ6C

	SchedTask.h - header file for the SchedTask class, derived from SchedBase.

	This class constructs an object defining a task to be dispatched periodically.

	(See also SchedTaskT.h which is similar but passes a parament to to dispatched task.)

	The constructor takes three positional parameters.

	Syntax:

	SchedTask taskName (next, period, function);
		where:
			taskName is arbitrary but must be unique
			next is when to dispatch function in the future (in milliseconds) (unsigned long)
			period is how often to dispatch function (in milliseconds) (unsigned long)
			function is the name of the  function to be dispatched that takes a parameter of type T and returns void


	The dispatched function needs to take the form:

		void func() {...}

	'Next' can be NOW (0) or NEVER (-1). If 'next' is NEVER it can be changed at run time with a call to the member function setNext(unsigned long newNext).

	The period can be set to ONESHOT (-1) in which case the function will be called only once.  It can be changed with a call to the member function setPeriod(unsigned long newPeriod).

	You create instances of the class in your main .ino sketch, before setup();

	If you create an instance of the class using the default constructor (i.e. no arguments) then you must use member function calls to alter it at run time.

		taskName.setNext(someNewNext);
		taskName.setPeriod(someNewPeriod);
		taskName.setFunc(someOtherFunction);

	consts NOW, NEVER, ONESHOT are defined in SchedBase.h

	The functions to dispatch must be 'forward declared' before the SchedTaskT objects are constructed to avoid compiler errors.  For example,

		void pan();
		void once();
		void never();

	Examples:

		SchedTask panTask(0, 100, pan); 						// function 'pan' will get dispatched immediately and every 100 ms thereafter
		SchedTask panTask(NOW, 100, pan);					// same as above

		SchedTask panTask(1000, 100, pan); 					// function 'pan' will get dispatched 1 sec from now and every 100 ms thereafter

		SchedTask oneShot(1000, 0, once);					// function 'once' will get dispatched 1 sec from now and never again
		SchedTask oneShot(1000, ONESHOT, once);			// same as above

		SchedTask notYet(NEVER, ONESHOT, never);			// function 'never' will not get dispatched (yet)
			use for example:
				notYet.setPeriod(5000); 						// modify the period to 5 sec
				and:
				notYet.setNext(10000);		   				// dispatch in 10 sec
				and:
				notYet.setFunc(funcName);						// when it's time to dispatch, dispatch funcName

		SchedTask taskName();									// use default constructor
		SchedTask taskName(NEVER, ONESHOT, NULL);			// equivalent to using default constructor
			use the following member functions to allow dispatching
				taskName.setNext(someNext)'
				taskName.setPeriod(somePeriod); 				//only necessary if default of ONESHOT is not desired
				taskName.setFunc(anotherFunc);

	To call member functions you need to use the name of the object (such as panTask in some examples above).

		panTask.setNext(200);

	However, when you want to dispatch a task	that is common to several scheduled tasks
	and therefore does not know the name of the object, see SchedTaskT.h.

	You must run the dispatcher in loop():

		SchedBase::dispatcher();								// note that no particular object is specified

	See SchedBase.h for a list of public member functions.

	See the included Example_5.ino for other examples.

	changes
		9/23/2014 7:44PM initial coding
		4/24/2020 11:18AM updates to comments
		05/17/2020 14:49 move constructor definitions
*/

#ifndef SchedTask_h
#define SchedTask_h

#include <SchedBase.h>

class SchedTask : public SchedBase {

	public:

		SchedTask(unsigned long next, unsigned long period, pFunc); 				// constructor declaration
		SchedTask();																				// default constructor declaration

		virtual pFunc getFunc() {return(func);}											// fetch a pointer to the function
		virtual void setFunc(pFunc pFnc) {func = pFnc;}									// set a new function pointer

	protected:

		virtual void callFunc() {func();}													// call the task on behalf of dispatcher
		pFunc func;																					// the stored address of func
};

#endif

/*
For a complete series of tutorials see:
https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

To make a small (secure) appreciation donation see:
https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=A2J54W4JEHZ6C

	SchedTask.h - header file for the SchedTask class, derived from SchedBase.

	This class constructs an object defining a task to be dispatched periodically.

	(See also SchedTaskT.h which is similar but passes a parament to to dispatched task.)

	The constructor takes three or four positional parameters.

	Syntax:

	SchedTask TaskName (next, period, {iterations,} function); // iterations is optional
		where:
			TaskName is arbitrary but must be unique
			'next' is when to dispatch function in the future (in milliseconds) (unsigned long)
			'period' is how often to dispatch function (in milliseconds) (unsigned long)
			'iterations', if specified, is how many times to dispatch function (long)
			'function' is the name of the function to be dispatched that returns void

	The dispatched function needs to take the form:

		void func() {...}

	'Next' can be NOW (0) or NEVER (0xFFFFFFFF). If 'next' is NEVER it can be changed at run time with a call to the member function setNext(unsigned long newNext).

	The period can be set to ONESHOT (0) in which case the function will be called only once.  It can be changed with a call to the member function setPeriod(unsigned long newPeriod).

	You create instances of the class in your main .ino sketch, before setup();

	If you create an instance of the class using the default constructor (i.e. no arguments) then you must use member function calls to alter it at run time.

		TaskName.setNext(someNewNext);
		TaskName.setPeriod(someNewPeriod);
		TaskName.setIterations(someCount);
		TaskName.setFunc(someOtherFunction);

	Definitions of NOW, NEVER, ONESHOT are in SchedBase.h

	The functions to dispatch must be 'forward declared' before the SchedTaskT objects are constructed to avoid compiler errors.  For example,

		void pan();
		void once();
		void never();

	Examples:

		SchedTask PanTask(0, 100, pan); 						// function 'pan' will get dispatched immediately and every 100 ms thereafter
		SchedTask PanTask(NOW, 100, pan);					// same as above

		SchedTask PanTask(1000, 100, pan); 					// function 'pan' will get dispatched 1 sec from now and every 100 ms thereafter

		SchedTask OneShot(1000, 0, once);					// function 'once' will get dispatched 1 sec from now and never again
		SchedTask OneShot(1000, ONESHOT, once);			// same as above

		SchedTask NotYet(NEVER, ONESHOT, never);			// function 'never' will not get dispatched (yet)
			use for example:
				NotYet.setPeriod(5000); 						// modify the period to 5 sec
				and:
				NotYet.setNext(10000);		   				// dispatch in 10 sec
				and:
				NotYet.setFunc(funcName);						// when it's time to dispatch, dispatch funcName

		SchedTask TaskName();									// use default constructor
			use the following member functions to allow dispatching
				TaskName.setNext(someNext);					// default is NEVER
				TaskName.setPeriod(somePeriod); 				// default is ONESHOT
				TaskName.setIterations(someCount);			// default is -1 (not specified)
				TaskName.setFunc(anotherFunc);				// default is NULL

	To call member functions you need to use the name of the object (such as PanTask in some examples above).

		PanTask.setNext(200);

	However, when you want to dispatch a function that is common to several scheduled tasks
	and needs to know the name of the SchedTask object, see SchedTaskT.h.

	You must run the dispatcher in loop():

		SchedBase::dispatcher();								// note that no particular object is specified

	See SchedBase.h for a list of public member functions.

	See the included Example_5.ino and Example_10 for other examples.

	The following is somewhat redundant and is taken from README.md in SchedTask on GitHub.

	****************************************************************************************************

	Here is a complete description of what the SchedTask Library does, how it does it, and how to use it.

	A Scheduled Task is a function to be called by a dispatcher and its associated timing specifications.  A function can be scheduled at some point in the future to run once or multiple times at some interval.

	A Scheduled Task is defined by a SchedTask object, an instance of a class.

	You create definitions of one or more Scheduled Tasks.  Each definition specifies the following:

	   When to next dispatch the associated function ('next') in milliseconds into the future;
	   How often to dispatch the associated function (the 'period') in miliseconds;
	   Optionally how many times ('iterations') to dispatch the associated function;
	   The name of the associated function.

	The Dispatcher examines the list of Scheduled Tasks to see if any is due for dispatching based on the Arduino function millis() which returns the number of milliseconds since starting your sketch.  (The library is designed to be immune from millis() overflow.)

	If a Task is due, the function specified by the Task is called.  That function returns to the Dispatcher which then exits.

	More technically, the definitions you create are calls to a constructor (SchedTask) in this form:

	   SchedTask TaskName(next, period, {iterations, } function);  // a Scheduled Task object

	Where:
	   TaskName is arbitrary but must be unique within your sketch;
	   'next' is when in the future to call (dispatch) 'function', in milliseconds (type unsigned long);
	   'period' is how often to call (dispatch) 'function', in milliseconds (type unsigned long);
	   the optional 'iterations' is how many times to call (dispatch) 'function', (type int);
	   'function' is the name of the function to be dispatched.

	There are some special values that you can use:
	   'next' can be NOW, which means dispatch immediately;
	   'next' can be NEVER, which means do not dispatch (until changed at runtime);
	   'period' can be ONESHOT, which means dispatch only once.  This is equivalent to specifying iterations as 1.

	The Dispatcher runs continuously in the required Arduino loop() function in this form:

	   SchedBase::dispatcher();

	You should not include any other code in loop() in order not to impact the timely execution of the Dispatcher.  Furthermore, any Scheduled Task (i.e., dispatched function) should be of short duration relative to the granularity of the scheduling you require.  In other words, a dispatched function that ties up the processor for 100 ms when there are tasks to be dispatched every 20 ms would not produce desired results.

	When you define a Scheduled Task (using the constructor) it is automatically added to the Dispatcher's task list.  If any tasks are to be dispatched simultaneously, the most recently added one will be dispatched first.  In other words, they are checked in reverse order to how they were constructed.

	If 'next' is NEVER that takes precedence over any iterations remaining and the task will not be dispatched.  If period is ONESHOT that will also override any iterations remaining.

	SchedTask is a derived class of SchedBase.  SchedTaskT is another derived class.  It differs from SchedTask in that it allows the dispatched function to receive a parameter of virtually any type.

	The usage of SchedTaskT is similar to that of SchedTask.  Here is the syntax:

	   SchedTaskT<someType> TaskName(next, period, {iterations, } function, parameter);

	So the following are valid calls to the constructor:

	   SchedTaskT<int> TaskName(NEVER, ONESHOT, 5, myFunc, 6);     // pass the int value 6 to myFunc when dispatched (it won't be dispatched until 'next' is changed from NEVER)

	   SchedTastT<> TaskName(NEVER, ONESHOT, 5, myFunc, 6);        // same as above, int is implied or default

	   SchedTaskT<int&> TaskName(1000, 500, myFunc, rMyInt);       // pass a reference to an int to myFunc when dispatched

	   SchedTaskT<String> TaskName (1000, 500, myFunc, myString);  // pass a String object

	See Example 6 for examples of other types.

	Because SchedTask does not make use of interrupts, your executing code is not interrupted by the Dispatcher.  This means you can modify the parameters (members) of any Scheduled Task on the fly, including the current one.  In other words, a Scheduled Task can modify itself which will influence future dispatching.

	The following member functions are provided:

	   TaskName.setNext(unsigned long next);           // set the 'next' value for TaskName

	   TaskName.setPeriod(unsigned long period);       // set the 'period' value for TaskName

	   TaskName.setFunc(myFunc);                       // myFunc is the name of your function that takes no argument and returns void

	   TaskName.setIterations(int iters);              // set the number of iterations for TaskName

	   unsigned long TaskName.getNext();               // return the 'next' value

	   unsigned long TaskName.getPeriod();             // return the 'period' value

	   int iters TaskName.getIterations();             // return the number of iterations (dispatches) remaining for TaskName

	   int TaskName.getTaskCount();                    // return the number of tasks in the list

	   int TaskName.getTaskID();                       // return the task ID for TaskName (an integer)

	   typedef void (*pFunc)();
	   pFunc pF = TaskName.getFunc();                  // obtain the function's address

	Thus a Scheduled Task defined (constructed) like this:

	   SchedTaskT<int> TaskName (NEVER, ONESHOT, myFunc, 6); // placeholder task due to NEVER

	Could be altered at runtime with:

	   TaskName.setNext(60000UL); // dispatch in 1 minute
	   TaskName.setPeriod(1000);  // dispatch every second
	   TaskName.setIterations(4); // dispatch only 4 times, then go dormant
	   TaskName.setFunc(myNewFunc); // even alter the function associated with this Scheduled Task object

	This will result in a new function (myNewFunc) to be dispatched in 60 seconds and every 1 second thereafter, 4 times.

	A currently active Scheduled Task (including the one executing) can be 'killed' (prevented from being further dispatched) like this:

	   TaskName.setNext(NEVER);

	There are default constructors:

	   SchedTask SomeTask();
	   SchedTaskT SomeTaskT();

	In order for these scheduled tasks to be useful you will need to update their parameters using some of the member functions above.

	***********************************

	Here are the minimum requirements to use the Scheduled Task Library:

	Before setup() pre-declare any functions you will define as Scheduled Tasks, for example:

	   void myTask1();
	   void myTask2(long);
	   void myTask3();

	Define any Scheduled Tasks before setup(), for example:

	   SchedTask MyTask1(NEVER, ONESHOT, myTask1);           // wait for call to setNext to override NEVER
	   SchedTaskT<int> MyTask2(10000, 60000UL, myTask2, 4L);  // start dispatching myTask2 in 10 sec every minute, pass it '4'
	   SchedTask MyTask3(1000, 300, 5, myTask3);             // dispatch myTask3 in 1 sec, every 300 ms, 5 times

	Include this in loop():

	   SchedBase::dispatcher();

	Write your functions:

	   void myTask1() {;}

	   void myTask2(long i) {;}

	   void myTask3() {;}
**************************************************

	Change log

		9/23/2014 7:44PM initial coding
		4/24/2020 11:18AM updates to comments
		05/17/2020 14:49 move constructor definitions
		08/21/2020 11:25 improved/corrected comments
		09/01/2020 14:07 added documentation from README.md
		09/25/2020 10:09 inherit directly from SchedBase
*/

#ifndef SchedTask_h
#define SchedTask_h

#include <SchedBase.h> // base class

class SchedTask : public SchedBase {
	typedef void (*pFunc)();  // pFunc is of Type pointer to a function that takes no argument and returns void

	public:

		SchedTask(unsigned long next, unsigned long period, pFunc pFnc); 			// constructor declaration
		SchedTask(unsigned long next, unsigned long period, long iterations, pFunc pFnc); // constructor declaration
		SchedTask();																				// default constructor declaration
		~SchedTask();																				// destructor

		void setFunc(pFunc pF) {func = pF;}													// set new function pointer
		pFunc getFunc() {return(func);} 														// return function pointer

	private:

		pFunc func;																					// the stored address of func
		virtual void callFunc() {func();}													// call the task on behalf of dispatcher
		virtual bool checkFunc() {return func != NULL;}									// whether func contains non-NULL
};
// Constructor definitions
SchedTask::SchedTask (unsigned long nxt, unsigned long intval, pFunc fnc) : SchedBase(nxt, intval), func(fnc) {} // constructor definition
SchedTask::SchedTask (unsigned long nxt, unsigned long intval, long iters, pFunc fnc) : SchedBase(nxt, intval, iters), func(fnc) {} // constructor definition
SchedTask::SchedTask () : SchedBase(), func(NULL) {} 									// default constructor
SchedTask::~SchedTask() {;}																	// destructor

#endif

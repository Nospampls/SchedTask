/*
For a complete series of tutorials see:
https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

To make a small (secure) appreciation donation see:
https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=A2J54W4JEHZ6C

SchedTaskT.h - header file for the SchedTaskT template class, derived from SchedBase.

Change Log for description
	02/01/2021 14:26 corrected reference to setArg should be setParm

This class constructs an object defining a task to be dispatched periodically that receives a parameter.

(See also SchedTask.h which dispatches a task without passing a parameter.)

The constructor takes four or five positional parameters.

Syntax:

SchedTaskT<T> TaskName (next, period, {iterations,} function, parameter);	// 'iterations' is optional
	where:
		TaskName is arbitrary but must be unique
		'next' is when to dispatch function in the future (in milliseconds) (unsigned long)
		'period' is how often to dispatch function (in milliseconds) (unsigned long)
		'iterations', if specified, is how many times to dispatch function
		'function' is the name of the  function to be dispatched that takes a parameter of type T and returns void
		'parameter' is what to pass to the dispatched function (of type T)

T will default to int.  Therefore these are equivalent:

	SchedTaskT<int> ...
	SchedTaskT<> ...

The dispatched function needs to take the form:

	void func(T parameter) {...}  // where T is the type of the parameter passed

'Next' can be NOW (0) or NEVER (0xFFFFFFFF). If 'next' is NEVER it can be changed at run time with
a call to the member function setNext(unsigned long newNext).

The period can be set to ONESHOT (0) in which case the function will be called only once.
It can be changed with a call to the member function setPeriod(unsigned long newPeriod).

You create instances of the class in your main .ino sketch, before setup();

If you create an instance of the class using the default constructor (i.e. no arguments)
then you must use member function calls to alter it at run time.

	TaskName.setNext(someNewNext);
	TaskName.setPeriod(someNewPeriod);
	TaskName.setIterations(someCount);	// if desired
	TaskName.setFunc(someOtherFunction);
	TaskName.setParm();

Definitions of NOW, NEVER, ONESHOT are in SchedBase.h

The functions to dispatch must be 'forward declared' before the SchedTaskT objects are constructed to avoid compiler errors.  For example,

	void pan(int);
	void once(int);
	void never(int);

Examples:

	int param 5;

	SchedTaskT<int> PanTask(0, 100, pan, param); 	// function 'pan' will get dispatched immediately and every 100 ms thereafter and be passed 'param'
	SchedTaskT<> PanTask(NOW, 100, pan, param);		// same as above

	SchedTaskT<> PanTask(1000, 100, pan, param); 	// function 'pan' will get dispatched 1 sec from now and every 100 ms thereafter and be passed 'param'

	SchedTaskT<> OneShot(1000, 0, once, param);		// function 'once' will get dispatched 1 sec from now and never again
	SchedTaskT<> OneShot(1000, ONESHOT, once, param);	// same as above

	SchedTaskT<> NotYet(NEVER, ONESHOT, never, param);	// function 'never' will not get dispatched (yet)
		use for example:
			NotYet.setPeriod(5000); 						// modify the period to 5 sec
			and:
			NotYet.setNext(10000);		   				// dispatch in 10 sec
			and:
			NotYet.setFunc(funcName);						// when it's time to dispatch, dispatch funcName
			and
			NotYet.setParm(param + 10);					// change the parameter to pass

	SchedTaskT<> Iterated(NOW, 1000, 5, someFunc, param); // dispatch someFunc immediately and every second thereafter, 5 times

	SchedTaskT<> TaskName;									// use default constructor
	SchedTaskT<int> TaskName(NEVER, ONESHOT, NULL, NULL);	// equivalent to using default constructor
		use the following member functions to allow dispatching
			TaskName.setNext(someNext);
			TaskName.setPeriod(somePeriod); 				//only necessary if default of ONESHOT is not desired
			TaskName.setIterations(someCount);
			TaskName.setFunc(SomeFunc);
			TaskName.setParm(someParm);

To pass other types (i.e. not integers), use this as a model:

	double pi = 3.14;
	void oneTime(double);

	SchedTaskT<double> DubTask(1000, ONESHOT, oneTime, pi);	// the parameter 'pi' will be passed to the function 'oneTime' in one second

	Examples of supported types for the parameter to be passed are:

		int
		long
		double
		String
		int&
		double&
		String&

To call member functions you generally need to use the name of the object (such as panTask in some examples above).

	PanTask.setNext(200);

However, when you want to dispatch a task	that is common to several scheduled tasks
and therefore does not know the name of the object, you can use this form:

	SchedTaskT<SchedBase *> TaskName(next, period, func, &taskName);

	The dispatched task 'func' will be passed a pointer to the base class SchedBase on which member functions can be called.
	To call member functions (for the next dispatch) in such a scheduled task you can do the following:

	void func(SchedBase* pBase) {
		pBase->setFunc(someNewFunc);
		pBase->setNext(someNewNext);
		pBase->setPeriod(someNewPeriod);
	}

You must run the dispatcher in loop():

	SchedBase::dispatcher();					// note that no particular object is specified

See SchedBase.h for a list of public member functions.

See the included Example_6.ino for other examples.

The following is somewhat redundant and is taken from README.md in SchedTask on GitHub.

************************ DETAILED DESCRIPTION ***********************************************************************

Change Log
   10/01/202013:39 updated

Here is a complete description of what the SchedTask Library does, how it does it, and how to use it.  Although this description may seem daunting due to its completeness, you don't need to know much in order to use the library.  You may wish to refer to the examples provided in the library.  See Minimum Requirements at the end of this description.

A Scheduled Task is a function to be called by a Dispatcher along with its associated timing specifications.  A function can be scheduled at some point in the future to run once or multiple times at some interval.

A Scheduled Task is defined by a SchedTask statement.  (Technically, this statement is a call to the constructor of a class.)

You create definitions of one or more Scheduled Tasks.  Each definition specifies the following:

   When to next dispatch the associated function ('next') in milliseconds into the future;
   How often to dispatch the associated function (the 'period') in milliseconds;
   Optionally how many times ('iterations') to dispatch the associated function;
   The name of the associated function.

The Dispatcher examines the list of Scheduled Tasks to see if any is due for dispatching.  (It uses the Arduino function millis() which returns the number of milliseconds since starting your sketch.  The library is designed to be immune from millis() overflow.)

If a Task is due, the function specified by the Task is called.  That function returns to the Dispatcher which then exits (returns to the loop() function).

The scheduled task definitions you create (constructor calls) look like this:

   SchedTask TaskName(next, period, {iterations, } function);

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

Here are some examples.  These statements would appear above setup();

   void doSomething(); // forward declaration required by the compiler

   SchedTask Task1 (10000, 2000, doSomething);  // dispatch doSomething() every 2 seconds starting 10 seconds after startup
   SchedTask Task2 (NOW, 2000, doSomething); // similar, but dispatch immediately
   SchedTask Task3 (NOW, 2000, 3, doSomething); // similar, but limit to 3 dispatches
   SchedTask Task3 (NOW, ONESHOT, doSomething); // dispatch doSomething immediately but only once
   SchedTask Task4 (NEVER, 2000, doSomething); // 'next' will be specified later in the sketch using setNext()

The Dispatcher runs continuously in the required Arduino loop() function in this form:

   void loop() {
      SchedBase::dispatcher();
   }

You should not include any other code in loop() in order not to impact the timely execution of the Dispatcher.  Furthermore, any Scheduled Task (i.e., dispatched function) should be of short duration relative to the granularity of the scheduling you require.  For example, a dispatched function that ties up the processor for 100 ms when there are tasks to be dispatched every 20 ms would not produce desired results.

When you define a Scheduled Task (using the constructor) it is automatically added to the Dispatcher's task list.  If any tasks are to be dispatched simultaneously, the most recently added one will be dispatched first.  In other words, they are checked in reverse order to how they were constructed.

If 'next' is NEVER that takes precedence over any iterations remaining and the task will not be dispatched.  If period is ONESHOT that will also override any iterations remaining.

SchedTask is a derived class of SchedBase.  SchedTaskT is another derived class.  It differs from SchedTask in that it allows the dispatched function to receive a parameter of virtually any type.

The usage of SchedTaskT is similar to that of SchedTask.  Here is the syntax:

   SchedTaskT<someType> TaskName(next, period, {iterations, } function, parameter);

So the following are valid calls to the constructor.  The argument within <> is the type of parameter to be passed.

   void myFunc(int); // forward declaration required by compiler
   SchedTaskT<int> TaskName(NEVER, ONESHOT, 5, myFunc, 6);     // pass the int value 6 to myFunc when dispatched - it won't be dispatched until 'next' is changed from NEVER in the sketch using setNext()

   void myFunc(int);
   SchedTastT<> TaskName(NEVER, ONESHOT, 5, myFunc, 6);        // same as above, type int is implied by default

   void myFunc(int&);
   SchedTaskT<int&> TaskName(1000, 500, myFunc, rMyInt);       // pass a reference to an int to myFunc when dispatched

   void myFunc(String);
   SchedTaskT<String> TaskName (1000, 500, myFunc, myString);  // pass the String object myString

See Example 6 for examples of other types.

Because SchedTask does not make use of interrupts, your executing code is not interrupted by the Dispatcher.  This means you can modify the parameters (members) of any Scheduled Task on the fly, including the current one.  In other words, a Scheduled Task can even modify itself which will influence future dispatching.

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

Thus a Scheduled Task defined (constructed) like this:

   SchedTaskT<int> TaskName (NEVER, ONESHOT, myFunc, 6); // placeholder task due to NEVER

Could be altered at runtime with:

   TaskName.setNext(60000UL); // dispatch in 1 minute
   TaskName.setPeriod(1000);  // dispatch every second
   TaskName.setIterations(4); // dispatch only 4 times, then go dormant
   TaskName.setParm(5);       // change the parm to be passed to 5
   TaskName.setFunc(myNewFunc); // even alter the function associated with this Scheduled Task object

This will result in a new function (myNewFunc) to be dispatched with the parameter 5 in 60 seconds and every 1 second thereafter, 4 times.

A currently active Scheduled Task (including the one executing) can be 'killed' (prevented from being further dispatched) like this:

   TaskName.setNext(NEVER);

There are default constructors which can be called like this:

   SchedTask SomeTask;
   SchedTaskT SomeTaskT;

In order for these scheduled tasks to be useful you will need to update their parameters using some of the member functions above.

********** ADVANCED TOPICS *************************

The following information may be useful in more highly complex sketches.

There is a getFunc() member function provided for the SchedTask class.

   void(*pF)() = Task.getFunc();

Pointers to SchedTask objects work as you might expect:

   SchedTask* p = &Task;
   p->setFunc(&func);
   void(*pF)() = p->getFunc();

Pointers to the base class SchedBase will invoke virtual functions:

   SchedBase* p = &Task;
   p->setFunc(&func);
   void(*pF)() = p->getFunc();

However, there are some differences for SchedTaskT objects.

The name of the function to obtain the function name is slightly different:

   void(*pF)(sometype) = Task.getFuncT(); // note the 'T' in getFuncT

The member functions setFunc() and setFuncT() are equivalent.

Invoking the basic member functions through a pointer of type SchedBase* using polymorphism will work as expected.  However, calls to setFunc(), setFuncT() and getFuncT() are not supported.  To access these member functions the pointer must dereference a SchedTaskT object.

It may be useful for a dispatched function to be able to access a pointer to its Task object.  For example, a particular function may be dispatched from several SchedTaskT<> objects and the function needs to operate on them individually.  One method is to have the passed parameter be a pointer to the object itself.  It's implemented like this.

   void func(SchedBase*);
   void func2(SchedBase*);
   SchedTaskT<SchedBase*> Task (NOW, 100, func, &Task); // self address passed but as pointer to base

   ...

   void func(SchedBase* p) {
      p->setNext(2000); // polymorphism works for basic member functions
      unsigned long period = p->getPeriod(); // example

      reinterpret_cast<SchedTaskT<SchedBase*>*>(p)->setFuncT(&func2); // see note below
      reinterpret_cast<SchedTaskTptr>(p)->setFuncT(&func2); // simpler form using typedef

      void(*pFunc)(SchedBase*) = reinterpret_cast<SchedTaskT<SchedBase*>*>(p)->getFuncT(); // see note below
      void(*pFunc)(SchedBase*) = reinterpret_cast<SchedTaskTptr>(p)->getFuncT(); // simpler form using typedef
   }

In the case of SchedTaskT polymorphism is not supported for setFuncT() and getFunc().  Hence the trick above.

********** MINIMUM REQUIREMENTS *************************

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

   void myTask1() {...;}

   void myTask2(long i) {...;}

   void myTask3() {...;}

************ END DETAILED DESCRIPTION ********************************

	Change log

		9/23/2014 7:44PM initial coding
		10/01/2020 14:53 revisions for release 1.1.0
		10/13/2020 10:48 getFunc() to return nullptr to remove warning in PlatformIO compiler
		10/13/2020 10:58 changed order of func and parm to remove warning in Platform() compiler
		2021-03-12 11:07:59 changed default constructors
*/

#ifndef SchedTaskT_h
#define SchedTaskT_h

#include <SchedBase.h>

template <typename T=int>
class SchedTaskT : public SchedBase {

	typedef void (*pFuncT)(T);  	// pFuncT is of Type pointer to a function that takes a T argument and returns void
	typedef void (*pFunc)();		// pFunct is of Type pointer to a function that takes no parms and returns void

	public:
		SchedTaskT(unsigned long next, unsigned long period, pFuncT fun, T arg); // constructor w/ parameter to pass
		SchedTaskT(unsigned long next, unsigned long period); // constructor with only next and period
		SchedTaskT(unsigned long next, unsigned long period, pFuncT); // constructor with func, no parameter
		SchedTaskT(unsigned long next = NEVER, unsigned long period = ONESHOT, long iterations = 0UL, pFuncT fun = nullptr, T arg = 0); // default constructor
		SchedTaskT(unsigned long next, unsigned long period, long iterations, pFuncT fun);
		SchedTaskT(unsigned long next, unsigned long period, long iterations); // constructor with next, period, iterations
		~SchedTaskT();														// destructor

		void setFunc(pFuncT pF) {func = pF;}						// set new function pointer
		void setFuncT(pFuncT pF) {func = pF;}						// alternate
		pFuncT getFuncT() {return func;}								// return the function pointer
		void setParm(T aType) {parm = aType;}						// set the parameter to pass to the function
		T getParm() {return parm;}										// get the parameter to pass to the function

	private:

		pFuncT func;														// address of function to call with parameter of type T
		T parm;																// parameter of type T to be passed to dispatched task

		void setFunc(pFunc pF) {;} 									// overrides pure virtual in base so this class not abstract
		pFunc getFunc() {return nullptr;}							// overrides pure virtual in base so this class not abstract

		virtual void callFunc() {func(parm);}						// call the function with parameter on behalf of dispatcher
		virtual bool checkFunc() {return func != NULL;}			// whether func is non-NULL
};

typedef SchedTaskT<SchedBase*>* SchedTaskTptr;					// used for pointer to SchedTaskT object

// constructor templates
template <typename T> SchedTaskT<T>::SchedTaskT (unsigned long nxt, unsigned long intval, pFuncT pFnc, T arg) : SchedBase(nxt, intval), func(pFnc), parm(arg) {} // constructor template
template <typename T> SchedTaskT<T>::SchedTaskT (unsigned long nxt, unsigned long intval) : SchedBase (nxt, intval) {}
template <typename T> SchedTaskT<T>::SchedTaskT (unsigned long nxt, unsigned long intval, pFuncT pFnc) : SchedBase (nxt, intval), func(pFnc) {}
template <typename T> SchedTaskT<T>::SchedTaskT (unsigned long nxt, unsigned long intval, long iters, pFuncT pFnc, T arg) : func(pFnc), parm(arg) {} // default constructor template
template <typename T> SchedTaskT<T>::SchedTaskT (unsigned long nxt, unsigned long intval, long iters, pFuncT pFnc) : SchedBase(nxt, intval, iters), func(pFnc) {} // constructor template
template <typename T> SchedTaskT<T>::SchedTaskT (unsigned long nxt, unsigned long intval, long iters) {}
template <typename T> SchedTaskT<T>::~SchedTaskT() {;}													// destructor

#endif

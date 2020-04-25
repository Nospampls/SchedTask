/*
	SchedTaskT.h - header file for the SchedTaskT template class, derived from SchedBase.  
	
	SchedTaskT is a class template.
	
	This class constructs an object defining a task to be dispatched periodically that receives a parameter.
	
	(See also SchedTask.h which dispatches a task without passing a parameter.)
	
	The constructor takes four positional parameters.
	
	Syntax:
	
	SchedTaskT<T> taskName (next, period, function, parameter);
		where:
			taskName is arbitrary but must be unique
			next is when to dispatch function in the future (in milliseconds) (unsigned long)
			period is how often to dispatch function (in milliseconds) (unsigned long)
			function is the name of the  function to be dispatched that takes a parameter of type T and returns void
			parameter is what to pass to the dispatched function (of type T)
			
	T will default to int.  Therefore these are equivalent:
	
		SchedTaskT<int> ...
		SchedTaskT<> ...
			
	The dispatched function needs to take the form:
	
		void func(T parameter) {...}  // where T is the type of the parameter passed
	
	'Next' can be NOW (0) or NEVER (-1). If 'next' is NEVER it can be changed at run time with a call to the member function setNext(unsigned long newNext).

	The period can be set to ONESHOT (-1) in which case the function will be called only once.  It can be changed with a call to the member function setPeriod(unsigned long newPeriod).
		
	You create instances of the class in your main .ino sketch, before setup();
	
	If you create an instance of the class using the default constructor (i.e. no arguments) then you must use member function calls to alter it at run time.
	
		taskName.setNext(someNewNext);
		taskName.setPeriod(someNewPeriod);
		taskName.setFunc(someOtherFunction);
		taskName.setParm
	
	consts NOW, NEVER, ONESHOT are defined in SchedBase.h
	
	The functions to dispatch must be 'forward declared' before the SchedTaskT objects are constructed to avoid compiler errors.  For example,
	
		void pan(int);
		void once(int);
		void never(int);
	
	Examples:
	
		int param 5;
		
		SchedTaskT<int> panTask(0, 100, pan, param); 	// function 'pan' will get dispatched immediately and every 100 ms thereafter and be passed 'param'
		SchedTaskT<> panTask(NOW, 100, pan, param);		// same as above
		
		SchedTaskT<> panTask(1000, 100, pan, param); 	// function 'pan' will get dispatched 1 sec from now and every 100 ms thereafter and be passed 'param'
		
		SchedTaskT<> oneShot(1000, 0, once, param);		// function 'once' will get dispatched 1 sec from now and never again
		SchedTaskT<> oneShot(1000, ONESHOT, once, param);	// same as above
		
		SchedTaskT<> notYet(NEVER, ONESHOT, never, param);	// function 'never' will not get dispatched (yet)
			use for example:
				notYet.setPeriod(5000); 						// modify the period to 5 sec
				and:
				notYet.setNext(10000);		   				// dispatch in 10 sec
				and:
				notYet.setFunc(funcName);						// when it's time to dispatch, dispatch funcName	
				and
				notYet.setArg(param + 10);						// change the parameter to pass
				
		SchedTaskT<> taskName();								// use default constructor
		SchedTaskT<int> taskName(NEVER, ONESHOT, NULL);	// equivalent to using default constructor
			use the following member functions to allow dispatching
				taskName.setNext(someNext)'
				taskName.setPeriod(somePeriod); 				//only necessary if default of ONESHOT is not desired
				taskName.setFunc(anotherFunc);
				taskName.setParm(someParm);
				
	To pass other types (i.e. not integers), use this as a model:
	
		double pi = 3.14;
		void oneTime(double);
		
		SchedTaskT<double> dubTask(1000UL, ONESHOT, oneTime, pi);	// the parameter 'pi' will be passed to the function 'oneTime' in one second
		
		The supported types for the parameter to be passed are:
		
			int
			double
			String
			int&
			double&
			String&		
			
	To call member functions you generally need to use the name of the object (such as panTask in some examples above). 
	
		panTask.setNext(200);
		
	However, when you want to dispatch a task	that is common to several scheduled tasks 
	and therefore does not know the name of the object, you can use this form:
	
		SchedTaskT<SchedBase *> taskName(next, period, func, &taskName);
		
		The dispatched task 'func' will be passed a pointer to the base class SchedBase on which  member functions can be called.  
		To call member functions (for the next dispatch) in such a schedule task you can do the following:
	
		void func(SchedBase* pBase) {
			pBase->setFunc(someNewFunc);
			pBase->setNext(someNewNext);
			pBase->setPeriod(someNewPeriod);
		}
		
	You must run the dispatcher in loop():
	
		SchedBase::dispatcher();					// note that no particular object is specified
		
	See SchedBase.h for a list of public member functions.
	
	See the included Example_6.ino for other examples.
	
	changes
		9/23/2014 7:44PM initial coding
		4/24/2020 11:18AM updates to comments

*/

#ifndef SchedTaskT_h
#define SchedTaskT_h

#include <SchedBase.h>

template <typename T> class SchedTaskT : public SchedBase {

	typedef void (*pFun)(T);

	public:
		
		SchedTaskT(unsigned long next, unsigned long period, void (*func)(), T); // constructor w/ parameter to pass
		SchedTaskT();														// default constructor declaration

		virtual void setFunc(void (*fnc)) {func = fnc;}			// set the function to dispatch
		virtual pFunc getFunc() {return(func);}					// get the function pointer
		void setParm(T aType) {parm = aType;}						// set the parameter to pass to the function
		T getParm() {return parm;}										// get the parameter to pass to the function

		
	protected:
	
		T parm;																// parameter of type T to be passed to dispatched task
		pFun func;															// address of function to call with parameter of type T
		virtual void callFunc() {func(parm);}						// call the function with parameter
};

template <typename T> SchedTaskT<T>::SchedTaskT (unsigned long nxt, unsigned long intval, void (*fnc)(), T arg) : SchedBase(nxt, intval), func(fnc), parm(arg) { // constructor definition

	SchedBase::addTask(this);											// add this task to the list to be dispatched	

}

template <typename T> SchedTaskT<T>::SchedTaskT() : SchedBase(NEVER, ONESHOT), func(NULL) {	// default constructor
	
	SchedBase::addTask(this);	
}

#endif
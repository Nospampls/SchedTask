# SchedTask
Task Scheduler Library for Arduino

The Task Scheduler Library simulates multi-tasking without the use of interrupts, enabling your sketch to handle multiple asynchronous tasks simultaneously. For example, you can easily blink two LEDs with different durations and periods at the same time.

The library is easy to use and includes examples. Only basic C++ programming skills are needed to take advantage of it.

A detailed tutorial on YouTube provides all you need to know to get started. It presents several example sketches that illustrate various ways to use the library. It deliberately starts out very basic in order to enable even beginner programmers to use it.

https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

If you would like to show your appreciation for this library you can securely make a small donation here:

[![Donate](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=A2J54W4JEHZ6C)

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

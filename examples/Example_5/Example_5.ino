// Example_5 - illustrate various methods of scheduling tasks plus the member functions

/*
	For the complete series of tutorials see
	https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

	For this example see
	https://www.youtube.com/watch?v=wWlGXLyDJOs

	A template in the ExampleConstants header file is used to support the following method for Serial output to the monitor:

		Serial << "\nHello World!" << someVariable << ...;

Change Log

	4/16/2020 11:59AM Initial Release
	05/17/2020 20:54 moved ExampleConstants.h
	05/18/2020 10:15 remove extraneous comment & added default constructor example
*/

const char CAPTION[] = "Example 5 scheduling and member functions";

#include <ExampleConstants.h>										// contains various constants used to control the sketch behavior
#include <SchedTask.h>												// Task Scheduler header file

int runCount = 3;

// required declarations before use by SchedTask constructors
void computeSum();
void computeTotal();
void calculate();
void doSomething();
void doSomethingElse();

/*
The SchedTask constructor takes 3 parameters:

	Next -- the future time in milliseconds to dispatch the function/task

		a value of 0 (NOW) means immediately
		a value of -1 (NEVER) means do not dispatch; placeholder task

	Period -- the interval between dispatches in milliseconds

		a value of 0 (ONESHOT) means only once

	Function -- the function to dispatch

		a value of NULL means do not dispatch; placeholder task

*/

// Examples follow

SchedTask Task1 (0, 1000, doSomething);						// dispatch doSomething immediately and every second thereafter
SchedTask Task2 (NOW, 1000, doSomething);						// same thing

SchedTask Task3 (50, 1000, doSomething);						// dispatch doSomething in 50 ms and every 1000 ms thereafter

SchedTask Task4 (-1, 1000, doSomething);						// never dispatch doSomething (placeholder task definition)
SchedTask Task5 (NEVER, 1000, doSomething);					// same thing

SchedTask Task6 (5000, 0, doSomething);						// dispatch doSomething in 5000 ms and only once
SchedTask Task7 (5000, ONESHOT, doSomething);				// same thing

SchedTask Task8 (NEVER, ONESHOT, NULL);						// function not specified, placeholder for later

SchedTask RealTask (5000, ONESHOT, computeSum);				// dispatch computeSum in 5 seconds

SchedTask MyTask (7000, 2500, computeTotal);					// dispatch computeTotal in 7 sec and every 2.5 sec thereafter

SchedTask Another (15000, ONESHOT, calculate);				// dispatch calculate in 15 sec only once

SchedTask Final (NEVER, ONESHOT, NULL);						// placeholder task awaiting modification (NULL function parameter)

SchedTask NoParms;													// use this form for default constructor; you must use member functions to specify parameters

/********************  Setup() **************************/
void setup() {

	Serial.begin(UART_SPEED);										// init the Monitor window
	Serial << "\n*** SchedTask " << CAPTION << " ***\n";	// Welcome message to monitor

// getNext
	Serial << "\nTask3 next: " << Task3.getNext();			// get the Next time to dispatch

// setNext
	Task3.setNext(2000);												// alter the Next time to dispatch

// setPeriod
	Task3.setPeriod(100);											// alter the period

// getNext, getPeriod
	Serial << "\nTask3 next: " << Task3.getNext() << ", Task3 period: " << Task3.getPeriod();	// get the next time to dispatch, Task3

// getNext, getPeriod
	Serial << "\nTask8 next: " << (int)Task8.getNext() << " (NEVER), Task8 period: " << Task8.getPeriod() << " (ONESHOT)"; // get the next time to dispatch & period, Task8

// setFunc
	Task3.setFunc(doSomethingElse);								// set a new function to dispatch

// getFunc
	if (Task3.getFunc() == doSomethingElse) {Serial << "\nTrue";}	// confirm the new function

// getTaskCount
	Serial << "\nTask count: " << Task1.getTaskCount() << "\n";	// show the task count

}

/******************* Loop() ********************************/

void loop() {

	SchedBase::dispatcher();										// run the dispatcher continuously
}


/********************* Functions ************************************/

/********************** computeSum ***************************************/
void computeSum() {

	Serial << "\n" << millis() << " - Task1 runs once after 5 seconds.\n";
}

/********************** computeTotal ***************************************/

void computeTotal() {												// to run 3x only

	if (runCount) {
		Serial << "\n" << millis() << " - MyTask runs every " << MyTask.getPeriod() << " ms after 7 seconds, 3 times.";
		if (!--runCount) Serial << "\n";
	}
	else {
		MyTask.setNext(NEVER);										// disable the task so it no longer gets dispatched
	}
}

/********************** calculate ***************************************/

void calculate() {

	Serial << "\n" << millis() << " - Another will schedule Final to run 1500 ms from now.";

	Final.setNext(1500);												// dispatch Task4 in 1.5 seconds
	Final.setFunc(task4);											// specify which task to dispatch
}

/********************** task4 ***************************************/

void task4() {

	Serial << "\n" << millis() << " - Final (task4) was scheduled by Another (calculate)\n";	// let us know it ran
}

/********************** doSomething **************************************/

void doSomething() {;}												// just a do nothing task

/********************** doSomethingElse **************************************/

void doSomethingElse() {

}

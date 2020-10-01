// Example_7 - How to insert a 'delay' into code

/*
	For the complete series of tutorials see
	https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

	For this example see
	https://www.youtube.com/watch?v=wWlGXLyDJOs

	A requirement for using SchedTask is that the dispatcher must run continuously in loop() and not be interrupted for long.
	In other words, each dispatched task must complete its work quickly and relinquish control.

	Thus the use of delay() or loop counters in a dispatched task is not appropriate because that would prevent the dispatcher from
	noticing that another task is due for dispatch during that delay.

	This sketch illustrates how to accomplish a delay using the SchedTask mechanism.
	In essence a dispatched task schedules another task to finish its work at a later time.

	If necessary for continuity, SchedTaskT could be used to pass a parameter to each successive function.

Change Log

	4/16/2020 11:59AM Initial Release
	05/17/2020 20:54 moved ExampleConstants.h
	09/30/2020 18:03 removed SoftWareSerial
*/

const char CAPTION[] = "Example 7 delay";

#include <ExampleConstants.h>									// contains various constants used to control the sketch behavior
#include <SchedTask.h>

// function declarations required before reference in SchedTask constructors
void start();
void partTwo();
void start2();

// Method 1
SchedTask StartTask (100, ONESHOT, start);				// dispatch the first part in 100 ms
SchedTask ContinueTask (NEVER, ONESHOT, partTwo);		// do not dispatch the continuation yet

// Method 2
SchedTask StartTask2 (2000, ONESHOT, start2);			// only use one Task object

/********************  Setup() **************************/

void setup() {

	Serial.begin(UART_SPEED);										// init the Monitor window
	Serial << "\n*** SchedTask " << CAPTION << " ***\n";	// Welcome message to monitor
}

/******************* Loop() ********************************/

void loop() {
	SchedBase::dispatcher();										// run the dispatcher continuously
}

/********************* Functions ************************************/

void start() {															// begins the work
	Serial << "\n" << millis() <<" first part of the task";
	ContinueTask.setNext(1000);									// delay 1 sec before continuing
}

void partTwo() {														// continues the work
	Serial << "\n" << millis() << " second part of the task\n";

// could schedule yet another part of the work here
}

void start2() {
	Serial << "\n" << millis() << " first part of the task, second method";

	StartTask2.setNext(1500);										// delay 1.5 sec before continuing
	StartTask2.setFunc(partTwo);									// when continuing, use partTwo() function
}

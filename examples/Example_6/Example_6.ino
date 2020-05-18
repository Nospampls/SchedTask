// Example_6 - illustrating various Types passed as parameters to a dispatched task/function

/*
	This sketch provides examples of various types that can be passed to dispatched functions.

	Notice that passing a vector is one way to pass multiple parameters (of the same Type) to a dispatched function.

	For the complete series of tutorials see
	https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

	For this example see
	https://www.youtube.com/watch?v=wWlGXLyDJOs

Change Log

	4/18/2020 8:19PM Initial Release
	05/17/2020 20:54 moved ExampleConstants.h
*/

const char CAPTION[] = "Example 6 Types";

#include <ExampleConstants.h>										// contains various constants used to control the sketch behavior
#include <SoftwareSerial.h>
#include <SchedTaskT.h>

// parms used to pass to dispatched functions
int parm1 = 9;															// integer
int& rParm1 = parm1;													// int reference
double duble = 3.14;													// double
String s = "Hello World";											// String object
int arr[3] = {3, 2, 1};												// array of ints
SchedBase* schedArr[2];												// array of SchedBase pointers

// function declarations required before reference in SchedTaskT constructors
void task1(int);
void task2(int&);
void task3(double);
void task4(String);
void task5(SchedBase*);
void task6(int*);
void task7(SchedBase**);

// Examples of passing various Types of parameters to dispatched tasks

SchedTaskT<int> Task1 (1000, ONESHOT, task1, parm1);		// pass an int

SchedTaskT<int&> Task2 (1000 + INTERVAL, ONESHOT, task2, rParm1); // pass ref to int

SchedTaskT<double> Task3 (1000 + 2*INTERVAL, ONESHOT, task3, duble);	// pass a double

SchedTaskT<String> Task4 (1000 + 3*INTERVAL, ONESHOT, task4, s);	// pass a String

SchedTaskT<SchedBase*> Task5 (1000 +4*INTERVAL, ONESHOT, task5, &Task5); // pass pointer to the SchedBase (base class)

SchedTaskT<int*> Task6 (1000 + 5*INTERVAL, ONESHOT, task6, arr);	// pass an array of ints

SchedTaskT<SchedBase**> Task7 (1000 + 6*INTERVAL, ONESHOT, task7, schedArr);	// pass an array of SchedTask pointers

/********************  Setup() **************************/

void setup() {

	Serial.begin(UART_SPEED);										// init the Monitor window
	Serial << "\n*** SchedTask " << CAPTION << " ***\n";	// Welcome message to monitor

	schedArr[1] = &Task6;											// init that SchedTask
}

/******************* Loop() ********************************/

void loop() {
	SchedBase::dispatcher();										// run the dispatcher continuously
}

/********************* Functions ************************************/

void task1(int i) {
	Serial << "\nint " << i << " was passed to task1";
}

void task2(int& rInt) {
	Serial << "\nint& " << rInt << " was passed to task2";
}

void task3(double dub) {
	Serial << "\ndouble " << dub << " was passed to task3";
}

void task4(String s) {
	Serial << "\n" << s << " was passed to task4";
}

void task5(SchedBase* pBase) {
	Serial << "\npointer to SchedBase was passed to task5";

	pBase->setPeriod(200);												// use pointer to scheduled task to alter its period
	Serial << "\nperiod = " << pBase->getPeriod() << " (200 now because it was changed)";		// example of calling member function of SchedBase
	pBase->setFunc(&task6);												// use pointer to scheduled task to alter the function to dispatch
}

void task6(int* arr) {
	Serial << "\narr[0] = " << arr[0] << " was passed to task6";
}

void task7(SchedBase** schedArr) {

	Serial << "\nschedArr[1] next = " << (int)schedArr[1]->getNext() << " was passed to task7 (-1 now because it was a one-shot Task)";
}

void task9(int) {														// get and set the parameter

	int parm1 = Task1.getParm();
	Task1.setParm(parm1 + 5);

}

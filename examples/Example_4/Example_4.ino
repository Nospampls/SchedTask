// Example_4 - blink 2 LEDs with separate durations and periods

/*
	This sketch introduces SchedTaskT<> which allows a parameter to be passed to the dispatched function.
	It illustrates the ease with which two LEDs can be independently controlled.  Each has a different duration and period.
	Note: by passing the LED pin number only one function is needed to turn them on, and only one to turn them off.

	For the complete series of tutorials see
	https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

	For this example see
	see https://www.youtube.com/watch?v=eIkzXibPwlA

Change Log

	4/16/2020 11:59AM Initial Release
	05/17/2020 20:54 moved ExampleConstants.h
	09/16/2020 16:32 removed SoftwareSerial.h
*/
const char CAPTION[] = "Example 4 Blink 2 LEDs";

#include <ExampleConstants.h>										// contains various constants used to control the sketch behavior
#include <SchedTaskT.h>												// NEWLY ADDED

// required declarations before use by SchedTask constructors
void turnOnLED(int);
void turnOffLED(int);

// in the following tasks I have arbitrarily delayed and staggered their first dispatches.
// this is partly to illustrate how to do so but also to allow setup() to finish it's initialization, including printing to the monitor

// the 1st LED tasks
SchedTaskT<int> OnTask1 (10, PERIOD1, turnOnLED, LED_PIN_1);				// dispatch turnOnLED after 10 ms and pass it the LED pin to use
SchedTaskT<int> OffTask1 (DURATION1+10, PERIOD1, turnOffLED, LED_PIN_1);// dispatch turnOffLED for LED 1

// the 2nd LED tasks
SchedTaskT<int> OnTask2 (50, PERIOD2 , turnOnLED, LED_PIN_2);				// dispatch turnOnLED after 50 ms and pass it the LED pin to use
SchedTaskT<int> OffTask2(DURATION2+50, PERIOD2, turnOffLED, LED_PIN_2);	// dispatch turnOffLED for LED 2 with a different period

/********************  Setup() **************************/

void setup() {

	Serial.begin(UART_SPEED);										// init the Monitor window
	Serial << "\n*** SchedTask " << CAPTION << " ***\n";	// Welcome message to monitor

	pinMode(LED_PIN_1, OUTPUT);									// initialize the hardware pins
	pinMode(LED_PIN_2, OUTPUT);

}

/******************* Loop() ********************************/

void loop() {

	SchedBase::dispatcher();										// run the dispatcher continuously
}

/********************* Functions ************************************/

// turn a LED on

void turnOnLED(int pin) {

	digitalWrite(pin, ON);											// turn on the LED
	if (OUTPUT_ENABLED) Serial << "\n" << millis() << " pin " << pin << " on";

}

// turn a LED off

void turnOffLED(int pin) {

	digitalWrite(pin, OFF);											// turn off the LED
	if (OUTPUT_ENABLED) Serial << "\n" << millis() << " pin " << pin << " off";

}

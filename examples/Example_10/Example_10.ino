// Example_10 - blink LED in bursts
//				  - demonstrates how to use iterations to limit dispatches

/*
	This Example illustrates the use of iterations.
	The onboard LED is blinked in bursts of 5 flashes, 6 times

	When 'iterations' are specified, the dispatcher limits the periodic dispatching of the scheduled task.

	For example, the following produce the same results:

	SchedTask RepeatTask (NOW, 1000, 4, someTask); // dispatch immediately, every 1 second, 4 times (only)

	SchedTask RepeatTask (NOW, 1000, someTask);	// iterations not yet specified
	...
	RepeatTask.setIterations(4);	// this runs in setup() or some called or scheduled function


	For the complete series of tutorials see
	https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

Change Log
	08/22/2020 20:58 Initial Release
	09/30/2020 19:07 remove SoftwareSerial
*/

const char CAPTION[] = "Example 10 Blink LED in bursts";

#include <ExampleConstants.h>										// contains various constants used to control the sketch behavior
#include <SchedTask.h>												// include the SchedTask header file

// forward declarations required by SchedTask constructors
void turnOffLED();
void pulseLED();
void sendBurst();

SchedTask OffTask (ON_TIME, ONESHOT, turnOffLED);			// the Task to turn off the LED
SchedTask PulseLED (NEVER, ON_TIME + OFF_TIME, pulseLED); // a Task to turn the LED on and off (once)
SchedTask SendBurst (NOW, 3000, 6, sendBurst);			// a Task to run every 3 seconds, 6 times (it sends a burst of 5 flashes)

/********************  Setup() **************************/
void setup() {

	Serial.begin(UART_SPEED);										// init the Monitor window
	Serial << "\n*** SchedTask " << CAPTION << " ***\n";	// Welcome message to monitor

	pinMode(LED_PIN, OUTPUT);										// initialize the hardware pin for LED
}

/******************* Loop() ********************************/

void loop() {
	SchedBase::dispatcher();										// dispatch any tasks due
}

/********************* Functions ************************************/

// turn the LED on (called function)
void turnOnLED() {
	digitalWrite(LED_PIN, ON);										// turn on the LED
	if (OUTPUT_ENABLED) Serial << "\n\n" << millis() << " On";
}

// turn the LED off (scheduled task)
void turnOffLED() {
	digitalWrite(LED_PIN, OFF);									// turn off the LED
	if (OUTPUT_ENABLED) Serial << "\n" << millis() << " Off";
}

// pulse the LED (this is a scheduled task)
void pulseLED() {														// turn the LED on and off (once)
	turnOnLED();														// turn it on now
// Note: we do not use delay() here -- not allowed with SchedTask!
	OffTask.setNext(ON_TIME);										// turn it off later (schedule the task to turn it off)
}

void sendBurst() {													// scheduled Task to send a burst
	PulseLED.setNext(NOW);											// schedule it immediately
	PulseLED.setIterations(5);										// dispatch it 5 times
}

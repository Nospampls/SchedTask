// Example_11 - blink LED (SchedTask method 1), disable after awhile
//				  - demonstrate how to use iterations to limit dispatches

/*
	This method uses one Task to schedule the turn-on function and another for the turn-off function.

	When 'iterations' are specified, the dispatcher limits the periodic dispatching of the scheduled task.

	For example, the following produce the same results:

	SchedTask RepeatTask (NOW, 1000, 4, someTask); // dispatch immediately, every 1 second, 4 times (only)

	SchedTask RepeatTask (NOW, 1000, someTask);	// iterations not yet specified
	...
	RepeatTask.setIterations(4);	// this runs in setup() or some called or scheduled function

	For the complete series of tutorials see
	https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

Change Log
	08/21/2020 13:03 Initial Release
*/

const char CAPTION[] = "Example 11 Blink LED, iterations";

#include <ExampleConstants.h>										// contains various constants used to control the sketch behavior
#include <SchedTask.h>												// include the SchedTask header file
#include <SchedTaskT.h>

void turnOnLED();														// forward declarations
void turnOffLED();													//   required by SchedTask constructors
void killBlinking();
void restartBlinking();

// blink LED 3 times only; illustrates specifying the iterations in the constructor
SchedTask ThreeOns (NOW, 3000, 3, turnOnLED);				// dispatch now and every 3 secs for 3 iterations
SchedTask ThreeOffs (1000, 3000, 3, turnOffLED);			// dispatch in 1 sec and every 3 secs for 3 iterations

// blink LED indefinitely
SchedTask OnTask (10000, 3000, turnOnLED);					// define the turn on task (dispatch in 10 sec, every 3 sec)
SchedTask OffTask (11000, 3000, turnOffLED);					// define the turn off task (dispatch in 11 sec, every 3 sec)

SchedTask KillTask (21000, ONESHOT, killBlinking);			// define task to disable blinking tasks after 21 seconds

SchedTask RestartTask (25000, ONESHOT, restartBlinking); // restart the blinking after 25 seconds; iterations defined at run time

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

// turn the LED on
void turnOnLED() {
	digitalWrite(LED_PIN, ON);										// turn on the LED
	if (OUTPUT_ENABLED) Serial << "\n\n" << millis() << " On";
}

// turn the LED off
void turnOffLED() {
	digitalWrite(LED_PIN, OFF);									// turn off the LED
	if (OUTPUT_ENABLED) Serial << "\n" << millis() << " Off";
}

// disable blinking tasks
void killBlinking() {
	OnTask.setNext(NEVER);											// ensure won't run again
	OffTask.setNext(NEVER);											// ensure won't run again
	if (OUTPUT_ENABLED) Serial << "\n\n" << millis() << " Blinking stopped";
}

// restart blinking, demonstrate iterations
void restartBlinking() {
	if (OUTPUT_ENABLED) Serial << "\n\n" << millis() << " Blinking restarting (3 blinks)";

	OnTask.setNext(NOW);												// dispatch OnTask now
	OnTask.setIterations(3);										// dispatch it 3 times only

	OffTask.setNext(1000);											// dispatch OffTask in 1 sec
	OffTask.setIterations(3);										// dispatch it 3 times only
}

// Example_3 - blink LED (SchedTask method 2)

/*		This method dispatches a task (turnOnLED) to turn the LED on.  turnOnLED then dispatches turnOffLED to turn it off.
		turnOffLED then re-schedules turnOnLED to turn it back on and the cycle repeats.

		This illustrates the ability to control when tasks are dispatched at run time.  The periods can be controlled similarly.

		For the complete series of tutorials see
		https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

		For this example see
		see https://www.youtube.com/watch?v=eIkzXibPwlA


--------_____________--------_____________
   on        off        on        off

Change Log

	4/16/2020 11:58AM Initial Release
	05/17/2020 20:54 moved ExampleConstants.h
	05/18/2020 10:39 corrected example number in caption
	09/16/2020 16:30 remove SoftwareSerial.h
*/

const char CAPTION[] = "Example 3 Blink LED SchedTask method 2";

#include <ExampleConstants.h>										// contains various constants used to control the sketch behavior
#include <SchedTask.h>												// include the SchedTask header file

void turnOnLED();														// forward declarations
void turnOffLED();													//   required by SchedTask constructors

/* REMOVED
SchedTask OnTask (0, 3000, turnOnLED);							// define the turn on task (dispatch now, every 3 sec)
SchedTask OffTask (1000, 3000, turnOffLED);					// define the turn off task (dispatch in 1 sec, every 3 sec)
*/

// NEW
SchedTask OnTask (NOW, ONESHOT, turnOnLED);					// define the turn on task (dispatch now, one time)
SchedTask OffTask (NEVER, ONESHOT, turnOffLED);				// define the turn off task (dispatch never)

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

// NEW
	OffTask.setNext(DURATION);										// schedule turnOffLED after DURATION (1 sec)
}

// turn the LED off

void turnOffLED() {

	digitalWrite(LED_PIN, OFF);									// turn off the LED
	if (OUTPUT_ENABLED) Serial << "\n" << millis() << " Off";

// NEW
	OnTask.setNext(PERIOD-DURATION);								// schedule turnOnLED after PERIOD-DURATION (2 sec)
}

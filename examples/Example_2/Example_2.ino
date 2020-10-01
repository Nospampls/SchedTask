// Example_2 - blink LED (SchedTask method 1)

/*
	This method uses one Task to schedule the turn-on function and another for the turn-off function.

	For the complete series of tutorials see
	https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

	For this example see
	see https://www.youtube.com/watch?v=nZHBbSkVUSo&t=7s


--------_____________--------_____________
   on        off        on        off


Change Log

	4/16/2020 11:57AM Initial Release
	05/17/2020 20:54 moved ExampleConstants.h
	09/16/2020 16:28 remove SoftwareSerial.h
*/

const char CAPTION[] = "Example 2 Blink LED SchedTask method 1";

#include <ExampleConstants.h>										// contains various constants used to control the sketch behavior

// NEW
#include <SchedTask.h>												// include the SchedTask header file

// NEW
void turnOnLED();														// forward declarations
void turnOffLED();													//   required by SchedTask constructors

// NEW
SchedTask OnTask (0, 3000, turnOnLED);							// define the turn on task (dispatch now, every 3 sec)
SchedTask OffTask (1000, 3000, turnOffLED);					// define the turn off task (dispatch in 1 sec, every 3 sec)


/********************  Setup() **************************/
void setup() {

	Serial.begin(UART_SPEED);										// init the Monitor window
	Serial << "\n*** SchedTask " << CAPTION << " ***\n";	// Welcome message to monitor

	pinMode(LED_PIN, OUTPUT);										// initialize the hardware pin for LED
}

/******************* Loop() ********************************/

void loop() {

/* REMOVED
	turnOnLED();														// turn on the LED
	delay (1000);														// leave it on for 1 sec
	turnOffLED();														// turn off the LED
	delay (2000);														// leave it off for 2 sec
*/

// NEW
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

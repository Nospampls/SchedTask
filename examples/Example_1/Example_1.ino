// Example_1 - blink LED (classic method - no use of SchedTask Task Scheduler yet)
/*		Use of delay() in loop()

	 For the complete series of tutorials see
	 https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

	 For this example see
	 see https://www.youtube.com/watch?v=nZHBbSkVUSo&t=7s

/*
Change Log

	4/16/2020 11:57AM Initial Release
	05/17/2020 20:54 moved ExampleConstants.h
	09/16/2020 16:25 remove SoftwareSerial.h
*/

const char CAPTION[] = "Example 1 Blink LED classic method";

#include <ExampleConstants.h>										// contains various constants used to control the sketch behavior

/********************  Setup() **************************/
void setup() {

	Serial.begin(UART_SPEED);										// init the Monitor window
	Serial << "\n*** SchedTask " << CAPTION << " ***\n";	// Welcome message to monitor

	pinMode(LED_PIN, OUTPUT);										// initialize the hardware pin
}

/******************* Loop() ********************************/

void loop() {

	turnOnLED();														// turn on the LED
	delay (1000);														// leave it on for 1 sec
	turnOffLED();														// turn off the LED
	delay (2000);														// leave it off for 2 sec

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

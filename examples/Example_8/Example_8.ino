// Example_8 - Morse Code to one LED

/*

	For the complete series of tutorials see
	https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

	For this example see
	https://www.youtube.com/watch?v=SbPebaNopFA

	This example illustrates using SchedTask to output Morse code to the onboard LED.  The code string is input by the user.
	The output rate is defined in constants.h WPM in words per minute.

Change Log

	4/16/2020 11:59AM Initial Release
	05/17/2020 20:54 moved ExampleConstants.h
	09/16/2020 16:16 changed type of pCode, remove SoftwareSerial.h
*/

const char CAPTION[] = "Example 8 Morse Code 1 LED";

#include <ExampleConstants.h>										// contains various constants used to control the sketch behavior
#include <SchedTask.h>

// forward declarations needed by SchedTask constructors
void continueOutput();
void turnOffLED();

int ledPin = LED_PIN;												// output pin for LED

SchedTask ContinueTask (NEVER, ONESHOT, continueOutput);	// placeholder scheduled task to continue output
SchedTask OffTask (NEVER, ONESHOT, turnOffLED);				// placeholder scheduled task to turn off LED

String sentence;														// store for user's sentence
int sentenceIdx;														// current index into the sentence (next char)

const unsigned int* pCode;																// pointer to current code
int pulseIdx;															// current index into the pulses (next pulse duration)

unsigned long dotDuration = DOT_DURATION;						// duration of one dot (dash is 3x, inter-pulse is 3x, inter-char is 7x)

/********************  Setup() **************************/

void setup() {

	Serial.begin(UART_SPEED);										// init the Monitor window
	Serial << "\n*** SchedTask " << CAPTION << " ***\n";	// Welcome message to monitor

	pinMode (ledPin , OUTPUT); 									// initialize the hardware mode

	readConsole();														// read a sentence from the console
	continueOutput();													// get the first character and start the output
}

/******************* Loop() ********************************/

void loop() {

	SchedBase::dispatcher();										// run the dispatcher continuously
}

/********************* Functions ************************************/

/********************* readConsole **********************************/

void readConsole () {												// read a sentence from the console

	Serial << "\nEnter a sentence (alpha and spaces only)";

	while (!Serial.available()) {} 								// wait for some input available

	while (Serial.available()) {									// loop while still arriving
		delay(3);
		if (Serial.available()) {
			char c = Serial.read();									// read next character
			sentence += c;												// append character to sentence
		}
	}
	sentence.toUpperCase();											// convert to upper case for morse code
	sentenceIdx = 0;													// point to the first char (reset the sentence index)
	pCode = NULL;														// no ptr to morse code yet

	Serial << "\nyou entered:" << sentence.substring(0, sentence.length()-1);
	Serial << "\n" << 1200/dotDuration << " words per minute\n";
}

/******************* continueOutput ***********************/

void continueOutput() {

	char c;

	if (pCode && pulseIdx < pCode[0]+1) {						// is the pulse index valid and more pulses?
		turnOnLED();													// go output the next pulse
	}
	else if (sentenceIdx < sentence.length()-1) {			// any more chars?

		c = getChar();													// get the next one

		if (c == ' ') {												// is it a space?
			ContinueTask.setNext(7 * dotDuration);				// then pause 7 dot durations
		}
		else {															// not a space
			pulseIdx  = 1;												// reset pulse index
			pCode = CODES[c-OFFSET];								// pointer to the morse code for char c (lookup the code)

			turnOnLED();												// go output the first pulse
		}
	}
}

/****************** turnOnLED ****************/

void turnOnLED() {

		digitalWrite(ledPin, ON);									// turn on the LED
		OffTask.setNext(pCode[pulseIdx] * dotDuration);		// turn off LED after length of the pulse	(dot or dash)
}

/**************** turnOffLED *************************/

void turnOffLED() {

	int units;															// interval in number of dot multiples between pulses

	digitalWrite(ledPin, OFF);										// turn off the LED

// compute silence interval after pulse depending on inter-pulse, inter-char, inter-word

	if (pulseIdx == pCode[0]) {									// last pulse of the character?
		units = 3;														// inter-char interval
	}
	else {																// more pulses
		units = 1;														// inter-pulse interval
	}
	pulseIdx++;															// incr the pulse index

	ContinueTask.setNext(units * dotDuration);				// silence before continuing

// now relinquish control to the Dispatcher
}

/***************** getChar ************************/

// return a character from the sentence
// '?' if not valid

char getChar() {

	char c;
	c = sentence.charAt(sentenceIdx++);							// get the next character and bump the index
	if ((c>='A' && c<='Z') || c==' ') return(c);				// return validated char
	else return('?');													// return '?' if invalid
}

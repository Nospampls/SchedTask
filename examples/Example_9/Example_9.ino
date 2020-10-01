// Example_9 - Morse Code to two LEDs
//		Different words per minute to each LED

/*
	For the complete series of tutorials see
	https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

	For this example see
	https://www.youtube.com/watch?v=SbPebaNopFA

	This example illustrates using SchedTaskT to output Morse code to two LEDs simultaneously.  The code strings are input by the user.
	The output rate is defined in constants.h WPM in words per minute.

Change Log

	4/18/2020 20:31 Initial Release
	05/17/2020 20:54 moved ExampleConstants.h
	09/30/2020 19:01 removed SoftwareSerial, pCode changed to const unsigned int
*/

const char CAPTION[] = "Example 9 Morse Code 2 LEDs";

#include <ExampleConstants.h>										// contains various constants used to control the sketch behavior
#include <SchedTaskT.h>

struct blinker; 														// forward reference

typedef blinker* pBlinker;											// pointer to blinker object

struct blinker {														// a blinker contains values pertaining to one of the LEDs

	blinker(int id, int pin, const unsigned long dotDur) {// constructor
		ID = id;															// ID of this blinker (1, 2, ...)
		ledPin = pin;													// output pin for the associated LED
		sentence = "";													// clear the sentence
		sentenceIdx = 0;												// initialize index into sentence
		dotDuration = dotDur;										// duration of one dot in ms
	}
	int ID;																// ID for this blinker
	int ledPin;															// output pin for LED

	SchedTaskT<pBlinker> ContinueTaskT;							// two SchedTaskT objects
	SchedTaskT<pBlinker> OffTaskT;								//   needed by the sketch, default constructors

	String sentence;													// store for user's sentence
	int sentenceIdx;													// current index into the sentence (next char)

	const unsigned int* pCode;										// point to current code
	int pulseIdx;														// current index into the pulses (next pulse duration)

	unsigned long dotDuration;										// duration of one dot (dash is 3x, inter-pulse is 3x, inter-char is 7x)
};
blinker blinker1(1, LED_PIN_1, DOT_DURATION_1);				// define blinker with a LED pin number and dot duration
blinker blinker2(2, LED_PIN_2, DOT_DURATION_2);				// define another blinker with another LED pin number

const int BLINKERS = 2;
pBlinker blinkers[BLINKERS];										// array of blinkers

/********************  Setup() **************************/

void setup() {

	Serial.begin(UART_SPEED);										// init the Monitor window
	Serial << "\n*** SchedTask " << CAPTION << " ***\n";	// Welcome message to monitor

// a couple of blinkers (i.e., 2 LEDs) in an array

	blinkers[0] = &blinker1;
	blinkers[1] = &blinker2;

	for (int i=0; i<BLINKERS; i++) {								// process all blinkers
		pinMode (blinkers[i]->ledPin , OUTPUT); 				// initialize the hardware mode

		blinkers[i]->ContinueTaskT.setFunc(continueOutput);// initialize the sched task to turn on LED
		blinkers[i]->ContinueTaskT.setParm(blinkers[i]);

		blinkers[i]->OffTaskT.setFunc(turnOffLED);			// initialize the sched task to turn off LED
		blinkers[i]->OffTaskT.setParm(blinkers[i]);

		readConsole(blinkers[i]);									// read a sentence from the console
	}
	for (int i=0; i<BLINKERS; i++) {								// loop thru blinkers now that we've read the sentences
		continueOutput(blinkers[i]);								// get the first character and start the output
	}
}

/******************* Loop() ********************************/

void loop() {

	SchedBase::dispatcher();										// run the dispatcher continuously
}

/********************* Functions ************************************/

/********************* readConsole **********************************/

void readConsole (pBlinker pB) {									// read a sentence from the console

	Serial << "\nEnter a sentence (alpha and spaces only) for blinker " << pB->ID;

	while (!Serial.available()) {} 								// wait for some input available

	while (Serial.available()) {									// loop while still arriving
		delay(3);
		if (Serial.available()) {
			char c = Serial.read();
			pB->sentence += c;										// append character to sentence
		}
	}
	pB->sentence.toUpperCase();									// convert to upper case for morse code
	pB->sentenceIdx = 0;												// point to the first char (reset the sentence index)
	pB->pCode = NULL;													// no ptr to morse code yet

	Serial << "\nyou entered:" << pB->sentence.substring(0, pB->sentence.length()-1);
	Serial << "\n" << 1200/pB->dotDuration << " words per minute\n";
}

/******************* continueOutput ***********************/

void continueOutput(pBlinker pB) {

	char c;

	if (pB->pCode && pB->pulseIdx < pB->pCode[0]+1) {		// is the pulse index valid and more pulses?
		turnOnLED(pB);													// go output the next pulse
	}
	else if (pB->sentenceIdx < pB->sentence.length()-1) {	// any more chars?

		c = getChar(pB);												// get the next one

		if (c == ' ') {												// is it a space?
			pB->ContinueTaskT.setNext(7 * pB->dotDuration);	// then pause 7 dot durations
		}
		else {															// not a space
			pB->pulseIdx  = 1;										// reset pulse index
			pB->pCode = CODES[c-OFFSET];							// pointer to the morse code for char c (lookup the code)

			turnOnLED(pB);												// go output the first pulse
		}
	}
}

/****************** turnOnLED ****************/

void turnOnLED(pBlinker pB) {

		digitalWrite(pB->ledPin, ON);								// turn on the LED
		pB->OffTaskT.setNext(pB->pCode[pB->pulseIdx] * pB->dotDuration);	// turn off LED after length of the pulse	(dot or dash)
}

/**************** turnOffLED *************************/

void turnOffLED(pBlinker pB) {

	int units;															// interval in number of dot multiples between pulses

	digitalWrite(pB->ledPin, OFF);								// turn off the LED

// compute silence interval after pulse depending on inter-pulse, inter-char, inter-word

	if (pB->pulseIdx == pB->pCode[0]) {							// last pulse of the character?
		units = 3;														// inter-char interval
	}
	else {																// more pulses
		units = 1;														// inter-pulse interval
	}
	pB->pulseIdx++;													// incr the pulse index
	pB->ContinueTaskT.setNext(units * pB->dotDuration);	// pause before continuing

// now relinquish control to the Dispatcher
}

/***************** getChar ************************/

// return a character from the sentence
// '?' if not valid

char getChar(pBlinker pB) {

	char c;
	c = pB->sentence.charAt(pB->sentenceIdx++);				// get the next character and bump the index
	if ((c>='A' && c<='Z') || c==' ') return(c);				// return validated char
	else return('?');													// return '?' if invalid
}

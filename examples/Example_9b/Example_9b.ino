// Example_9b - Morse Code to two LEDs
/*
	Modification of Example_9 to use iterations.  I think this approach is more straightforward than Example_9.

	Different words per minute to each LED
	Open the Monitor window to be prompted to enter sentences to be output in Morse code

	For the complete series of tutorials see
	https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

	For Example_9 see
	https://www.youtube.com/watch?v=SbPebaNopFA

	This example illustrates using SchedTaskT to output Morse code to two LEDs simultaneously.  The code strings are input by the user.
	The output rate is defined in ExampleConstants.h WPM_1 and WPM_2 in words per minute.

	This Example_8b differs from Example_9 in that it uses iterations.

Change Log

	08/25/2020 10:54 Initial release
*/

const char CAPTION[] = "Example 9b Morse Code 2 LEDs";

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

	SchedTaskT<pBlinker> OutputCharT;							// two SchedTaskT objects
	SchedTaskT<pBlinker> PulseLEDT;
	SchedTaskT<pBlinker> OffTaskT;								//   needed by the sketch, default constructors

	String sentence;													// store for user's sentence
	int sentenceIdx;													// current index into the sentence (next char)

	const unsigned int* pCode;										// point to current code
	unsigned long dotDuration;										// duration of one dot (dash is 3x, inter-pulse is 3x, inter-char is 7x)
};

blinker blinker1(1, LED_PIN_1, DOT_DURATION_1);				// define blinker with a LED pin number and dot duration
blinker blinker2(2, LED_PIN_2, DOT_DURATION_2);				// define another blinker with another LED pin number

const int BLINKERS = 2;
pBlinker blinkers[] = {&blinker1, &blinker2};				// array of blinker pointers

/********************  Setup() **************************/

void setup() {

	Serial.begin(UART_SPEED);										// init the Monitor window
	Serial << "\n*** SchedTask " << CAPTION << " ***\n";	// Welcome message to monitor

	for (int i=0; i<BLINKERS; i++) {								// process all blinkers
		pinMode (blinkers[i]->ledPin , OUTPUT); 				// initialize the hardware mode

		blinkers[i]->OutputCharT.setFunc(outputChar);		// initialize the sched task to turn on LED
		blinkers[i]->OutputCharT.setParm(blinkers[i]);		// pass the blinker pointer

		blinkers[i]->OffTaskT.setFunc(turnOffLED);			// initialize the sched task to turn off LED
		blinkers[i]->OffTaskT.setParm(blinkers[i]);			// pass the blinker pointer

		blinkers[i]->PulseLEDT.setFunc(pulseLED);
		blinkers[i]->PulseLEDT.setParm(blinkers[i]);

		readConsole(blinkers[i]);									// read a sentence from the console
	}
	for (int i=0; i<BLINKERS; i++) {								// loop thru blinkers now that we've read the sentences
		outputChar(blinkers[i]);									// get the first character and start the output
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

/******************* outputChar ***********************/

void outputChar(pBlinker pB) {

	char c;

	if (pB->sentenceIdx < pB->sentence.length()-1) {		// any more chars in sentence?
		char c = getChar(pB);										// get the next one
		if (c == ' ') {												// is it a space?
			pB->OutputCharT.setNext((INTER_WORD-INTER_CHAR) * pB->dotDuration);	// then pause inter word before fetching the next char
		}
		else {															// not a space
			pB->pCode = CODES[c-OFFSET];							// pointer to the morse code for char c (lookup the code)
			pB->PulseLEDT.setNext(NOW);							// start outputting char now
			pB->PulseLEDT.setIterations(pB->pCode[0]);		// iterate the number of dots & dashes in char code
		}
	}
}

/****************** pulseLED *****************/
// this scheduled task is entered once for each dot or dash ('pulse') in the character's code
// the timing of each iteration is adjusted based on length of dot or dash
void pulseLED(pBlinker pB) {
	turnOnLED(pB);														// turn on the LED

// iterations at this point will be the number of dots and dashes in the code minus 1 (the number of iterations remaining)
// pCode[0] is the number of dots and dashes in the code
// so pCode[0]-PulseLED.getIterations() is the index into the code for the next pulse duration
	unsigned long pulseLength = pB->dotDuration * pB->pCode[pB->pCode[0]-pB->PulseLEDT.getIterations()]; // length of pulse fetched from morse code table for char

	pB->OffTaskT.setNext(pulseLength);									// turn off the LED after pulseLength (dot or dash)
	pB->PulseLEDT.setNext(pulseLength + INTER_PULSE * pB->dotDuration);	// re-schedule self for the next pulse On time
	pB->OutputCharT.setNext(pulseLength + INTER_CHAR * pB->dotDuration);// re-schedule the next character output
}

/****************** turnOnLED ****************/

void turnOnLED(pBlinker pB) {
		digitalWrite(pB->ledPin, ON);								// turn on the LED
		pB->OffTaskT.setNext(pB->dotDuration * pB->pCode[1+5-pB->PulseLEDT.getIterations()]); // turn off LED after length of the pulse	(dot or dash)
}

/**************** turnOffLED *************************/

void turnOffLED(pBlinker pB) {
	digitalWrite(pB->ledPin, OFF);								// turn off the LED
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

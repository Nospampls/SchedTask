// Example_8b - Morse Code to one LED
// Modification of Example_8 to use iterations.  I think this is more straightforward than Example_8.

/*
	Open the IDE Monitor window to be prompted to enter a sentence to be output in Morse code.

	For the complete series of tutorials see
	https://www.youtube.com/watch?v=nZHBbSkVUSo&list=PL69rZyCQYu-SrPAZUc2Lj_zsjPLxtI9fv

	For Example_8 see
	https://www.youtube.com/watch?v=SbPebaNopFA

	This example illustrates using SchedTask to output Morse code to the onboard LED.  The code string is input by the user.
	The output rate is defined in ExampleConstants.h WPM in words per minute.

	This Example_8b differs from Example_8 in that it uses iterations.

Change Log
	08/24/2020 20:40 Initial coding
	09/30/2020 18:11 change pCode to const unsigned int, remove SoftwareSerial
*/

const char CAPTION[] = "Example 8b Morse Code 1 LED";

#include <ExampleConstants.h>										// contains various constants used to control the sketch behavior
#include <SchedTask.h>

// forward declarations needed by SchedTask constructors
void outputChar();
void turnOffLED();
void pulseLED();

SchedTask OutputChar (NEVER, ONESHOT, outputChar);			// placeholder scheduled task to output a char
SchedTask OffTask (NEVER, ONESHOT, turnOffLED);				// placeholder scheduled task to turn off LED
SchedTask PulseLED (NEVER, ONESHOT, pulseLED);				// placeholder scheduled task to output a char

int ledPin = LED_PIN;												// output pin for LED
String sentence;														// store for user's sentence
int sentenceIdx;														// current index into the sentence (next char)

const unsigned int* pCode;																// pointer to current code

/********************  Setup() **************************/

void setup() {

	Serial.begin(UART_SPEED);										// init the Monitor window
	Serial << "\n*** SchedTask " << CAPTION << " ***\n";	// Welcome message to monitor

	pinMode (ledPin , OUTPUT); 									// initialize the hardware mode

	readConsole();														// read a sentence from the console
	outputChar();														// start the output
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
		delay(3);														// we can get away with this in setup()
		if (Serial.available()) {
			char c = Serial.read();									// read next character
			sentence += c;												// append character to sentence
		}
	}
	sentence.toUpperCase();											// convert to upper case for morse code
	sentenceIdx = 0;													// point to the first char (reset the sentence index)

	Serial << "\nyou entered:" << sentence.substring(0, sentence.length()-1);
	Serial << "\n" << WPM << " words per minute\n";
}

/******************* outputChar ***********************/

void outputChar() {
	if (sentenceIdx < sentence.length()-1) {					// any more chars in sentence?
		char c = getChar();											// get the next one
		if (c == ' ') {												// is it a space?
			OutputChar.setNext((INTER_WORD-INTER_CHAR) * DOT_DURATION);	// then pause inter word before fetching the next char
		}
		else {															// not a space
			pCode = CODES[c-OFFSET];								// pointer to the morse code for char c (lookup the code)
			PulseLED.setNext(NOW);									// start outputting char now
			PulseLED.setIterations(pCode[0]);					// iterate the number of dots & dashes in char code
		}
	}
}

/****************** pulseLED *****************/
// this scheduled task is entered once for each dot or dash ('pulse') in the character's code
// the timing of each iteration is adjusted based on length of dot or dash

void pulseLED() {
	turnOnLED();														// turn on the LED

// iterations at this point will be the number of dots and dashes remaining in the code (the number of iterations remaining)
// pCode[0] is the total number of dots and dashes in the code
// so pCode[0]-PulseLED.getIterations() is the index into the code for the next pulse duration
	unsigned long pulseLength = DOT_DURATION * pCode[pCode[0]-PulseLED.getIterations()]; // length of pulse fetched from morse code table for char

	OffTask.setNext(pulseLength);									// turn off the LED after pulseLength (dot or dash)
	PulseLED.setNext(pulseLength + INTER_PULSE * DOT_DURATION);	// re-schedule self for the next pulse On time
	OutputChar.setNext(pulseLength + INTER_CHAR * DOT_DURATION);// re-schedule the next character output
}

/****************** turnOnLED ****************/

void turnOnLED() {
		digitalWrite(ledPin, ON);									// turn on the LED
		OffTask.setNext(DOT_DURATION * pCode[1+5-PulseLED.getIterations()]);		// turn off LED after length of the pulse	(dot or dash)
}

/**************** turnOffLED *************************/

void turnOffLED() {
	digitalWrite(ledPin, OFF);										// turn off the LED
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

/*

ExampleConstants.h for SchedTask examples (only)

Change Log

   4/17/2020 2:29PM Initial Release
   7/7/2020 4:12PM make OUTPUT_ENABLED variable not const so user can easily change it in sketch
   08/23/2020 20:48 better yet, make OUTPUT_ENABLED #define so user can override it in sketch
   08/23/2020 20:51 added UL to unsigned long constants to set a good example for users.
   09/16/2020 16:22 remove SoftwareSerial.h

*/
#ifndef ExampleConstants_h
#define ExampleConstants_h

#include <Arduino.h>

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } // allow use of Serial <<

const bool TRUE 					= 1;
const bool FALSE 					= 0;

// output messages to monitor?
#define OUTPUT_ENABLED TRUE

const int ON						= HIGH;
const int OFF						= LOW;

const int UART_SPEED				= 19200;			// ASYNC speed from Arduino to PC via USB

const int LED_PIN					= 13;
const int LED_PIN_1				= 7;
const int LED_PIN_2				= 8;

const unsigned long PERIOD		= 3000UL;
const unsigned long DURATION	= 1000UL;

const unsigned long PERIOD1	= 2000UL;		// period for blinking LED 1
const unsigned long DURATION1	= 500UL;			// blink duration

const unsigned long PERIOD2	= 3000UL;			// for LED 2;
const unsigned long DURATION2 = 200UL;

const unsigned long ON_TIME   = 200UL;         // used in Example 11
const unsigned long OFF_TIME  = 200UL;

const int INTERVAL				= 200;			// interval between scheduled tasks

const unsigned int WPM			= 13;				// words per minute rate
const unsigned int WPM_1		= 5;				// words per minute rate for blinker 1
const unsigned int WPM_2		= 12;				// words per minute rate for blinker 2

const unsigned long DOT_DURATION   = 1200UL/WPM;	// duration of one dot
const unsigned long DOT_DURATION_1 = 1200UL/WPM_1;	// duration of one dot, blinker 1
const unsigned long DOT_DURATION_2 = 1200UL/WPM_2;	// duration of one dot, blinker 2

const int DOT = 1;
const int DASH= 3;
const int INTER_PULSE = 1;
const int INTER_CHAR = 3;
const int INTER_WORD = 7;

// Morse code table
const unsigned int mq[] = {6, DOT, DOT, DASH, DASH, DOT, DOT};	// question mark
const unsigned int ma[] = {6, DOT, DOT, DASH, DASH, DOT, DOT};	// filler, unused
const unsigned int mA[] = {2, DOT, DASH};								// the letter A
const unsigned int mB[] = {4, DASH, DOT, DOT, DOT};
const unsigned int mC[] = {4, DASH, DOT, DASH, DOT};
const unsigned int mD[] = {3, DASH, DOT, DOT};
const unsigned int mE[] = {1, DOT};
const unsigned int mF[] = {4, DOT, DOT, DASH, DOT};
const unsigned int mG[] = {3, DASH, DASH, DOT};
const unsigned int mH[] = {4, DOT, DOT, DOT, DOT};
const unsigned int mI[] = {2, DOT, DOT};
const unsigned int mJ[] = {2, DASH, DOT};
const unsigned int mK[] = {3, DASH, DOT, DASH};
const unsigned int mL[] = {4, DOT, DASH, DASH, DOT};
const unsigned int mM[] = {2, DASH, DASH};
const unsigned int mN[] = {2, DASH, DOT};
const unsigned int mO[] = {4, DASH, DASH, DASH, DASH};
const unsigned int mP[] = {4, DOT, DASH, DASH, DOT};
const unsigned int mQ[] = {4, DASH, DASH, DOT, DASH};
const unsigned int mR[] = {3, DOT, DASH, DOT};
const unsigned int mS[] = {3, DOT, DOT, DOT};
const unsigned int mT[] = {1, DASH};
const unsigned int mU[] = {3, DOT, DOT, DASH};
const unsigned int mV[] = {4, DOT, DOT, DOT, DASH};
const unsigned int mW[] = {3, DOT, DASH, DASH};
const unsigned int mX[] = {4, DASH, DOT, DOT, DASH};
const unsigned int mY[] = {4, DASH, DOT, DASH, DASH};
const unsigned int mZ[] = {4, DASH, DASH, DOT, DOT};

const unsigned int * CODES[] = {mq, ma, mA, mB, mC, mD, mE, mF, mG, mH, mI, mJ, mK, mL, mM, mN, mO, mP, mQ, mR, mS, mT, mU, mV, mW, mX, mY, mZ};// array of letter codes
const int OFFSET = '?';														// codes index offset

#endif

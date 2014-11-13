//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC LEDS TEST PROGRAM :													//
//  -----------------------													//
//																			//
//  This test program switches the LEDs on and off in a nice pattern		//
//  so that the user can see if all LEDs are OK.							//
//																			//
//////////////////////////////////////////////////////////////////////////////


#include "../PrcLib/PRC.h"		// PRC.h contains the prototype of PrcInit() and timing functions
#include "../PrcLib/LEDs.h"		// LEDs.h contains the prototypes of LEDs functions


int main(void) {
	int i, j;

	//  PRC initialisation
	//  ALWAYS CALL THIS FUNCTION FIRST !!!!!!
	prcInit();
	
	//  The LED pattern is the following :
	//  1) First, LED7 is switched on.
	//  2) Then it is switched off and the LED to its right is switched on.
	//  3) After 200ms, the operation is repeated until LED0 is switched on,
	//     so that it appears that the light is moving from LED7 to LED0.
	//  4) Points 1->3 are reapeated 8x, but the number of point 3 is
	//     decremented by 1 each time, so that at the end, all LEDs are on.
	//  5) After 500ms, all LEDs are switched off, and the pattern restarts.
	while(1) {
		// Points 1->3 are repeated 8x
		for (i=0; i<8; i++) {
			ledWrite(7, 1);					// LED7 is switched on
			for (j=7; j>i; j--) {			// the light "moves" one step less at each iteration
				delay_ms(200);				// 200ms wait
				ledWrite(j, 0);				// The on LED is switched off
				ledWrite(j-1, 1);			// The LED to is right is switched on
			}
		}
		delay_ms(500);						// 500ms wait
		ledWriteByte(0);					// All LEDs are switched off
	}
}

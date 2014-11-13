//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC MOTORS TEST PROGRAM :												//
//  -------------------------												//
//																			//
//  This test program tests the motor ouputs by applying a tringular wave	//
//	voltage to the motors, so that their speed varies continously			//
//																			//
//////////////////////////////////////////////////////////////////////////////



#include "../PrcLib/PRC.h"		//  PRC.h contains the prototype of PrcInit() and timing functions
#include "../PrcLib/LEDs.h"		//  LEDs.h contains the prototypes of LEDs functions
#include "../PrcLib/DigIn.h"	//  DigIn.h contains the prototypes of digital inputs functions
#include "../PrcLib/AnaIn.h"	//  AnaIn.h contains the prototypes of analog inputs functions
#include "../PrcLib/Motors.h"	//  Motors.h contains the prototypes of motors functions
	

int main(void) {
	int i, j;

	//  PRC initialisation
	//  ALWAYS CALL THIS FUNCTION FIRST !!!!!!
	prcInit();
	for (i=0; i<8; i++)	{					// All motors are enabled
		motorEnable(i);
	}
	for (j=0; j<=20; j++) {					// Motors voltage is increased from 0 to 2000,
		for(i=0; i<8; i++) {				// linearly, by step of 100 each 100ms
			motorSetVoltage(i, 100*j);
		}
		delay_ms(100);
	}
	// a continuous triangular wave is applied to the motors
	while(1) {
		for (j=20; j>=-20; j--) {						// Motors voltage is decreased from 2000 to -2000,
			for(i=0; i<8; i++) {						// linearly, by step of -100 each 100ms
				motorSetVoltage(i, 100*j);
			}
			delay_ms(100);
		}
		for (j=-20; j<=20; j++) {						// Motors voltage is increased from -2000 to 2000,
			for(i=0; i<8; i++) {						// linearly, by step of 100 each 100ms
				motorSetVoltage(i, 100*j);
			}
			delay_ms(100);
		}
	}
}

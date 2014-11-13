//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC NXT ENCODERS TEST PROGRAM :											//
//  -------------------------												//
//																			//
//  This test program regulates a NXT motor position. Initial position is 0	//
//	and reference position is 1080° (3 turns).								//
//																			//
//////////////////////////////////////////////////////////////////////////////



#include "../PrcLib/PRC.h"		//  PRC.h contains the prototype of PrcInit() and timing functions
#include "../PrcLib/LEDs.h"		//  LEDs.h contains the prototypes of LEDs functions
#include "../PrcLib/DigIn.h"	//  DigIn.h contains the prototypes of digital inputs functions
#include "../PrcLib/AnaIn.h"	//  AnaIn.h contains the prototypes of analog inputs functions
#include "../PrcLib/Motors.h"	//  Motors.h contains the prototypes of motors functions


#define MOTOR	1
#define V_MAX	2000

int main(void) {
	long pos, err, voltage;
	
	//  PRC initialisation
	//  ALWAYS CALL THIS FUNCTION FIRST !!!!!!
	prcInit();
	motorEnable(MOTOR);						// The motor mustbe enabled before we can use it
	while(1) {
		pos = encoderRead(MOTOR);			// Motor position is read
		err = 1080 - pos;					// Position error is calculated
		voltage = 100*err;					// New motor command is caculated. Here Kp = 100
		if (voltage > V_MAX) {				// Voltage value is limited : we don't want the motor turning too fast
			voltage = V_MAX;
		} else if (voltage < -V_MAX) {
			voltage = -V_MAX;
		}
		motorSetVoltage(MOTOR, voltage);	// New command is applied to the motor
	}
}

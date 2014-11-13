//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC DIGITAL INPUTS TEST PROGRAM :										//
//  ---------------------------------										//
//																			//
//  This test program reads each digital input and writes its state on		//
//  the corresponding LED													//
//																			//
//////////////////////////////////////////////////////////////////////////////



#include "../PrcLib/PRC.h"		//  PRC.h contains the prototype of PrcInit() and timing functions
#include "../PrcLib/LEDs.h"		//  LEDs.h contains the prototypes of LEDs functions
#include "../PrcLib/DigIn.h"	//  DigIn.h contains the prototypes of Digital inputs functions


int main(void){
	int i;
	
	//  PRC initialisation
	//  ALWAYS CALL THIS FUNCTION FIRST !!!!!!
	prcInit();
	//  The program repeats the same actions endlessly
	while(1) {
		//  Digital inputs are handled one after the other, but program
		//  execution is so fast that it appears instantaneous to the user
		for (i=0; i<8; i++) {
			ledWrite(i, digInRead(i));	// digital input is read and its state is written on the LED
		}
	}		
}

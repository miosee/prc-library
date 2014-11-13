//////////////////////////////////////////////////////////////////////////////
//																			//
//	PRC ANALOG INPUTS TEST PROGRAM :										//
//  ---------------------------------										//
//																			//
//  This test program reads each analog input and writes its value on		//
//  the LEDs where it can be read in binary. The read value is divided by 4	//
//  to fit in an 8-bit value (it's a 10bit value)							//
//																			//
//////////////////////////////////////////////////////////////////////////////



#include "../PrcLib/PRC.h"		//  PRC.h contains the prototype of PrcInit() and timing functions
#include "../PrcLib/LEDs.h"		//  LEDs.h contains the prototypes of LEDs functions
#include "../PrcLib/DigIn.h"	//  DigIn.h contains the prototypes of digital inputs functions
#include "../PrcLib/AnaIn.h"	//  AnaIn.h contains the prototypes of analog inputs functions



int main(void){
	unsigned char i;
	int j;
	
	//  PRC initialisation
	//  ALWAYS CALL THIS FUNCTION FIRST !!!!!!
	prcInit();
	// We start with AnaIn0
	i = 0;
	while (1) {
		while(digInRead(0) == 0) {
			j = anaInRead(i);
			j = j >> 2;
			ledWriteByte(j);
			delay_ms(100);
		}
		while (digInRead(0) == 1);
		i++;
		if (i > 7) {
			i = 0;
		}	
		delay_ms(250);
	}	
}

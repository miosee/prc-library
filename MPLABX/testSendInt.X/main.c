//////////////////////////////////////////////////////////////////////////////
//																			//
//	Exemple de communication radio avec un terminal RS232					//
//--------------------------------------------------------------------------//
//																			//
//	Ce programme montre comment établir une communication entre le PRC		//
//	et un PC équipé d'un terlinal RS232.									//
//	Dans ce cas, aucune programmation n'est nécessaire côté PC				//
//																			//
//////////////////////////////////////////////////////////////////////////////



#include "../PrcLib/PRC.h"		//  PRC.h contains the prototype of PrcInit() and timing functions
#include "../PrcLib/LEDs.h"		//  LEDs.h contains the prototypes of LEDs functions
#include "../PrcLib/anaIn.h"
#include "../PrcLib/AMB2300.h"	//  Radio.h contains the prototypes of Bluetooth radio functions


int main(void) {
	int sampleNb;
	int samplePer;
	long nextTime;
	int i;
	
	prcInit();			// Initialisation du PRC
	radioInit();		// Initialisation de la radio
	// Main loop
	while(1) {
		if (radioGetStatus() != RADIO_CONNECTED){	// on attend d'etre connecte au PC (action du terminal RS232)
			ledToggle(7);							// en faisant clignoter une LED
			delay_ms(250);
		} else {
			ledWrite(7,1);							// une fois connecte, la LED reste allumee
			sampleNb = radioGetInt();				// on recupere le nombre d'echantillons voulu par l'utilisateur
			samplePer = radioGetInt();				// on recupere la periode d'echantillonnage voulue par l'utilisateur
			nextTime = samplePer;
			timerStart();							// on utilise le timer pour que la periode d'echantillonnage soit exacte
			for (i=0;i<sampleNb; i++) {
				while(timerRead() < nextTime);			// on attend le prochain instant d'echantillonnage
				radioSendInt(anaInRead(0));				// on envoie l'echantillon
				nextTime += samplePer;					// on calcule le prochain instant d'échantillonnage
			}
			while(radioGetStatus() == RADIO_CONNECTED);
		}
	}
}

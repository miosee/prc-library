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
#include <stdlib.h>


int main(void) {
	char c = 0;
	char str[10];
	
	prcInit();			// Initialisation du PRC
	radioInit();		// Initialisation de la radio
	// Main loop
	while(1) {
		delay_ms(500);
		if (radioGetStatus() != RADIO_CONNECTED){	// on attend d'etre connecte au PC (action du terminal RS232)
			ledToggle(7);							// en faisant clignoter une LED
		} else {
			ledWriteByte(0);							// une fois connecté, on éteint la LED
			ledWrite(c,1);
			itoa(str, anaInRead(c), 10);
			radioSendString(str);					// on envoie une chaine de caractères qui s'imprimera dans le terminal
			radioSendString("\n");
			if (radioGetRxBufferSpace() > 0) {
				c = radioGetChar() - '0';			// on récupère la réponse de l'utilisateur (le programme est bloqué en attendant)
				if (c < 0) {
					c = 0;
				} else if (c > 7) {
					c = 7;
				}
			}
		}
	}
}

/* [1] : les terminaux rs232 sont principalement conçus pour échanger du texte, ils interprètent donc les données qu'ils envoient/recoivent
 * comme du texte en code ASCII : chaque caractère correspond à une valeur entre 0 et 255.
 * Dans le cas des caractères représentant les chiffres décimaux, ils osnt codés par les valeurs allant de 48 (pour '0') à 57 (pour '9').
 * Pour obtenir un chiffre à partir de son code ASCII, il suffit donc de soustraire 48 de ce dernier.
 * Pour que le code soit plus "parlant", on peut écrire '0' à la place de 48 (comme à la ligne 34).
 * De manière générale, écrire un caractère entre '' dans un code en langage C est équivalent à écrire la valeur de son code ASCII.
 */

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
#include "../PrcLib/AMB2300.h"	//  Radio.h contains the prototypes of Bluetooth radio functions


int main(void) {
	char c;
	char str[35] = "Quelle LED voulez-vous inverser ?\n";
	
	prcInit();			// Initialisation du PRC
	radioInit();		// Initialisation de la radio
	// Main loop
	while(1) {
		c = 0;
		while (radioGetStatus() != RADIO_CONNECTED){	// on attend d'etre connecte au PC (action du terminal RS232)
			ledToggle(7);								// en faisant clignoter une LED
			delay_ms(250);
		}
		ledWrite(7,0);							// une fois connecté, on éteint la LED
		radioSendString(str);					// on envoie une chaine de caractères qui s'imprimera dans le terminal
		c = radioGetChar();						// on récupère la réponse de l'utilisateur (le programme est bloqué en attendant)
		c = c - '0';							// La valeur envoyée est normalement le code ASCII d'un chiffre, on le convertit [1]
		if ((c >= 0) && (c <= 7)) {				// on vérifie que c'est bien un chiffre entre 0 et 7
			ledToggle(c);						// on inverse la LED choisie
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

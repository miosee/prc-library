//////////////////////////////////////////////////////////////////////////////
//																			//
//	Exemple de communication radio avec un terminal RS232					//
//--------------------------------------------------------------------------//
//																			//
//	Ce programme montre comment �tablir une communication entre le PRC		//
//	et un PC �quip� d'un terlinal RS232.									//
//	Dans ce cas, aucune programmation n'est n�cessaire c�t� PC				//
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
		ledWrite(7,0);							// une fois connect�, on �teint la LED
		radioSendString(str);					// on envoie une chaine de caract�res qui s'imprimera dans le terminal
		c = radioGetChar();						// on r�cup�re la r�ponse de l'utilisateur (le programme est bloqu� en attendant)
		c = c - '0';							// La valeur envoy�e est normalement le code ASCII d'un chiffre, on le convertit [1]
		if ((c >= 0) && (c <= 7)) {				// on v�rifie que c'est bien un chiffre entre 0 et 7
			ledToggle(c);						// on inverse la LED choisie
		}
	}
}

/* [1] : les terminaux rs232 sont principalement con�us pour �changer du texte, ils interpr�tent donc les donn�es qu'ils envoient/recoivent
 * comme du texte en code ASCII : chaque caract�re correspond � une valeur entre 0 et 255.
 * Dans le cas des caract�res repr�sentant les chiffres d�cimaux, ils osnt cod�s par les valeurs allant de 48 (pour '0') � 57 (pour '9').
 * Pour obtenir un chiffre � partir de son code ASCII, il suffit donc de soustraire 48 de ce dernier.
 * Pour que le code soit plus "parlant", on peut �crire '0' � la place de 48 (comme � la ligne 34).
 * De mani�re g�n�rale, �crire un caract�re entre '' dans un code en langage C est �quivalent � �crire la valeur de son code ASCII.
 */

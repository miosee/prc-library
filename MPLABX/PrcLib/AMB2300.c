#include <xc.h>
#include "PRC.h"
#include "AMB2300.h"
#include "uart1.h"

#define RADIO_UART		UART1
#define RADIO_BAUDRATE	115200

radioStatusType radioStatus;

#define RADIO_RX_DATA_SIZE		250					//!< taille du buffer de réception
unsigned char radioRxData[RADIO_RX_DATA_SIZE];		//!< buffer de réception
int radioRxDataNb = 0;								//!< nombre d'octets en attente dans le buffer de réception
int radioRxDataStartPtr = 0;						//!< pointeur vers le plus ancien octet du buffer de réception
int radioRxDataEndPtr = 0;							//!< pointeur vers le plus récent octet du buffer de réception

#define RADIO_TX_DATA_SIZE		250					//!< taille du buffer d'émission
unsigned char radioTxData[RADIO_TX_DATA_SIZE];		//!< buffer d'émission
int radioTxDataNb = RADIO_TX_DATA_SIZE;				//!< place disponible (en octets) dans le buffer d'émission
int radioTxDataStartPtr = 0;						//!< pointeur vers le plus ancien octet du buffer d'émission
int radioTxDataEndPtr = 0;							//!< pointeur vers le plus récent octet du buffer d'émission


// defines wireless module reset pin
#define AMB2300_RESET			LATFbits.LATF7
#define TRIS_AMB2300_RESET		TRISFbits.TRISF7

#define AMB2300_STX						(0x02)
#define AMB2300_IND						(0x69)
#define AMB2300_READY					(0x25)
#define AMB2300_LINK_ESTABLISHED		(0x0C)
#define AMB2300_SPP_TRANSPARENT_MODE	(0x11)
#define AMB2300_SPP_LINK_RELEASED		(0x0E)
#define AMB2300_ETX						(0x03)



inline radioStatusType radioGetStatus(void) {
	return radioStatus;
}


/**
 * @return le nombre d'octets disponibles dans le buffer de réception
 */
inline int radioGetRxBufferSpace(void) {
	return (radioRxDataNb);
}


/**
 * @return la place disponible (en octets) dans le buffer d'émission
 */
inline int radioGetTxBufferSpace(void) {
	return (radioTxDataNb);
}


inline char radioGetChar(void) {
	char data;
	int test = 1;

	while(test) {
		uart1RxIsrDisable();								// on désactive l'interruption de réception pour manipuler les variables de l'ISR
		if(radioRxDataNb > 0) {							// on attend qu'il y ait au moins un octet à lire dans le buffer de réception
			radioRxDataNb--;									// on met à jour radioRxDataNb
			data = radioRxData[radioRxDataStartPtr];			// on lit l'octet le plus ancien du buffer de réception
			if (++radioRxDataStartPtr >= RADIO_RX_DATA_SIZE) {	// on met à jour le pointeur vers l'octet le plus ancien du buffer
				radioRxDataStartPtr = 0;
			}
			test = 0;
		}
		uart1RxIsrEnable();									// on réactive l'interruption
	}
	return(data);
}


inline void radioSendChar(char data) {
	int test = 1;

	while(test) {
		uart1TxIsrDisable();								// on désactive l'interruption de réception pour manipuler les variables de l'ISR
		if (radioTxDataNb > 0) {							// on attend qu'on puisse écrire au moins un octet dans le buffer d'émission)
			radioTxDataNb--;									// on met à jour radioTxDataNb
			radioTxData[radioTxDataEndPtr] = data;				// on écrit l'octet à la fin du buffer d'émission
			if (++radioTxDataEndPtr >= RADIO_TX_DATA_SIZE) {	// on met à jour le pointeur vers la fin du buffer
				radioTxDataEndPtr = 0;
			}
			test = 0;
		}
		uart1TxIsrEnable();									// on réactive l'interruption
	}
	if (U1STAbits.UTXBF == 0) {							// Si l'UART n'est pas en train d'envoyer un octet (le buffer d'émission était vide)
		IFS0bits.U1TXIF = 1;							// on active "manuellement" l'ISR pour démarrer l'envoi
	}
}


int radioGetInt(void) {
	union {
		int i;
		char c[2];
	} data;
	int i;

	for (i=0; i<2;i++) {
		data.c[i] = radioGetChar();
	}
	return(data.i);
}

void radioSendInt(int data) {
	radioSendChar((char)(data & 0xFF));			// on envoie l'octet de poids faible
	radioSendChar((char)((data >> 8) & 0xFF));		// on en voie l'octet de poids fort
}


float radioGetFloat(void) {
	union {
		float f;
		char c[4];
	} data;
	int i;

	for (i=0; i<4;i++) {
		data.c[i] = radioGetChar();
	}
	return(data.f);
}


void radioSendFloat(float data) {
	union {
		float f;
		char c[4];
	} tmp;
	int i;

	tmp.f = data;					// on copie le réel à envoyer dans la varaible temporaire pour popuvoir le manipuler
	for (i=0; i<4;i++) {
		radioSendChar(tmp.c[i]);	// on envoie les 4 octets du réel un par un
	}
}


void radioSendString(char* data) {
	int i = 0;

	while (*(data+i) != 0) {
		radioSendChar(*(data+i));
		i++;
	}
}


void __attribute__((interrupt, auto_psv)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0;
	while ((radioTxDataNb < RADIO_TX_DATA_SIZE) && uart1TxReady()) {
		// first byte of the output buffer is written in the UART Tx buffer
		uart1SendChar(radioTxData[radioTxDataStartPtr]);
		// the start pointer must now point to the next byte to transmit,
		// that is the next in the vector ...
		radioTxDataStartPtr++;
		// ... unless the end of the vector is reached. If so, the next byte
		// to transmit is at the beginning of the vector (even if the vector
		// is empty: the next byte will be written there)
		if (radioTxDataStartPtr >= RADIO_TX_DATA_SIZE) {
			radioTxDataStartPtr = 0;
		}
		// data counter is decremented
		radioTxDataNb++;
	}
}


inline int AMB2300DetectFrame(unsigned char data);


void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void) {
    unsigned char tmp;
	int frame;

    IFS0bits.U1RXIF = 0;
	while (U1STAbits.URXDA)	{
		if (U1STAbits.FERR == 1)
			tmp = U1RXREG;	// First, if there was a framing error, we discard the byte (should never happen).
		else if (radioRxDataNb < RADIO_RX_DATA_SIZE) {	// if input buffer is not full
			radioRxDataNb++;
			tmp = U1RXREG;
			radioRxData[radioRxDataEndPtr] = tmp;		// new byte is written in the buffer
			// the end pointer must now point to where the next byte received
			// has to be put, that is after this one in the vector ...
			radioRxDataEndPtr++;
			// ... unless the end of the vector is reached. If so, we put the
			// new byte at the beginning of the vector (as buffer is not full,
			// its first byte has to be empty)
			if (radioRxDataEndPtr == RADIO_RX_DATA_SIZE) {
				radioRxDataEndPtr = 0;
			}
			frame = AMB2300DetectFrame(tmp);
			switch (radioStatus) {
				case RADIO_UNINITIALIZED:
					if (frame == AMB2300_READY)
						radioStatus = RADIO_NOT_CONNECTED;
					break;
				case RADIO_NOT_CONNECTED:
					if (frame == AMB2300_LINK_ESTABLISHED) {
						radioStatus = RADIO_CONNECTED;
						radioRxDataNb = 0;
						radioRxDataStartPtr = radioRxDataEndPtr;
					}
					break;
				case RADIO_CONNECTED:
					if (frame == AMB2300_SPP_LINK_RELEASED) {
						radioStatus = RADIO_NOT_CONNECTED;
						radioRxDataNb = 0;
					}
			}
		}
	}
}





radioStatusType radioInit(void) {
	radioStatus = RADIO_UNINITIALIZED;
	uart1Config(RADIO_BAUDRATE, UART_8N1_HARD_HANDSHAKE);
	uart1TxIsrEnable();
	uart1RxIsrEnable();
	TRIS_AMB2300_RESET = 0;
	AMB2300_RESET = 0;
	delay_ms(1);
	AMB2300_RESET = 1;
	while(radioGetStatus() == RADIO_UNINITIALIZED);

	return radioStatus;
}



enum AMB2300FrameState {
	START,
	TYPE,
	OPCODE,
	LENGTH1,
	LENGTH2,
	CHECKSUM,
	DATA,
	END
};


inline int AMB2300DetectFrame(unsigned char data) {
	static enum AMB2300FrameState frameState = START;
	static int frameType = 0;
	static unsigned char checksum;
	static int length;

	switch (frameState) {
		case START:
			if (data == AMB2300_STX)
				frameState = TYPE;
			break;
		case TYPE:
			if (data == AMB2300_IND)
				frameState = OPCODE;
			else
				frameState = START;
			break;
		case OPCODE:
			if ((data == AMB2300_READY) || (data == AMB2300_LINK_ESTABLISHED) ||
				(data == AMB2300_SPP_TRANSPARENT_MODE) || (data == AMB2300_SPP_LINK_RELEASED)) {
				frameType = data;
				checksum = AMB2300_IND + data;
				frameState = LENGTH1;
			} else
				frameState = START;
			break;
		case LENGTH1:
			length = data;
			checksum += data;
			frameState = LENGTH2;
			break;
		case LENGTH2:
			length += (((unsigned int)data) << 8);
			checksum += data;
			frameState = CHECKSUM;
			break;
		case CHECKSUM:
			if (data == checksum)
				frameState = DATA;
			else
				frameState = START;
			break;
		case DATA:
			if (length-- <= 1)
				frameState = END;
			break;
		case END:
			frameState = START;
			if (data == AMB2300_ETX)
				return frameType;
	}
	return 0;
}
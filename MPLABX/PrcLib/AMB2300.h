#ifndef RADIO_H
#define	RADIO_H

typedef enum {
	RADIO_UNINITIALIZED,
	RADIO_NOT_CONNECTED,
	RADIO_CONNECTED
} radioStatusType;

radioStatusType radioInit(void);
radioStatusType radioGetStatus(void);
int radioGetRxBufferSpace(void);
int radioGetTxBufferSpace(void);
char radioGetChar(void);
void radioSendChar(char data);
int radioGetInt(void);
void radioSendInt(int data);
float radioGetFloat(void);
void radioSendFloat(float data);
void radioSendString(char* data);

#endif	/* RADIO_H */

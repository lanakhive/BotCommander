#include "framecom.h"
#include "XBeeUART.h"

//states for decoder state machine
typedef enum
{
	S_READY, S_SYNC, S_IDENT, S_READ, S_CHECK
} frameState;

//buffer to store encoded command for transmission
static char tbuffer[40];

//pointer to destination buffer
static char * buffer;

void startMessageInput(int * flag, char * dest, int size)
{
	buffer = dest;
	initUART(flag,&frameDecoder);
}

//Frame decoder state machine for byte RX ISR callback	
char frameDecoder(char byte)
{
	static frameState state = S_READY;
	static char length;
	static char checksum;
	
	static char *bptr;
	switch (state)
	{
	case S_READY: //ready to receive next frame
		if (byte == SYNCCHAR) state = S_SYNC;
		else state = S_READY;
		break;
	case S_SYNC: //frame synched, need address
		if (byte == THISADDR) state = S_IDENT;
		else state = S_READY;
		break;
	case S_IDENT: //frame addressed to device, need length
		length = byte; //set data length
		checksum = 0; //reset checksum
		bptr = buffer; //reset buffer pointer
		if (length != 0) state = S_READ;
		else state = S_READY;
		break;
	case S_READ: //reading data
		checksum ^= byte; //compute checksum
		*(bptr++) = byte; //read data byte
		if (--length == 0) state = S_CHECK;
		else state = S_READ;
		break;
	case S_CHECK: //need to verify checksum
		*bptr = '\0'; //add null terminator
		state = S_READY;
		if (checksum == byte) return 1; //set flag
		break;
	default:
		; //error
	}
	return 0;
}

//src must be null terminated
//returns number of bytes written
static int encodeFrame(char * src, char * dest, int dlength)
{
	char checksum = 0;
	char length=0;
	int dptr = 3;

	while (*src != '\0')
	{
		++length; //increment length
		if (length > dlength-5) return 0;//is length exceeded
		checksum ^= *src; //perform checksum xor
		*(dest+(dptr++)) = *(src++); //copy data
	}
	*(dest + dptr) = checksum; //write checksum
	*(dest + 1) = DESTADDR;
	*(dest + 2) = length; //write length
	*dest = SYNCCHAR; //write start
	*(dest + dptr + 1) = '\0'; //write null
	return length+5;
}

int sendString(char * str)
{
	if (!encodeFrame(str,tbuffer,40)) //encode command
	{
		return 0;
	}
	sendUARTString(tbuffer); //send command
	return 1;	
}


#include "XBeeUART.H"
#include <msp430f2619.h>

// ------------------------------------------------------------------
// FIFO
// ------------------------------------------------------------------

// Create a FIFO with a size defined by BUFSIZE
// Call fifoInit to initilize the FIFO
// then
// Call fifoFilled to check if fifo is full then
// call fifoIn to add a byte to the FIFO
// or
// Call fifoFilled to check if FIFO is empty then
// call fifoOut to get a byte from the FIFO

//BUFSIZE should be close to the maximum bytes sent at any
//single time
#define BUFSIZE 10

typedef struct {
	int curr; //offset of last byte removed
	int tail; //offset of last byte added
	int count; //number of bytes in the FIFO
	char buff[BUFSIZE]; //array of bytes up to BUFSIZE
} fifo;

static fifo txbuffer;
static fifo rxbuffer;
static int * rxflag;

static char (*bytehandler)(char);


void fifoInit(fifo * buffer)
{
	//initilize buffer offset pointers
	buffer->curr = 0;
	buffer->tail = 0;
	buffer->count = 0;
}

//add a byte to the FIFO
void fifoIn(fifo * buffer, char byte)
{
	if (buffer->count == BUFSIZE) return; //check if fifo full
	buffer->tail++; //increment pointer
	if (buffer->tail==BUFSIZE) buffer->tail = 0; //handle wraparound
	buffer->buff[buffer->tail]=byte; //add byte to fifo
	buffer->count++; //increment counter
}

//get a byte from the FIFO
char fifoOut(fifo * buffer)
{
	char byte;
	if (!buffer->count) return 0; //check if fifo empty
	buffer->curr++; //increment pointer
	if (buffer->curr == BUFSIZE) buffer->curr = 0; //handle wraparound
	byte = buffer->buff[buffer->curr]; //get byte from fifo
	buffer->count--; //decrement count
	return byte;
}

//check if fifo is filled
//return 0  : fifo has contents
//return 1  : fifo is completely full
//return -1 : fifo is empty
int fifoFilled(fifo * buffer)
{
	if (!buffer->count) return -1;
	if (buffer->count == BUFSIZE) return 1;
	return 0;
}

// ------------------------------------------------------------------
// UART
// ------------------------------------------------------------------

void initUART(int * flag, char (*handler)(char))
{
	int i,j;
	//add delay for XBee transparent serial to pair
	for (i=0;i<2000;i++)
		for (j=0;j<200;j++);
	//initialize fifo buffers
	fifoInit(&txbuffer);
	fifoInit(&rxbuffer);
	//Set flag when matching byte recieved
	rxflag = flag;
	bytehandler = handler;
	UCA0CTL0 = 0; //8-bit data, (no parity, 1 stop bit)
	UCA0CTL1 |= UCSSEL_2 | UCSWRST; //select clock source and disable UART
	//Setup baud rate
	UCA0BR1 = 0x00; //baud rate divider (high byte)
	UCA0BR0 = 0x68; //baud rate divider (low byte)
	UCA0MCTL = UCBRS_1; //modulation
	
	//UCA0STAT = UCLISTEN;
	TXRXPORTSEL |= TXPIN | RXPIN; //select primary peripheral function (UART)
	UCA0CTL1 &= ~UCSWRST; //enable UART
	IE2 |= UCA0RXIE; //enable RX interrupt
}

void sendUARTByte(char byte)
{
	while(fifoFilled(&txbuffer) == 1); //wait if FIFO full
	fifoIn(&txbuffer,byte); //add the byte to the buffer
	IE2 |= UCA0TXIE; //enable interrupt
}

char readUARTByte(void)
{
	return fifoOut(&rxbuffer);
}

void sendUARTString(char * bytes)
{
	while (*bytes != '\0')
	{
		while(fifoFilled(&txbuffer) == 1); //wait if FIFO full
		fifoIn(&txbuffer,*bytes++); //add each byte to fifo
		IE2 |= UCA0TXIE; //enable interrupt
	}
}

void sendUARTStringN(char * bytes, int length)
{
	int i;
	for (i=0;i<length;i++)
	{
		while(fifoFilled(&txbuffer) == 1); //wait if FIFO full
		fifoIn(&txbuffer,bytes[i]); //add each byte to fifo
		IE2 |= UCA0TXIE; //enable interrupt
	}
}

void readUARTString(char * dest, unsigned int maxlength)
{
	char byte;
	while (fifoFilled(&rxbuffer) != -1) //read until buffer empty
	{
		byte = fifoOut(&rxbuffer);
		if (!(maxlength--)) break; //stop after maxlength
		*(dest++) = byte;
	}
	*dest = '\0';
}

// return 0: buffer empty
// return 1: byte available in buffer
int checkUARTByte(void)
{
	return (!(fifoFilled(&rxbuffer) == -1));
}


//TXBUF0 is empty
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	//while (!(IFG1 & UTXIFGO));
	UCA0TXBUF = fifoOut(&txbuffer);
	if (fifoFilled(&txbuffer) == -1) //check if tx buffer empty
	{
		IE2 &= ~UCA0TXIE; //disable tx ISR
	}
}


//RXBUF0 is full
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
	char byte = UCA0RXBUF; //get the byte
	
	if (bytehandler(byte))
	{
		*rxflag = 1;
		_BIC_SR(LPM0_EXIT);
	}
	
	
	//if (byte == rxmatch) stopcount++;
	//else stopcount = 0;
	//if (stopcount == 3) //check if byte matches
	//{
	//	*rxflag = 1; //set flag
	//	_BIC_SR(LPM0_EXIT); //exit low power mode
	//}
	if (bytehandler==0) fifoIn(&rxbuffer,byte); //store byte in FIFO
}




#ifndef XBEEUART_H_
#define XBEEUART_H_

/********************************************************************
 * --- UART Driver ---
 * 
 * Interfaces with UART hardware to allow communication with the
 * XBee module
 *******************************************************************/

//define I/O ports for UART
#define TXRXPORTSEL P3SEL
#define TXPIN BIT4
#define RXPIN BIT5

/********************************************************************
 * initUART
 * Initialize UART driver
 * Inputs: flag - pointer to flag to set when matched
 *         handler - function pointer to receive byte
 * Outputs: none
 *******************************************************************/
void initUART(int * flag, char (*handler)(char));

/********************************************************************
 * sendUARTByte
 * Transmits a single byte 
 * Inputs: byte - byte to transmit
 * Outputs: none
 *******************************************************************/
void sendUARTByte(char byte);


 /*******************************************************************
  * sendUARTString
 * Transmits a null terminated string of bytes
 * Inputs: bytes - pointer to bytes
 * Outputs: none
 *******************************************************************/
void sendUARTString(char * bytes);

/********************************************************************
 * sendUARTStringN
 * Transmits a string of bytes of a given length
 * Inputs: bytes - pointer to bytes to send
 *         length - number of bytes to send
 * Outputs: none
 *******************************************************************/
void sendUARTStringN(char * bytes, int length);

/********************************************************************
 * readUARTByte
 * Read a single byte
 * Inputs: none
 * Outputs: first byte that has not been read
 *******************************************************************/
char readUARTByte(void);

/********************************************************************
 * readUARTString
 * Read a string of bytes
 * string will be null terminated
 * Inputs: dest - pointer to write string at
 *         maxlength - max characters to read
 *                     (excluding null)
 * Outputs: none
 *******************************************************************/
void readUARTString(char * dest, unsigned int maxlength);

/********************************************************************
 * checkUARTByte
 * check if a byte is available to be read
 * Inputs: none
 * Outputs: 0 - no byte available
 *          1 - byte available
 *******************************************************************/
int checkUARTByte(void);

#endif /*XBEEUART_H_*/

#ifndef FRAMECOM_H_
#define FRAMECOM_H_

/********************************************************************
 * --- Frame Communication Encoder/Decoder ---
 * 
 * Starts a thread to decode incoming commands and provides an
 * interface to encode and send messages
 * 
 * -Depends on UART driver
 *******************************************************************/

//Character used for sync
#define SYNCCHAR '~'

//Address for this device
#define THISADDR 0x01

//Address for destination device
#define DESTADDR 0x02

/********************************************************************
 * startMessageInput
 * Starts the message receive thread.
 * Will initialize the UART driver, decode incoming
 * messages, and allow for messages to be encoded
 * and sent
 * Inputs: flag - pointer to set flag for message received
 *         dest - pointer to buffer to store decoded message
 *         size - size of buffer
 * Outputs: none
 *******************************************************************/
void startMessageInput(int * flag, char * dest, int size);

/********************************************************************
 * frameDecoder
 * Starts the message receive thread.
 * Callback for state machine to decode incoming frame
 * Inputs: byte - byte received
 * Outputs: 0 - frame not decoded yet
 *          1 - full frame has been decoded sucessfully
 *******************************************************************/
char frameDecoder(char byte);

/********************************************************************
 * sendString
 * Encodes a command using a frame and sends it over UART
 * Inputs: str - null terminated string to send
 * Outputs: 0 - string could not be encoded
 *          1 - string encoded and sent
 *******************************************************************/
int sendString(char * str);

#endif /*FRAMECOM_H_*/

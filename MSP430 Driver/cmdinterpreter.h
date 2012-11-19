#ifndef CMDINTERPRETER_H_
#define CMDINTERPRETER_H_

/********************************************************************
 * --- Command Interpreter ---
 * 
 * Parses command strings and executes the corresponding routine.
 * 
 * -Commands must be of format "<command> <integer>"
 *******************************************************************/

//String to send when command interpreter is initalized
#define CMDINITSTRING "Embedded Bot v0.4"

/********************************************************************
 * cmdInit
 * Initalizes the command interpreter for parsing commands
 * Inputs: none
 * Outputs: none
 *******************************************************************/
void cmdInit(int * flags);

/********************************************************************
 * cmdExecute
 * reads the command that was recieved and
 * executes the corresponding command function
 * Inputs: command - null terminated string containing the command
 * Outputs: none
 *******************************************************************/
void cmdExecute(char * command);


static void int2hexstr(int value, char * dest);

#endif /*CMDINTERPRETER_H_*/

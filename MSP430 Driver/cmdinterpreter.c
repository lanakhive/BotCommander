#include <msp430f2619.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "XBeeUART.h"
#include "cmdinterpreter.h"
#include "motor.h"
#include "framecom.h"
#include "distance.h"
#include "shaftencoder.h"
#include "lightsensor.h"

static int * flagarray;

// ------------------------------------------------------------------
// Command Functions
// ------------------------------------------------------------------

static void cmdActionLight(int a)
{
	sendString("Valid command recognized");
	P1DIR = BIT0; //Set LED digital pin as output
	P1OUT ^= BIT0;
}

static void cmdActionGo(int a)
{
	sendString("Moving Forward");
	if (a == INT_MAX) a = 100;
	motorForward(a);
}

static void cmdActionReverse(int a)
{
	sendString("Moving Backward");
	if (a == INT_MAX) a = 100;
	motorReverse(a);
}

static void cmdActionStop(int a)
{
	sendString("Stopping");
	flagarray[1] = 0;
	motorCoast();
}

static void cmdActionTL(int a)
{
	sendString("Rotating Left");
	if (a == INT_MAX) a = 60;
	motorRotateCCW(a);
}

static void cmdActionTR(int a)
{
	sendString("Rotating Right");
	if (a == INT_MAX) a = 60;
	motorRotateCW(a);
}

static void cmdDistL(int a)
{
	int d = readDistance(DISTANCE_LEFT);
	static char diststring[] = "Left distance is ###### cm";
	int2hexstr(d, diststring+17);
	sendString(diststring);
}

static void cmdDistC(int a)
{
	int d = readDistance(DISTANCE_CENTER);
	static char diststring[] = "Center distance is ###### cm";
	int2hexstr(d, diststring+19);
	sendString(diststring);
}

static void cmdDistR(int a)
{
	int d = readDistance(DISTANCE_RIGHT);
	static char diststring[] = "Right distance is ###### cm";
	int2hexstr(d, diststring+18);
	sendString(diststring);
}

static void cmdRPM(int a)
{
	static char rpmstring[] = "RPM count is ######";
	int rrpm = shaftrpm();
	int2hexstr(rrpm, rpmstring+13);
	sendString(rpmstring);
}

static void cmdSC(int a)
{
	static char countstring[] = "Shaft encoder: ###### segments";
	int count = shaftcount();
	int2hexstr(count, countstring+15);
	sendString(countstring);
}

static void cmdDistLb(int a)
{
	int d = readDistanceBinary(DISTANCE_LEFT);
	static char diststring[] = "Left binary distance: ######";
	int2hexstr(d, diststring+22);
	sendString(diststring);
}

static void cmdDistCb(int a)
{
	int d = readDistanceBinary(DISTANCE_CENTER);
	static char diststring[] = "Center binary distance: ######";
	int2hexstr(d, diststring+24);
	sendString(diststring);
}

static void cmdDistRb(int a)
{
	int d = readDistanceBinary(DISTANCE_RIGHT);
	static char diststring[] = "Right binary distance: ######";
	int2hexstr(d, diststring+23);
	sendString(diststring);
}

static void cmdLightStart(int a)
{
	sendString("Seeking Light");
	flagarray[1] = 1;
}

static void cmdLightEnd(int a)
{
	sendString("Mission Complete");
	flagarray[1] = 0;
	motorCoast();
}

static void cmdLightLeft(int a)
{
	int d = getLightCount(LEFTLIGHT);
	static char diststring[] = "Left light sensor: ######";
	int2hexstr(d, diststring+19);
	sendString(diststring);
}

static void cmdLightRight(int a)
{
	int d = getLightCount(RIGHTLIGHT);
	static char diststring[] = "Right light sensor: ######";
	int2hexstr(d, diststring+20);
	sendString(diststring);
}

// ------------------------------------------------------------------
// Command Interpreter
// ------------------------------------------------------------------

//command buffer size
//determines max command length
#define CMDBUFFSIZE 10
//command buffer
static char * cmdBuf;

//struct to store command id and argument passed
typedef struct {
	int cmdID;
	int cmdArg;
} cmdArg;

//struct to store a command routine entry
typedef struct {
	char * command;
	void (*func)(int);
} cmdRoutine;

//array containing command and corresponding function addresses
//commands should be uppercase
static cmdRoutine cmdList[] = {
	{"GO",&cmdActionGo},
	{"RV",&cmdActionReverse},
	{"SP",&cmdActionStop},
	{"TL",&cmdActionTL},
	{"TR",&cmdActionTR},
	{"LI",&cmdActionLight},
	{"DL",&cmdDistL},
	{"DC",&cmdDistC},
	{"DR",&cmdDistR},
	{"RPM", &cmdRPM},
	{"SC", &cmdSC},
	{"DLB", &cmdDistLb},
	{"DCB", &cmdDistCb},
	{"DRB", &cmdDistRb},
	{"LS", &cmdLightStart},
	{"END", &cmdLightEnd},
	{"LL", &cmdLightLeft},
	{"LR", &cmdLightRight}
};

//calculate number of commands in the list
static const int cmdRoutineCount = sizeof(cmdList)/sizeof(cmdRoutine);

static void int2hexstr(int value, char * dest)
{
	static char lookup[] = "0123456789ABCDEF";
	dest[0] = '0';
	dest[1] = 'x';
	dest[2] = lookup[(value & 0xF000) >> 12];
	dest[3] = lookup[(value & 0x0F00) >> 8];
	dest[4] = lookup[(value & 0x00F0) >> 4];
	dest[5] = lookup[(value & 0x000F) >> 0];
}
	
static void identifyCommand(cmdArg * cmd)
{
	unsigned int cmdOffset;
	char * token;
	char * tokenend;
	int argnum;
	
	token = strtok(cmdBuf," ");//get the command word
	if (token == NULL) //check for empty command
	{
		cmd->cmdID = -1;
	}
	else
	{
		cmd->cmdID = -2; //assume command not found
		for (cmdOffset=0; cmdOffset<cmdRoutineCount; cmdOffset++) //find the matching command
		{
			if (strcmp(token, cmdList[cmdOffset].command) == 0) cmd->cmdID = cmdOffset;
		}
	}
	cmd->cmdArg = INT_MAX;
	token = strtok(NULL," ");//get the command argument
	if (token != NULL)
	{
		argnum = strtol(token,&tokenend,10);//convert the string to an integer
		if (token != tokenend && *tokenend == '\0') cmd->cmdArg = argnum; //correct argument
		else cmd->cmdID = -3; //incorrect argument
	}
}

void cmdExecute(char * command)
{
	cmdArg cmd;
	cmdBuf = command;
	identifyCommand(&cmd); //determine what the command is
	if (cmd.cmdID == -1) //command was empty
	{
	}
	else if (cmd.cmdID == -2) //command not in list
	{
		sendString("Unknown Command");
	}
	else if (cmd.cmdID == -3) //command argments wrong
	{
		sendString("Incorrect Format");
	}
	else if (cmd.cmdID == -4)
	{
		sendString("Transmission Error");
	}
	else //command found
	{
		cmdList[cmd.cmdID].func(cmd.cmdArg);
	}
	
}

void cmdInit(int * flags)
{
	flagarray = flags;
	sendString(CMDINITSTRING);
}



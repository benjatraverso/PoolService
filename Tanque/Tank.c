/*
 Name:		Tank.ino
 Created:	3/31/2016 5:40:30 PM
 Author:	Benjamint
*/

#include "Types.h"
#include "Pinout.h"
#include "States.c"

int state;

// the setup function runs once when you press reset or power the board
void setup()
{
	state = eIdle;

	pinMode(EnableLeft, OUTPUT); //set enable motor pin as output
	pinMode(EnableRight, OUTPUT);//same for right one

	//set al motor connected pins as output
	pinMode(MotorLF, OUTPUT);
	pinMode(MotorLB, OUTPUT);
	pinMode(MotorRF, OUTPUT);
	pinMode(MotorRB, OUTPUT);
}

// the loop function runs over and over again until power down or reset
void loop()
{
	switch (state)
	{
	case eIdle:
		eStateBeIdle();
		state = eMoveForward;
		break;
	case eMoveForward:
		eStateMoveForward();
		state = eMoveBackwards;
		break;
	case eMoveBackwards:
		eStateMoveBackwards();
		state = eTurnLeft;
		break;
	case eTurnLeft:
		eStateTurnLeft();
		state = eTurnRight;
		break;
	case eTurnRight:
		eStateTurnRight();
		state = eIdle;
		break;
	}
}

#include "States.h"

void eStateBeIdle()
{
	setBoth(LOW, LOW, LOW, LOW);
}

void eStateMoveForward()
{
	eStateBeIdle();
	seBoth(HIGH, LOW, HIGH, LOW);
}

void eStateMoveBackwards()
{
	beIdle();
	setBoth(LOW, HIGH, LOW, HIGH);
}

void eStateTurnLeft()
{
	beIdle();
	setBoth(HIGH, LOW, LOW, HIGH):
}

void eStateTurnRight()
{
	beIdle();
	setBoth(LOW, HIGH, HIGH, LOW);
}

void setBoth(bool MotorLeftForward,
			 bool MotorLeftBackwards,
			 bool MotorRightForward,
			 bool MotorRightBacwards)
{
	digitalWrite(MotorLF, MotorLeftForward);
	digitalWrite(MotorLB, MotorLeftBackwards);
	digitalWrite(MotorLF, MotorRightForward);
	digitalWrite(MotorLB, MotorRightBacwards);
}

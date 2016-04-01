#define EnableLeft  10  // Enable Pin for motor 1
#define EnableRight 11  // Enable Pin for motor 2

#define MotorLF 8   // Control pin Forward for left motor
#define MotorLB 9   // Control pin Backward for left motor
#define MotorRF 12  // Control pin Forward for right motor
#define MotorRB 13  // Control pin Backward for right motor

int state;

enum States
{
  eIdle = 0,
  eMoveForward,
  eMoveBackwards,
  eTurnRight,
  eTurnLeft
};

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
  delay(10000);
  //enable both motors half speed
  analogWrite(EnableLeft, 255);
  analogWrite(EnableRight, 255);
  
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

void eStateBeIdle()
{
  setBoth(LOW, LOW, LOW, LOW);
}

void eStateMoveForward()
{
  eStateBeIdle();
  setBoth(HIGH, LOW, HIGH, LOW);
}

void eStateMoveBackwards()
{
  eStateBeIdle();
  setBoth(LOW, HIGH, LOW, HIGH);
}

void eStateTurnLeft()
{
  eStateBeIdle();
  setBoth(HIGH, LOW, LOW, HIGH);
}

void eStateTurnRight()
{
  eStateBeIdle();
  setBoth(LOW, HIGH, HIGH, LOW);
}

void setBoth(int MotorLeftForward,int MotorLeftBackwards, int MotorRightForward, int MotorRightBacwards)
{
  digitalWrite(MotorLF, MotorLeftForward);
  digitalWrite(MotorLB, MotorLeftBackwards);
  digitalWrite(MotorRF, MotorRightForward);
  digitalWrite(MotorRB, MotorRightBacwards);
}


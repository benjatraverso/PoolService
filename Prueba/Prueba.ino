const byte EnableLeft = 10;  // Enable Pin for left motor
const byte EnableRight = 11;  // Enable Pin for right motor

const byte MotorLF = 8;   // Control pin Forward for left motor
const byte MotorLB = 9;   // Control pin Backward for left motor
const byte MotorRF = 12;  // Control pin Forward for right motor
const byte MotorRB = 13;  // Control pin Backward for right motor

const int  NORMAL_SPEED = 255/2;
const int  LOW_SPEED	= 255/4;
const int  HIGH_SPEED	= 255;
const int  DELAY		= 5000;

int Speed;

////////////////////////////////////////////////////////////////////////
//
//									SETUP
//
////////////////////////////////////////////////////////////////////////
void setup()
{
	//lets just focus on left motor
	pinMode(EnableLeft, OUTPUT);
	pinMode(MotorLF, OUTPUT);
	pinMode(MotorLB, OUTPUT);
	Speed = NORMAL_SPEED;
}

////////////////////////////////////////////////////////////////////////
//
//									LOOP
//
////////////////////////////////////////////////////////////////////////
void loop()
{
	//move forward
	digitalWrite(MotorLF, HIGH);
	digitalWrite(MotorLB, LOW);
	analogWrite(EnableLeft, Speed);
	delay(DELAY);

	//wait a little not moving
	digitalWrite(MotorLF, LOW);
	digitalWrite(MotorLB, LOW);
	analogWrite(EnableLeft, 0);
	analogWrite(EnableLeft, 0);
	delay(DELAY/2);

	//then move backwards
	digitalWrite(MotorLF, LOW);
	digitalWrite(MotorLB, HIGH);
	analogWrite(EnableLeft, Speed);
	delay(DELAY);
	
	//wait a little not moving again and repeat
	digitalWrite(MotorLF, LOW);
	digitalWrite(MotorLB, LOW);
	analogWrite(EnableLeft, 0);
	delay(DELAY/2);
}

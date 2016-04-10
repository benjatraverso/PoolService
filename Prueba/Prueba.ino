const byte EnableLeft = 10;  // Enable Pin for left motor

const byte MotorLF = 12;   // Control pin Forward for left motor
const byte MotorLB = 13;   // Control pin Backward for left motor

const int  NORMAL_SPEED = 100;
const int  LOW_SPEED	= 50;
const int  HIGH_SPEED	= 255;
const int  DELAY		= 2500;

int Speed;
int count = 0;
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
	Speed = LOW_SPEED;
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

  //incremento velocidades para descartar que el motor no se mueve por velocidad baja.
  if(Speed == 255)
  {
    Speed = 50;
  }
  else 
  {
    Speed+=50;
  }
  if(Speed > 255)
  {
    Speed = 255;
  }
}

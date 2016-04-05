const byte EnableLeft = 10;  // Enable Pin for left motor
const byte EnableRight = 11;  // Enable Pin for right motor

const byte MotorLF = 8;   // Control pin Forward for left motor
const byte MotorLB = 9;   // Control pin Backward for left motor
const byte MotorRF = 12;  // Control pin Forward for right motor
const byte MotorRB = 13;  // Control pin Backward for right motor

const bool FORWARD    = 1;
const bool BACKWARDS  = 0;
const int  NORMAL_SPEED = 255/2;
const int  LOW_SPEED = 255/4;
const int  HIGH_SPEED = 255;

enum States
{
  eIdle = 0,
  eMoveForward,
  eMoveBackwards,
  eTurnRight,
  eTurnLeft
};

int state;
int Speed;

void eStateBeIdle()
{
  digitalWrite(MotorLF, LOW);
  digitalWrite(MotorLB, LOW);
  digitalWrite(MotorRF, LOW);
  digitalWrite(MotorRB, LOW);
  
  //disable both motors
  analogWrite(EnableLeft, 0);
  analogWrite(EnableRight, 0);
}

void eStateMoveForward(int Speed)
{
  eStateBeIdle();
  setRight(FORWARD, Speed);
  setLeft(FORWARD, Speed);
}

void eStateMoveBackwards(int Speed)
{
  eStateBeIdle();
  setRight(BACKWARDS, Speed);
  setLeft(BACKWARDS, Speed);
}

void eStateTurnLeft(int Speed)
{
  eStateBeIdle();
  setRight(BACKWARDS, Speed);
  setLeft(FORWARD, Speed);
}

void eStateTurnRight(int Speed)
{
  eStateBeIdle();
  setRight(FORWARD, Speed);
  setLeft(BACKWARDS, Speed);
}

void setRight(bool Direction, int Speed)
{
  Serial.println("RightMotor: ");
  Serial.println(Direction);
  digitalWrite(MotorRF, Direction);
  digitalWrite(MotorRB, !Direction);
  analogWrite(EnableRight, Speed);
}

void setLeft(bool Direction, int Speed)
{
  Serial.println("LeftMotor: ");
  Serial.println(Direction);
  digitalWrite(MotorLF, Direction);
  digitalWrite(MotorLB, !Direction);
  analogWrite(EnableLeft, Speed);
}

void eStateOneRight(int Speed)
{
  eStateBeIdle();
  setRight(FORWARD, Speed);
}
 
void eStateOneLeft(int Speed)
{
  eStateBeIdle();
  setLeft(FORWARD, Speed);
}

// the setup function runs once when you press reset or power the board
void setup()
{
  state = eIdle;
  Serial.begin(9600);
  pinMode(EnableLeft, OUTPUT); //set enable motor pin as output
  pinMode(EnableRight, OUTPUT);//same for right one

  //set al motor connected pins as output
  pinMode(MotorLF, OUTPUT);
  pinMode(MotorLB, OUTPUT);
  pinMode(MotorRF, OUTPUT);
  pinMode(MotorRB, OUTPUT);

  Speed = NORMAL_SPEED;
}

// the loop function runs over and over again until power down or reset
void loop()
{
  Serial.println("State: ");
  Serial.println(state);
  switch (state)
  {
    case eIdle:
    {
      eStateBeIdle();
      state = eMoveForward;
      break;
    }
    case eMoveForward:
    {
      eStateMoveForward(Speed);
      state = eMoveBackwards;
      break;
    }
    case eMoveBackwards:
    {
      eStateMoveBackwards(Speed);
      state = eTurnLeft;
      break;
    }
    case eTurnLeft:
    {
      eStateTurnLeft(Speed);
      state = eTurnRight;
      break;
    }
    case eTurnRight:
    {
      eStateTurnRight(Speed);
      state = eIdle;
      break;
    }
    default:
    {
      state = eIdle;
    }
  }
 delay(5000);
}

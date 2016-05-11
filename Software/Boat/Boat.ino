const byte EnableLeft = 11;  // Enable Pin for left motor
const byte EnableRight = 10;  // Enable Pin for right motor

const byte MotorLF = 8;   // Control pin Forward for left motor
const byte MotorLB = 7;   // Control pin Backward for left motor
const byte MotorRF = 12;  // Control pin Forward for right motor
const byte MotorRB = 13;  // Control pin Backward for right motor

const bool FORWARD    = 1;
const bool BACKWARDS  = 0;
const byte NORMAL_SPEED = 100;
const byte LOW_SPEED = 75;
const byte HIGH_SPEED = 125;
const byte leftProximitySensor = 2;
const byte rightProximitySensor = 3;

enum States
{
  eIdle = 0,
  eMoveForward,
  eMoveBackwards,
  eTurnRight,
  eTurnLeft,
  eFullTurnLeft,
  eFullTurnright,
  eError
};

int glState;
int glSpeed;

volatile int glRightSensorTriggered = LOW;
volatile int glRightSensorTriggered = LOW;


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

  pinMode(proximitySensor, INPUT_PULLUP);
  //pin2 es interrupción 0, acoplo ahí el sensor de cercanía con cambio de estado
  attachInterrupt( digitalPinToInterrupt(rigthProximitySensor), rightSensor, CHANGE );
  attachInterrupt( digitalPinToInterrupt(leftProximitySensor), leftSensor, CHANGE );
}

// the loop function runs over and over again until power down or reset
void loop()
{
  switch (glState)
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

void rightSensor( void )
{
  rightSensorTriggered = HIGH;
}

void leftSensor( void )
{
  leftSensorTriggered = HIGH;
}
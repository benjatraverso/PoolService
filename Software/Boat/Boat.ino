const byte EnableLeft = 11;   // Enable Pin for left motor
const byte EnableRight = 10;  // Enable Pin for right motor

const byte MotorLF = 8;   // Control pin Forward for left motor
const byte MotorLB = 7;   // Control pin Backward for left motor
const byte MotorRF = 12;  // Control pin Forward for right motor
const byte MotorRB = 13;  // Control pin Backward for right motor

const bool FORWARD              = 1;
const bool BACKWARDS            = 0;
const byte NORMAL_SPEED         = 100;
const byte LOW_SPEED            = 75;
const byte HIGH_SPEED           = 125;
const byte leftProximitySensor  = 0;   //analog input 0 reads left sensor
const byte rightProximitySensor = 1;   //analog input 1 reads right sensor
const byte leftSensorEmitter    = 5;
const byte rightSensorEmitter   = 6;

const int NO_OBJECT         = 800;
const int OBJECT_TOO_CLOSE  = 400;
const int SENSING_INTERVALS = 2000;   //reading sensors every 2 seconds

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

void setup()
{
  glState = eIdle;
  Serial.begin(9600);
  pinMode(EnableLeft, OUTPUT); //set enable motor pin as output
  pinMode(EnableRight, OUTPUT);//same for right one
  pinmode(rightSensorEmitter, OUTPUT);
  pinmode(leftSensorEmitter, OUTPUT);
  //both start off
  digitalWrite(rightSensorEmitter, LOW);
  digitalWrite(leftSensorEmitter, LOW);
  
  //set al motor connected pins as output
  pinMode(MotorLF, OUTPUT);
  pinMode(MotorLB, OUTPUT);
  pinMode(MotorRF, OUTPUT);
  pinMode(MotorRB, OUTPUT);
}

void loop()
{
  DoStep();
  GetNextStep();
}

////////////////////////////////////////////////////
//
//    HERE WE SET WHICH WOULD BE THE NEXT STEP
//
////////////////////////////////////////////////////
void GetNextStep( void )
{
  switch (glState)
  {
    case eIdle:
    {
      int right = readRight();
      int left = readLeft();
      
      if(right <= left)
      {
        //we do not care about the speed here, just turn slow and check again in that state, 
        //if things got more complicated then switch to full turning but we will know which Direction
        glState = eTurnRight;          
      }
      else
      {
        glState = eTurnLeft;
      }
      break;
    }
    case eMoveForward:
    {
      int right = readRight();
      int left = readLeft();
      
      while(right < NO_OBJECT && left < NO_OBJECT)
      {
        //we stay moving forward untin one of the sensors register something
        delay( SENSING_INTERVALS );
      }

      glState = left <= right ? eTurnLeft : eTurnRight;
      break;
    }
    case eMoveBackwards:
    {
      //no often, only try this step in case of error
      glState = ;
      break;
    }
    case eTurnLeft:
    {
      glState = ;
      break;
    }
    case eTurnRight:
    {
      int right = readLeft();
      if(right < OBJECT_TOO_CLOSE)
      {
        glState = eTurnFullRight;          
      }
      else if(right > NO_OBJECT)
      {
        glState = eMoveForward;
      }
      else
      {
        glState = eTurnRight
      }
      glState = ;
      break;
    }
    case eTurnFullLeft:
    {
      //sensor level warning went to urgent
      int right = readRight();
      if(right < OBJECT_TOO_CLOSE)
      {
        glState = eTurnLeft();
      }
      break;
    }
    case eTurnFullRight:
    {
      //sensor level warning went to urgent
      int left = readLeft();
      if(left < OBJECT_TOO_CLOSE)
      {
        glState = eTurnRight();
      }
      break;
    }
    default:
    {
      glState = eIdle;
    }
  }
}

int readLeft( void )
{
  digitalWrite(leftSensorEmitter, HIGH);
  delay(1);//minimal time to wait to get a valid data
  int lecture = analogRead(leftProximitySensor);

  //leave emitter off for lowering consumtion
  digitalWrite(leftSensorEmitter, LOW);
  return lecture;
}

int readRight( void )
{
  digitalWrite(rightSensorEmitter, HIGH);
  delay(1);//minimal time to wait to get a valid data
  int lecture = analogRead(rightProximitySensor);

  //leave emitter off for lowering consumtion
  digitalWrite(rightSensorEmitter, LOW);
  return lecture;
}

void eStateBeIdle()
{
  //can not have delays for many steps may use it to stop motors first
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
  // always go to idle first to avoid enabling both pins of motor
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

void DoStep( void )
{
  switch (glState)
  {
    case eIdle:
    {
      eStateBeIdle();
      break;
    }
    case eMoveForward:
    {
      eStateMoveForward(glSpeed);
      break;
    }
    case eMoveBackwards:
    {
      eStateMoveBackwards(glSpeed);
      break;
    }
    case eTurnLeft:
    {
      eStateTurnLeft(glSpeed);
      break;
    }
    case eTurnRight:
    {
      eStateTurnRight(glSpeed);
      break;
    }
    case eTurnFullLeft:
    {
      //sensor level warning went to urgent
      eStateTurnFullLeft(glSpeed);
      break;
    }
    case eTurnFullRight:
    {
      //sensor level warning went to urgent
      eStateTurnFullRight(glSpeed);
      break;
    }
    default:
    {
      eStateBeIdle();
    }
  }
}
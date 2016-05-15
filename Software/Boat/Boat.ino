//----------------------------------------------------------------------------
//                            CONSTANTS
//----------------------------------------------------------------------------
const byte EnableLeft = 11;   // Enable Pin for left motor
const byte EnableRight = 10;  // Enable Pin for right motor

const byte MotorLF = 8;   // Control pin Forward for left motor
const byte MotorLB = 7;   // Control pin Backward for left motor
const byte MotorRF = 12;  // Control pin Forward for right motor
const byte MotorRB = 13;  // Control pin Backward for right motor

const bool FORWARD              = 1;
const bool BACKWARDS            = 0;

const byte leftProximitySensor  = 0;   //analog input 0 reads left sensor
const byte rightProximitySensor = 1;   //analog input 1 reads right sensor
const byte leftSensorEmitter    = 5;
const byte rightSensorEmitter   = 6;

const byte NORMAL_SPEED         = 100;
const byte LOW_SPEED            = 75;
const byte HIGH_SPEED           = 125;
const int NO_OBJECT         = 600;
const int OBJECT_TOO_CLOSE  = 400;
const int SENSING_INTERVALS = 2000;   //reading sensors every 2 seconds
const int STEPS_DELAY       = 3000;

//----------------------------------------------------------------------------
//                            ENUMS
//----------------------------------------------------------------------------
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

//----------------------------------------------------------------------------
//                            GLOBAL VARIABLES
//----------------------------------------------------------------------------
int glState;
int glSpeed = NORMAL_SPEED;

//----------------------------------------------------------------------------
//                            SETUP
//----------------------------------------------------------------------------
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

//----------------------------------------------------------------------------
//                            LOOP
//----------------------------------------------------------------------------
void loop()
{
  DoStep();
  GetNextStep();
}

//----------------------------------------------------------------------------
//                            NEXT STATE
//----------------------------------------------------------------------------
void GetNextStep( void )
{
  int right = readRight();
  int left = readLeft();

  switch (glState)
  {
    case eIdle:
    {
      //we do not care about the speed here, just normal turn (one motor) and check again later once in that state,
      //if things got more complicated then switch to full turning but we will know which Direction
      if(right <= left)
      {
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
      while(right < NO_OBJECT && left < NO_OBJECT)
      {
        //we stay moving forward untin one of the sensors register something significant
        delay( SENSING_INTERVALS );
        right = readRight();
        left = readLeft();
      }

      glState = left <= right ? eTurnLeft : eTurnRight;
      break;
    }

    case eTurnLeft:
    {
      while(right < NO_OBJECT)
      {
        //stay same state and pool sensor
        delay( STEPS_DELAY );
        right = readRight();
      }

      if(right < OBJECT_TOO_CLOSE)
      {
        glState = eTurnFullLeft;          
      }
      else(right > NO_OBJECT)
      {
        glState = eMoveForward;
      }

      break;
    }

    case eTurnRight:
    {
      while(left < NO_OBJECT)
      {
        //stay same state and pool sensor
        delay( STEPS_DELAY );
        left = readLeft();
      }

      if(left < OBJECT_TOO_CLOSE)
      {
        glState = eTurnFullRight;          
      }
      else(left > NO_OBJECT)
      {
        glState = eMoveForward;
      }

      break;
    }

    case eTurnFullLeft:
    {
      //sensor level warning went to urgent
      while( right < OBJECT_TOO_CLOSE )
      {
        delay( STEPS_DELAY );
        right = readRight();
      }

      glState = eTurnLeft();
      break;
    }

    case eTurnFullRight:
    {
      //sensor level warning went to urgent
      while( left < OBJECT_TOO_CLOSE )
      {
        left = readLeft();
        delay( STEPS_DELAY );
      }

      glState = eTurnRight();
      break;
    }

    case eError:
    {
      //no often, only try this step in case of error
      //all logic is inside the step function.
      //start over from idle for we don't know where are we
      glState = eIdle;
      break;
    }

    default:
    {
      //we should never be here, but..., just in case
      glState = eIdle;
    }
  }
}

//----------------------------------------------------------------------------
//                            SENSORS
//----------------------------------------------------------------------------
// TODO: turning sensor emitter on can be through PWM to decrease
// power consumtion
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

//----------------------------------------------------------------------------
//                                STATES
//----------------------------------------------------------------------------
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

void eStateMoveForward()
{
  // always go to idle first to avoid enabling both pins of motor
  eStateBeIdle(); 
  setRight(FORWARD, glSpeed);
  setLeft(FORWARD, glSpeed);
}

void eStateMoveBackwards()
{
  // always go to idle first to avoid enabling both pins of motor
  eStateBeIdle(); 
  setRight(BACKWARDS, glSpeed);
  setLeft(BACKWARDS, glSpeed);
}

void eStateTurnFullLeft()
{
  eStateBeIdle();
  setRight(BACKWARDS, glSpeed);
  setLeft(FORWARD, glSpeed);
}

void eStateTurnFullRight()
{
  eStateBeIdle();
  setRight(FORWARD, glSpeed);
  setLeft(BACKWARDS, glSpeed);
}

void eStateTurnRight()
{
  eStateBeIdle();
  setRight(FORWARD, glSpeed);
}
 
void eStateTurnLeft()
{
  eStateBeIdle();
  setLeft(FORWARD, glSpeed);
}

void setRight( bool Direction )
{
  digitalWrite(MotorRF, Direction);
  digitalWrite(MotorRB, !Direction);
  analogWrite(EnableRight, glSpeed);
}

void setLeft( bool Direction )
{
  digitalWrite(MotorLF, Direction);
  digitalWrite(MotorLB, !Direction);
  analogWrite(EnableLeft, glSpeed);
}

void eStateError( void )
{
  //full speed for all this attempts
  glSpeed = HIGH_SPEED;
  //be idle and move back
  eStateMoveBackwards();
  //keep pushing back for a while
  delay( STEPS_DELAY );
  //shake a little
  eStateTurnFullLeft();
  delay( STEPS_DELAY );
  eStateTurnFullRight();
  delay( STEPS_DELAY );
  //end, this will put me on idle later, let's hope the mad dance made it
  //get stuff back to normal
  glSpeed = NORMAL_SPEED;
}

//----------------------------------------------------------------------------
//                            CURRENT STATE
//----------------------------------------------------------------------------
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
      eStateMoveForward();
      break;
    }

    case eTurnLeft:
    {
      eStateTurnLeft();
      break;
    }

    case eTurnRight:
    {
      eStateTurnRight();
      break;
    }

    case eTurnFullLeft:
    {
      //sensor level warning went to urgent
      eStateTurnFullLeft();
      break;
    }

    case eTurnFullRight:
    {
      //sensor level warning went to urgent
      eStateTurnFullRight();
      break;
    }

    case eError:
    {
      eStateError();
      break;
    }

    default:
    {
      eStateBeIdle();
    }
  }
}
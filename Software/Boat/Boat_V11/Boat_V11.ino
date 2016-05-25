//----------------------------------------------------------------------------
//
// Versión 1.1
// Sensores con interrupciones
//
//----------------------------------------------------------------------------

#include "Boat.h"
//----------------------------------------------------------------------------
//                            GLOBAL VARIABLES
//----------------------------------------------------------------------------
volatile byte glState;
volatile byte glDirection = NEVERMIND;
int glSpeed = NORMAL_SPEED;

//----------------------------------------------------------------------------
//                            SETUP
//----------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);//for testing purposes
  pinMode( EnableLeft, OUTPUT ); //set enable motor pin as output
  pinMode( EnableRight, OUTPUT );//same for right one
  
  pinMode( leftSensor, OUTPUT );
  pinMode( rightSensor, OUTPUT );

  pinMode(rightProximitySensor, INPUT_PULLUP);
  attachInterrupt( digitalPinToInterrupt(rightProximitySensor), rightObjectDetected, LOW );

  pinMode(leftProximitySensor, INPUT_PULLUP);
  attachInterrupt( digitalPinToInterrupt(leftProximitySensor), leftObjectDetected, LOW );

  //set al motor connected pins as output
  pinMode( MotorLF, OUTPUT );
  pinMode( MotorLB, OUTPUT );
  pinMode( MotorRF, OUTPUT );
  pinMode( MotorRB, OUTPUT );

  // start with everything dead
  glState = eError; //start on error to test the most movements we can on launch
}

//----------------------------------------------------------------------------
//                            LOOP
//----------------------------------------------------------------------------
void loop( void )
{
  switch ( glState )
  {
    case eIdle:
    {
      beIdle();
      break;
    }

    case eMoveForward:
    {
      moveForward();
      enableSensors();
      delay(20);
      while(glState == eMoveForward)
      {
        delay( STEPS_DELAY );
        //TODO: get outta here if've been for too long
        //also do other stuff while moving forward (future)
      }
      break;
    }

    case eTurn:
    {
      turn(glDirection);
      if(glDirection == TURN_RIGHT)
      {
        killRightSensor();
        detachInterrupt(digitalPinToInterrupt(leftProximitySensor));
        attachInterrupt(digitalPinToInterrupt(leftProximitySensor), noMoreObject, RISING);
      }
      else if(glDirection == TURN_LEFT)
      {
        killLeftSensor();
        detachInterrupt(digitalPinToInterrupt(rightProximitySensor));
        attachInterrupt(digitalPinToInterrupt(rightroximitySensor), noMoreObject, RISING);
      }
      
      while(glState == eTurn)
      {
        //stay here as long as the object is still detected
        //TODO: if been here for too long change to fullturn
      }
      break;
    }
    
    case eTurnFull:
    {

      break;
    }

    case eError:
    {
      inError();
      //no often, only try this step in case of error
      //all logic is inside the step function.
      //start over from idle for we don't know where we are
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
void rightObjectDetected( void )
{
  glDirection = TURN_LEFT;
  glState = eTurn;
}

void leftObjectDetected( void )
{
  glDirection = TURN_RIGHT;
  glState = eTurn;
}

void noMoreObject( void )
{
  glDirection = NEVERMIND;
  glState = eMoveForward;
}

void inError( void )
{
  //full speed for all this attempts
  glSpeed = HIGH_SPEED;
  //be idle and move back
  moveBackwards();
  //keep pushing back for a while
  delay( STEPS_DELAY );
  //shake a little
  turnFullLeft();
  delay( STEPS_DELAY );
  turnFullRight();
  delay( STEPS_DELAY );
  //end, this will put me on idle later, let's hope the mad dance made it
  //get stuff back to normal
  glSpeed = NORMAL_SPEED;
}

//----------------------------------------------------------------------------
//                                MOVEMENTS
//----------------------------------------------------------------------------
void beIdle( void )
{
  //can not have delays for many steps may use it to stop motors first
  killRight();
  killLeft();
}

void moveForward( void )
{
  // always go to idle first to avoid enabling both pins of motor
  setRight(FORWARD);
  setLeft(FORWARD);
}

void moveBackwards( void )
{
  // always go to idle first to avoid enabling both pins of motor
  setRight(BACKWARDS);
  setLeft(BACKWARDS);
}

void turnFullLeft( void )
{
  setRight(BACKWARDS);
  setLeft(FORWARD);
}

void turnFullRight( void )
{
  setRight(FORWARD);
  setLeft(BACKWARDS);
}

void turnRight( void )
{
  killLeft();
  setRight(FORWARD);
}
 
void turnLeft( void )
{
  killRight();
  setLeft( FORWARD );
}

void setRight( bool Direction )
{
  killRight();
  digitalWrite( MotorRF, Direction );
  digitalWrite( MotorRB, !Direction );
  analogWrite( EnableRight, glSpeed );
}

void setLeft( bool Direction )
{
  killLeft();
  digitalWrite( MotorLF, Direction );
  digitalWrite( MotorLB, !Direction );
  analogWrite( EnableLeft, glSpeed );
}

void killRight( void )
{
  digitalWrite( MotorRF, LOW );
  digitalWrite( MotorRB, LOW );
  digitalWrite( EnableRight, LOW );
}

void killLeft( void )
{
  digitalWrite( MotorLF, LOW );
  digitalWrite( MotorLB, LOW );
  digitalWrite( EnableLeft, LOW );
}

void move( bool Direction )
{
  if( Direction )
  {
    moveForward();
  }
  else
  {
    moveBackwards();
  }
}

void turn( bool Way )
{
  if( Way )
  {
    turnRight();
  }
  else
  {
    turnLeft();
  }
}


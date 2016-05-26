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
volatile byte glState = eError;
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
  pinMode(leftProximitySensor, INPUT_PULLUP);

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

      // for moving forward we need to open our eyes
      enableSensors();
      attachInterrupt( digitalPinToInterrupt(rightProximitySensor), rightObjectDetected, LOW );
      attachInterrupt( digitalPinToInterrupt(leftProximitySensor), leftObjectDetected, LOW );
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
      // we need to change interrupt for the sensor that detected an object
      // and we don't need the other one, so we detach both
      detachInterrupts();
      // record the much time we spend here...
      unsigned long timePassed = millis();
      // do the actual turn's task, turn
      turn(glDirection);

      // if we are here an interrupt popedUp, check which way to turn
      if( glDirection == TURN_RIGHT )
      {
        // if object detected left, no need to leave right sensor on until we complete turning
        killRightSensor();
        // we now need sensor to interrupt when the object is gone
        attachInterrupt( digitalPinToInterrupt( leftProximitySensor ), noMoreObject, RISING );
      }
      else if( glDirection == TURN_LEFT )
      {
        // if object detected right, no need to leave left sensor on until we complete turning
        killLeftSensor();
        // we now need sensor to interrupt when the object is gone
        attachInterrupt( digitalPinToInterrupt( rightProximitySensor ), noMoreObject, RISING );
      }
      
      while( glState == eTurn )
      {
        // stay here as long as the object is still detected
        if( FULL_TURN_TIME < (timePassed - millis() ) )
        {
          // or too long to be turning with just on motor
          glState = eTurnFull;
        }
      }
      break;
    }
    
    case eTurnFull:
    {
      unsigned long timePassed = millis();

      // do the step task
      turnFull( glDirection );

      // stay here untill there is a new noObject interrupt
      while( glState == eTurnFull )
      {
        // or we figured there must be an error
        if( ERROR_TIME < (timePassed - millis() ) )
        {
          glState = eError;
        }
      }
      break;
    }

    case eError:
    {
      inError();
      // no often, only try this step in case of error
      // all logic is inside the step function.
      // then start over from idle for we don't know where we are
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

void turnFull( bool Way )
{
  if( Way )
  {
    turnFullRight();
  }
  else
  {
    turnFullLeft();
  }
}

void enableSensors( void )
{
  digitalWrite( leftSensor, HIGH );
  digitalWrite( rightSensor, HIGH );
}

void killRightSensor( void )
{
  digitalWrite( rightSensor, LOW );
}

void killLeftSensor( void )
{
  digitalWrite( leftSensor, LOW );
}

void detachInterrupts( void )
{
  detachInterrupt( digitalPinToInterrupt( rightProximitySensor ) );
  detachInterrupt( digitalPinToInterrupt( leftProximitySensor ) );
}

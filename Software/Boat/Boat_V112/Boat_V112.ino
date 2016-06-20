//----------------------------------------------------------------------------
//
// Versión 1.1.2
// Interrupt throw sensors but only modifiyng glDirection in each
//
//----------------------------------------------------------------------------

#include "Boat.h"
//----------------------------------------------------------------------------
//                            GLOBAL VARIABLES
//----------------------------------------------------------------------------
byte glState;
volatile byte glDirection = NEITHER;
int glSpeed = NORMAL_SPEED;// TODO: change to HIGH_SPEED for solar version
volatile byte lint = HIGH;
volatile byte rint = HIGH;
//----------------------------------------------------------------------------
//                            SETUP
//----------------------------------------------------------------------------
void setup()
{
  Serial.begin( 9600 );//for testing purposes TODO: remove serial comm for release version
  pinMode( EnableLeft, OUTPUT ); //set enable motor pin as output
  pinMode( EnableRight, OUTPUT );//same for right one
  
  pinMode( leftSensor, OUTPUT );
  pinMode( rightSensor, OUTPUT );

  pinMode( rightProximitySensor, INPUT );
  pinMode( leftProximitySensor, INPUT );

  //set al motor connected pins as output
  pinMode( MotorLF, OUTPUT );
  pinMode( MotorLB, OUTPUT );
  pinMode( MotorRF, OUTPUT );
  pinMode( MotorRB, OUTPUT );

  // start with everything dead
  glState = eIdle;

  attachInterrupt( digitalPinToInterrupt( rightProximitySensor ), rightSensorAlert, CHANGE );
  attachInterrupt( digitalPinToInterrupt( leftProximitySensor ), leftSensorAlert, CHANGE );
  //attachInterrupt( rightProximitySensor, rightSensorAlert, CHANGE );
  //attachInterrupt( leftProximitySensor, leftSensorAlert, CHANGE );
}

//----------------------------------------------------------------------------
//                            LOOP
//----------------------------------------------------------------------------
void loop( void )
{
  Serial.println("Estate: ");
  switch ( glState )
  {
    case eIdle:
    {
      Serial.println("idle");
      beIdle();
      glState = eMoveForward;
      break;
    }

    case eMoveForward:
    {
      Serial.println("moving forward");
      moveForward();

      // for moving forward we need to open our eyes
      enableSensors();
      delay( 500 );

      rint = HIGH;
      lint = HIGH;
      while( rint && lint )
      {
        //TODO: get outta here if've been for too long
        //also do other stuff while moving forward (future)
      }
      Serial.println("object detected");
      glState = eTurn;
      break;
    }

    case eTurn:
    {
      Serial.print("turning ");
      // record the much time we spend here...
      unsigned long timePassed = millis();
      
      Serial.println(rint);
      Serial.println(lint);
      if( rint == LOW )
      {
        Serial.println("left");
        //killLeftSensor();
        // do the actual turn's task, turn
      }
      else if( lint == LOW )
      {
        Serial.println("right");
        //killRightSensor();
        // do the actual turn's task, turn
      }
      else
      {
        glState = eMoveForward;
      }
      
      while( !rint || !lint )
      {
        // stay here as long as the object is still detected
        
        if( FULL_TURN_TIME < ( millis() - timePassed ) )
        {
          // or too long to be turning with just on motor
          glState = eTurnFull;
          break;//get out of this loop
        }
      }
    
      //if both not detected, go to forward
      if( rint && lint )
      {
        Serial.println("object gone");
        glState = eMoveForward;
      }
      break;
    }
    
    case eTurnFull:
    {
      Serial.println("turning full");
      unsigned long timePassed = millis();

      // do the step task
      if( lint || rint )
      {
        glState = eMoveForward;
      }
      else
      {
        turnFull( lint );//if left is LOW it should turn to left, which is 0, if it right then left will be 1, which is the state of lint  
      }

      // stay here untill there is a new noObject interrupt
      while( !rint && !lint )
      {
        // or we figured there must be an error
        if( ERROR_TIME < ( millis() - timePassed ) )
        {
          glState = eError;
        }
      }
      
      if( rint && lint )
      {
        Serial.println("object gone");
        glState = eMoveForward;
        break;
      }

      break;
    }

    case eError:
    {
      inError();
      Serial.println("running error state");
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
  delay( STEPS_DELAY );
}

//----------------------------------------------------------------------------
//                            SENSORS - INTERRUPTS
//----------------------------------------------------------------------------
void rightSensorAlert( void )
{
  rint = digitalRead( rightProximitySensor );
  if( rint )
  {
    moveForward();
  }
  else
  {
    turnLeft();
  }
  lint = !rint;
}

void leftSensorAlert( void )
{
  lint = digitalRead( leftProximitySensor );
  if( lint )
  {
    moveForward();
  }
  else
  {
    turnRight();
  }
  lint = !rint;
}

void enableSensors( void )
{
  digitalWrite( leftSensor, HIGH );
  digitalWrite( rightSensor, HIGH );
}

void killRightSensor( void )
{
  Serial.println("kill right");
  digitalWrite( rightSensor, LOW );
}

void killLeftSensor( void )
{
  Serial.println("kill left");
  digitalWrite( leftSensor, LOW );
}

void detachInterrupts( void )
{
  detachInterrupt( digitalPinToInterrupt( rightProximitySensor ) );
  detachInterrupt( digitalPinToInterrupt( leftProximitySensor ) );
}

//----------------------------------------------------------------------------
//                                MOVEMENTS
//----------------------------------------------------------------------------
void inError( void )
{
  //full speed for all this attempts
  glSpeed = HIGH_SPEED;
  //be idle and move back
  moveBackwards();
  //keep pushing back for a while
  delay( ERROR_DELAY );
  //shake a little
  turnFullLeft();
  delay( ERROR_DELAY );
  turnFullRight();
  delay( ERROR_DELAY );
  turnFullLeft();
  delay( ERROR_DELAY );
  turnFullRight();
  delay( ERROR_DELAY );
  moveBackwards();
  //end, this will put me on idle later, let's hope the mad dance made it
  //get stuff back to normal
  glSpeed = NORMAL_SPEED;
}

void beIdle( void )
{
  //can not have delays for many steps may use it to stop motors first
  killRight();
  killLeft();
}

void moveForward( void )
{
  // always go to idle first to avoid enabling both pins of motor
  setRight( FORWARD );
  setLeft( FORWARD );
}

void moveBackwards( void )
{
  // always go to idle first to avoid enabling both pins of motor
  setRight( BACKWARDS );
  setLeft( BACKWARDS );
}

void turnFullLeft( void )
{
  setRight( BACKWARDS );
  setLeft( FORWARD );
}

void turnFullRight( void )
{
  setRight( FORWARD );
  setLeft( BACKWARDS );
}

void turnRight( void )
{
  killLeft();
  setRight( FORWARD );
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

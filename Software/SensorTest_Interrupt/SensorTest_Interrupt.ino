//----------------------------------------------------------------------------
//
// Versión 1.1.2
// Interrupts that interrupt!
//
//----------------------------------------------------------------------------

#include "Boat.h"

volatile int glSpeed = NORMAL_SPEED;
//----------------------------------------------------------------------------
//                            SETUP
//----------------------------------------------------------------------------
void setup()
{
  pinMode( EnableLeft, OUTPUT ); //set enable motor pin as output
  pinMode( EnableRight, OUTPUT );//same for right one
  
  pinMode( leftSensor, OUTPUT );
  pinMode( rightSensor, OUTPUT );

  pinMode( rightProximitySensor, INPUT_PULLUP );
  pinMode( leftProximitySensor, INPUT_PULLUP );

  //set al motor connected pins as output
  pinMode( MotorLF, OUTPUT );
  pinMode( MotorLB, OUTPUT );
  pinMode( MotorRF, OUTPUT );
  pinMode( MotorRB, OUTPUT );

  attachInterrupt( digitalPinToInterrupt( rightProximitySensor ), rightSensorAlert, CHANGE );
  attachInterrupt( digitalPinToInterrupt( leftProximitySensor ), leftSensorAlert, CHANGE );
}

//----------------------------------------------------------------------------
//                            LOOP
//----------------------------------------------------------------------------
void loop( void )
{
  enableSensors();
  moveForward();
  while( 1 )
  {
    //enter here what you want to do while moving forward...
  }
}

//----------------------------------------------------------------------------
//                            SENSORS - INTERRUPTS
//----------------------------------------------------------------------------
void rightSensorAlert( void )
{
  beIdle();
  if( digitalRead( rightProximitySensor ) )
  {
    moveForward();
  }
  else
  {
    turnFullLeft();
  }
}

void leftSensorAlert( void )
{
  beIdle();
  if( digitalRead( leftProximitySensor ) )
  {
    moveForward();
  }
  else
  {
    turnFullRight();
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

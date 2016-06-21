//----------------------------------------------------------------------------
//
// Versión 1.1.2
// Interrupts that interrupt!
//
//----------------------------------------------------------------------------

#include "Boat.h"
//----------------------------------------------------------------------------
//                            GLOBAL VARIABLES
//----------------------------------------------------------------------------
byte glState;
int glSpeed = NORMAL_SPEED;// TODO: change to HIGH_SPEED for solar version

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

  pinMode( rightProximitySensor, INPUT_PULLUP );
  pinMode( leftProximitySensor, INPUT_PULLUP );

  //set al motor connected pins as output
  pinMode( MotorLF, OUTPUT );
  pinMode( MotorLB, OUTPUT );
  pinMode( MotorRF, OUTPUT );
  pinMode( MotorRB, OUTPUT );

  attachInterrupt( digitalPinToInterrupt( rightProximitySensor ), rightSensorAlert, CHANGE );
  attachInterrupt( digitalPinToInterrupt( leftProximitySensor ) , leftSensorAlert , CHANGE );
}

//----------------------------------------------------------------------------
//                            LOOP
//----------------------------------------------------------------------------
void loop( void )
{
  enableSensors();
  int timelapsed = millis();
  moveForward( NORMAL_SPEED );
  while( 1 )
  {
    //no pool should take longer than 5 min
    if( 5 * 60 * SECONDS > ( millis() - timelapsed ) )
    {
      inError();
    }
    //place stuff to do while moving forward
  }
}

//----------------------------------------------------------------------------
//                            SENSORS - INTERRUPTS
//----------------------------------------------------------------------------
void rightSensorAlert( void )
{
  if( digitalRead( rightProximitySensor ) )
  {
    moveForward( NORMAL_SPEED );
  }
  else
  {
    turnFullLeft( NORMAL_SPEED );
  }
}

void leftSensorAlert( void )
{
  if( digitalRead( leftProximitySensor ) )
  {
    moveForward( NORMAL_SPEED );
  }
  else
  {
    turnFullRight( NORMAL_SPEED );
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
  moveBackwards( glSpeed );
  //keep pushing back for a while
  delay( ERROR_DELAY );
  //shake a little
  turnFullLeft( glSpeed );
  delay( ERROR_DELAY );
  turnFullRight( glSpeed );
  delay( ERROR_DELAY );
  turnFullLeft( glSpeed );
  delay( ERROR_DELAY );
  turnFullRight( glSpeed );
  delay( ERROR_DELAY );
  moveBackwards( glSpeed );
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

void moveForward( int sp )
{
  // always go to idle first to avoid enabling both pins of motor
  setRight( FORWARD, sp );
  setLeft( FORWARD, sp );
}

void turnFullLeft( int sp )
{
  setLeft( BACKWARDS, sp );
  setRight( FORWARD, sp );
}

void turnFullRight( int sp )
{
  setLeft( FORWARD, sp );
  setRight( BACKWARDS, sp );
}

void setRight( bool Direction, int sp )
{
  killRight();
  digitalWrite( MotorRF, Direction );
  digitalWrite( MotorRB, !Direction );
  analogWrite( EnableRight, sp );
}

void setLeft( bool Direction, int sp )
{
  killLeft();
  digitalWrite( MotorLF, Direction );
  digitalWrite( MotorLB, !Direction );
  analogWrite( EnableLeft, sp );
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

void moveBackwards( int sp )
{
  setRight( BACKWARDS, sp );
  setLeft( BACKWARDS, sp ); 
}


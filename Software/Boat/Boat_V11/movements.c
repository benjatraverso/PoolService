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
  setLeft(FORWARD);
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
  digitalWrite( MotorRF, Direction );
  digitalWrite( MotorRB, !Direction );
  digitalWrite( EnableRight, LOW );
}

void killLeft( void )
{
  digitalWrite( MotorLF, LOW );
  digitalWrite( MotorLB, LOW );
  digitalWrite( EnableLeft, LOW );
}

move( int Direction )
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

turn( bool Way )
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
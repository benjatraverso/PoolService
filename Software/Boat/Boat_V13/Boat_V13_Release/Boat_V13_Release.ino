//----------------------------------------------------------------------------
//
// Versión 13 - final
// watch sensors through pooling. Added shadow detection sensors - erase delays
//
//----------------------------------------------------------------------------

#include "Boat.h"
//----------------------------------------------------------------------------
//                            GLOBAL VARIABLES
//----------------------------------------------------------------------------
byte glState;
byte glDirection = NEITHER;
int glSpeed = HIGH_SPEED;// TODO: change to HIGH_SPEED for solar version
//----------------------------------------------------------------------------
//                            SETUP
//----------------------------------------------------------------------------
void setup()
{
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
      glDirection = NEITHER;
      glState = eMoveForward;
      break;
    }

    case eMoveForward:
    {
      moveForward();

	  // if stuck going forward we will realize by measuring an absurd amount of time
	  unsigned long timePassed = millis();
	  
	  // for moving forward we need to open our eyes
      enableSensors();
      delay( 500 );

      glDirection = NEITHER;

      while( NEITHER == glDirection )
      {
        if( !digitalRead( leftProximitySensor ) )
        {
          glDirection = TURN_RIGHT;
          glState = eTurn;
        }
        
        if( !digitalRead( rightProximitySensor ) )
        {
          glDirection = TURN_LEFT;
          glState = eTurn;
        }
        
        if( SUNS_DEAD < analogRead( LeftShadowSensor ) )
        {
          glDirection = TURN_RIGHT;
          glState = eTurnFull;
        }
        
		if(SUNS_DEAD < analogRead( RightShadowSensor ) )
    {
      glDirection = TURN_LEFT;
      glState = eTurnFull;
    }
        
		if ( (2*60*100) < ( millis() - timePassed ) )
		{
			// we've been here for too long, we've got to be stuck
			glState = eError;
			glDirection = NEITHER; //just in case...
			break;
		}

        //also do other stuff while moving forward (future)
        delay( POOLING_DELAY );
      }

      break;
    }

    case eTurn:
    {
      // record the much time we spend here...
      unsigned long timePassed = millis();
    
      if( TURN_LEFT == glDirection )
      {
        killLeftSensor();
        // do the actual turn's task, turn
        turnLeft();
      }
      else
      {
        killRightSensor();
        // do the actual turn's task, turn
        turnRight();
      }
      
	  while( NEITHER != glDirection && eTurn == glState )
      {
		// stay here as long as the object is still detected
		// only read the sensor we care about
		// if that input is now high, go to forward
		if (
			//if glDirection is different, the digitalRead is not performed and the other expression of the OR is evaluated
			( TURN_LEFT == glDirection && digitalRead( rightProximitySensor ) ) ||
			( TURN_RIGHT == glDirection && digitalRead( leftProximitySensor ) )
			)
		{
			glDirection = NEITHER;
			glState = eMoveForward;
			delay(GET_AWAY_TIME); // just turn a little longer so the angle is not completely parallel to the wall
		}
		// if not, check if we've been here for too long to be turning with just on motor
        else if( FULL_TURN_TIME < ( millis() - timePassed ) )
        {
          glState = eTurnFull;
        }
		else
		{
			delay( POOLING_DELAY ); // we will stay here, wait a little for next sensors check
		}
      }

      break;
    }
    
    case eTurnFull:
    {
      unsigned long timePassed = millis();

      // do the step task
      int lint = digitalRead( leftProximitySensor );
      int rint = digitalRead( rightProximitySensor );
      turnFull( glDirection );
      
      // stay here untill there is no more object detected or time's been too long
      while( NEITHER != glDirection && eTurnFull == glState )
      {
		// only read the sensor we care about
		// if that input is now high, go to forward
		if (
			(TURN_LEFT == glDirection && digitalRead(rightProximitySensor)) ||
			(TURN_RIGHT == glDirection && digitalRead(leftProximitySensor))
			)
		{
			glDirection = NEITHER;
			glState = eMoveForward;
			delay(GET_AWAY_TIME); // just turn a little longer so the angle is not completely parallel to the wall
		}
        // or we figured there must be an error
		else if( 2*60*100 < ( millis() - timePassed ) )
        {
          glState = eError;
		  glDirection = NEITHER; // just in case...
        }
		else
		{
			delay(POOLING_DELAY); // we will stay here, wait a little for next sensors check
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
      glDirection = NEITHER;
    }
  }
  //delay( STEPS_DELAY );
}

//----------------------------------------------------------------------------
//                            SENSORS - INTERRUPTS
//----------------------------------------------------------------------------
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

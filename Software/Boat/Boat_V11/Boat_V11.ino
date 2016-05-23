//----------------------------------------------------------------------------
// Versión 1.1
// Sensores con interrupciones
// sin pwm para habilitar motores para porder agregar líneas de debug en
// un arrchivo en tarjeta SD
//----------------------------------------------------------------------------

#include "Boat.h"

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
  glState = eError; //start on error to test the most movements we can on launch
  Serial.begin(9600);//for testing purposes
  pinMode( EnableLeft, OUTPUT ); //set enable motor pin as output
  pinMode( EnableRight, OUTPUT );//same for right one

  pinMode(rightProximitySensor, INPUT_PULLUP);
  attachInterrupt( digitalPinToInterrupt(rightProximitySensor), rightObjectDetected, CHANGE );

  pinMode(leftProximitySensor, INPUT_PULLUP);
  attachInterrupt( digitalPinToInterrupt(leftProximitySensor), leftObjectDetected, CHANGE );

  //set al motor connected pins as output
  pinMode( MotorLF, OUTPUT );
  pinMode( MotorLB, OUTPUT );
  pinMode( MotorRF, OUTPUT );
  pinMode( MotorRB, OUTPUT );

  // start with everything dead
  eStateBeIdle();
}

//----------------------------------------------------------------------------
//                            LOOP
//----------------------------------------------------------------------------
void loop( void )
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

      if( right < OBJECT_TOO_CLOSE )
      {
        glState = eTurnFullLeft;          
      }
      else if( right > NO_OBJECT )
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
      else if(left > NO_OBJECT)
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

      glState = eTurnLeft;
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

      glState = eTurnRight;
      break;
    }

    case eError:
    {
      inError();
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
  digitalWrite( leftSensorEmitter, HIGH );
  delay(1);//minimal time to wait to get a valid data
  int lecture = analogRead( leftProximitySensor );

  //leave emitter off for lowering consumtion
  digitalWrite( leftSensorEmitter, LOW );
  return lecture;
}

int readRight( void )
{
  digitalWrite( rightSensorEmitter, HIGH );
  delay(1);//minimal time to wait to get a valid data
  int lecture = analogRead( rightProximitySensor );

  //leave emitter off for lowering consumtion
  digitalWrite( rightSensorEmitter, LOW );
  return lecture;
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
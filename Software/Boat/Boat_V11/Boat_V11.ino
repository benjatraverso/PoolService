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
  switch (glState)
  {
    case eIdle:
    {

      break;
    }

    case eMoveForward:
    {
      interrupts();
      eMoveForward();
      while(glState == eMoveForward)
      {
        delay( STEPS_DELAY );
        //TODO: get outta here if've been for too long
        //also do other stuff while moving forward (future)
      }
      break;
    }

    case eTurnLeft:
    {

      break;
    }

    case eTurnRight:
    {

      break;
    }

    case eTurnFullLeft:
    {

      break;
    }

    case eTurnFullRight:
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
  noInterrupts();
  glState = eTurnLeft;
}

void leftObjectDetected( void )
{
  noInterrupts();
  glState = eTurnRight;
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
#ifndef BOAT_LIB
#define BOAT_LIB

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
  eTurnFullRight,
  eTurnFullLeft,
  eError
};

#endif
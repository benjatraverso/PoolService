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
const bool RIGHT                = 1;
const bool LEFT                 = 0;

const byte leftProximitySensor  = 3;   //interrupt input 0 reads left sensor
const byte rightProximitySensor = 2;   //interrupt input 1 reads right sensor
const byte leftSensor    = 5;
const byte rightSensor   = 4;

const byte NORMAL_SPEED         = 100;
const byte LOW_SPEED            = 75;
const byte HIGH_SPEED           = 125;
const int SECONDS               = 1000;
const int STEPS_DELAY           = 3 * SECONDS;
const int FULL_TURN_TIME      	= 10 * SECONDS;		//10 seconds to pass from turning to 2 motors turnning
const int ERROR_TIME		        = 60 * SECONDS;	// 60 seconds to determin we are stuck

const byte DETECTED      = 0;
const byte GONE          = 1;

const byte SUNS_DEAD     = 1000;

//----------------------------------------------------------------------------
//                            ENUMS
//----------------------------------------------------------------------------
enum States
{
  eIdle = 0,
  eMoveForward,
  eMoveBackwards,
  eTurn,
  eTurnFull,
  eError
};

enum Directions
{
  TURN_LEFT = 0,
  TURN_RIGHT,
  NEVERMIND
};

#endif

//                         +------------------+
//                         |                  |
//               RESET PC6 | 1             28 | ADC5
//                         |                  | 
//                  RX PD0 | 2             27 | ADC4
//                         |                  | 
//                  TX PD1 | 3             26 | ADC3
//                         |                  | 
//                INT0 PD2 | 4             25 | ADC2
//                         |                  | 
//          (PWM) INT1 PD3 | 5             24 | ADC1
//                         |                  | 
//                     PD4 | 6             23 | ADC0
//                         |                  | 
//                     VCC | 7             22 | GND
//                         |                  | 
//                     GND | 8             21 | AREF
//                         |                  | 
//                   XTAL1 | 9             20 | AVCC
//                         |                  | 
//                   XTAL2 | 10            19 | PD13
//                         |                  | 
//               (PWM) PD5 | 11            18 | PD12
//                         |                  | 
//               (PWM) PD6 | 12            17 | PD11 (PWM)
//                         |                  | 
//                     PD7 | 13            16 | PD10 (PWM)
//                         |                  | 
//                     PB0 | 14            15 | PD9 (PWM)
//                         |                  |
//                         +------------------+

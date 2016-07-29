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

const byte FORWARD              = 1;
const byte BACKWARDS            = 0;
const byte RIGHT                = 1;
const byte LEFT                 = 0;

const byte leftProximitySensor  = 2;   //interrupt input 0 reads left sensor (DP2 is interrupt 0)
const byte rightProximitySensor = 3;   //interrupt input 1 reads right sensor (DP3 is interrupt 1)
const byte leftSensor    = 5;
const byte rightSensor   = 4;

const byte NORMAL_SPEED         = 100;
const byte LOW_SPEED            = 75;
const byte HIGH_SPEED           = 125;
const int SECONDS               = 1000;
const int STEPS_DELAY           = 3 * SECONDS;
const int FULL_TURN_TIME      	= 3 * SECONDS;		// 10 seconds to pass from turning to 2 motors turnning
const int ERROR_TIME		        = 30 * SECONDS;   // seconds to determin we are stuck
const int ERROR_DELAY           = 10 * SECONDS;
const int POOLING_DELAY         = 1 * SECONDS;

const byte NEITHER     = 2;
const byte TURN_LEFT   = 0;
const byte TURN_RIGHT  = 1;

const byte SHADOW_LEFT = A0;
const byte SHADOW_RIGHT = A1;
const int SUNS_DEAD    = 1000;

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

#endif

//                               +-----___-----+
//                               |             |
//                         RESET | 1        28 | AI5
//                               |             |
//                           RXD | 2        27 | AI4
//                               |             |
//                           TXD | 3        26 | AI3
//                               |             |
// leftProximitySensor  INT0 PD2 | 4        25 | AI2
//                               |             |
// rightProximitySensor INT1 PD3 | 5        24 | AI1
//                               |             |
// rightSensor               PD4 | 6        23 | AI0
//                               |             |
//                           VCC | 7        22 | GND
//                               |             |
//                           GND | 8        21 | AREF
//                               |             |
//                         XTAL1 | 9        20 | AVCC
//                               |             |
//                         XTAL2 | 10       19 | DP13         MotorRB
//                               |             |
// leftSensor          (PWM) DP5 | 11       18 | DP12         MotorRF
//                               |             |
//                     (PWM) DP6 | 12       17 | DP11 (PWM)   EnableLeft
//                               |             |
// MotorLB                   DP7 | 13       16 | DP10 (PWM)   EnableRight
//                               |             |
// MotorLF                   DP8 | 14       15 | DP9 (PWM)
//                               |             |
//                               +-------------+

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

//                         +------------------+
//                         |                  |
//     (PCINT14/RESET) PC6 | 1             28 | PC5 (ADC5/SCL/PCINT13)
//                         |                  | 
//       (PCINT16/RXD) PD0 | 2             27 | PC4 (ADC4/SDA/PCINT12)
//                         |                  | 
//       (PCINT17/TXD) PD1 | 3             26 | PC3 (ADC3/PCINT11)
//                         |                  | 
//      (PCINT18/INT0) PD2 | 4             25 | PC2 (ADC2/PCINT10)
//                         |                  | 
//(PCINT19/OC2B/INT1) PD3  | 5             24 | PC1 (ADC1/PCINT9)
//                         |                  | 
//    (PCINT20/XCK/T0) PD4 | 6             23 | PC0 (ADC0/PCINT8)
//                         |                  | 
//                     VCC | 7             22 | GND
//                         |                  | 
//                     GND | 8             21 | AREF
//                         |                  | 
//(PCINT6/XTAL1/TOSC1) PB6 | 9             20 | AVCC
//                         |                  | 
//(PCINT7/XTAL2/TOSC2) PB7 | 10            19 | PB5 (SCK/PCINT5)
//                         |                  | 
//   (PCINT21/OC0B/T1) PD5 | 11            18 | PB4 (MISO/PCINT4)
//                         |                  | 
// (PCINT22/OC0A/AIN0) PD6 | 12            17 | PB3 (MOSI/OC2A/PCINT3)
//                         |                  | 
//      (PCINT23/AIN1) PD7 | 13            16 | PB2 (SS/OC1B/PCINT2)
//                         |                  | 
//  (PCINT0/CLKO/ICP1) PB0 | 14            15 | PB1 (OC1A/PCINT1)
//                         |                  |
//                         +------------------+
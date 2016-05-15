//----------------------------------------------------------------------------
//                            CONSTANTS
//----------------------------------------------------------------------------
const byte leftProximitySensor  = 0;   //analog input 0 reads left sensor
const byte rightProximitySensor = 1;   //analog input 1 reads right sensor
const byte leftSensorEmitter    = 5;
const byte rightSensorEmitter   = 6;

const int NO_OBJECT         = 800;
const int OBJECT_TOO_CLOSE  = 400;
const int SENSING_INTERVALS = 2000;   //reading sensors every 2 seconds
const int STEPS_DELAY       = 3000;

//----------------------------------------------------------------------------
//                            SETUP
//----------------------------------------------------------------------------
void setup()
{
  pinMode(rightSensorEmitter, OUTPUT);
  pinMode(leftSensorEmitter, OUTPUT);
  pinMode(13, OUTPUT);

  //both start off
  digitalWrite(rightSensorEmitter, LOW);
  digitalWrite(leftSensorEmitter, LOW);
}

//----------------------------------------------------------------------------
//                            LOOP
//----------------------------------------------------------------------------
void loop()
{
  int lecture = readLeft();
  if(lecture < OBJECT_TOO_CLOSE)
  {
    digitalWrite(13, HIGH);
  }
  else
  {
    digitalWrite(13, LOW);
  }
  delay(1000);
}

//----------------------------------------------------------------------------
//                            SENSORS
//----------------------------------------------------------------------------
// TODO: turning sensor emitter on can be through PWM to decrease
// power consumtion
int readLeft( void )
{
  delay(1);
  int enviromentLight = analogRead(leftProximitySensor);
  digitalWrite(leftSensorEmitter, HIGH);
  delay(1);//minimal time to wait to get a valid data
  int lecture = analogRead(leftProximitySensor);

  //leave emitter off for lowering consumtion
  digitalWrite(leftSensorEmitter, LOW);
  return lecture - enviromentLight;
}

int readRight( void )
{
  digitalWrite(rightSensorEmitter, HIGH);
  delay(1);//minimal time to wait to get a valid data
  int lecture = analogRead(rightProximitySensor);

  //leave emitter off for lowering consumtion
  digitalWrite(rightSensorEmitter, LOW);
  return lecture;
}

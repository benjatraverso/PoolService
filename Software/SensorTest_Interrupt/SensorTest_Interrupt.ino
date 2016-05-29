volatile int sensorTriggered = -1;
int proximitySensor = 3;

void setup() {
  Serial.begin( 9600 );//inicio comunicación serie
  pinMode(proximitySensor, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  digitalWrite(5,HIGH);
  attachInterrupt( digitalPinToInterrupt( proximitySensor ), objectClose, LOW );//pin2 es interrupción 0, acoplo ahí el sensor de cercanía con cambio de estado
}

void loop() {
  if(sensorTriggered == 1)
  {
    detachInterrupt( digitalPinToInterrupt( proximitySensor ) );
    attachInterrupt( digitalPinToInterrupt( proximitySensor ), objectGone, RISING );
    Serial.print( "Obstaculo cerca\n" );
    sensorTriggered = -1;
    delay( 2000 );
  }
  else if(sensorTriggered == 0)
  {
    detachInterrupt( digitalPinToInterrupt( proximitySensor ) );
    attachInterrupt( digitalPinToInterrupt( proximitySensor ), objectClose, LOW );
    Serial.print( "Obstaculo se alejo\n" );
    sensorTriggered = -1;
    delay( 2000 );
  }
}

void objectClose( void )
{
  sensorTriggered = 1;
}

void objectGone( void )
{
  sensorTriggered = 0;
}


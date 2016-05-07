volatile int sensorTriggered = LOW;
int proximitySensor = 2;

void setup() {
  Serial.begin( 9600 );//inicio comunicación serie
  pinMode(proximitySensor, INPUT_PULLUP);
  attachInterrupt( digitalPinToInterrupt(proximitySensor), broadcastSensor, CHANGE );//pin2 es interrupción 0, acoplo ahí el sensor de cercanía con cambio de estado
}

void loop() {
  if(sensorTriggered == HIGH)
  {
    Serial.print( "Obstaculo detectado\n" );
    sensorTriggered = LOW;
    delay(2000);
  }
}

void broadcastSensor( void )
{
  sensorTriggered = HIGH;
}


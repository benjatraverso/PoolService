int receiver = A0;               // IR photodiode on analog pin A0
int emitter = 2;            // IR emitter LED on digital pin 2

void setup()
{
  Serial.begin(9600);         // initializing Serial monitor
  pinMode(emitter,OUTPUT);  // IR emitter LED on digital pin 2
}

void loop()
{
  digitalWrite(emitter,HIGH);// setup IR LED as off
  int val = analogRead(receiver);
  Serial.println(val);
  delay(3000);
}


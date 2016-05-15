int receiver = 0;               // IR photodiode on analog pin A0
int emitter = 7;            // IR emitter LED on digital pin 2

void setup()
{
  Serial.begin(9600);         // initializing Serial monitor
  pinMode(emitter,OUTPUT);  // IR emitter LED on digital pin 7
}

void loop()
{
  Serial.print("enviromental light lecture\t");
  Serial.println(leer());
  digitalWrite(emitter,HIGH);// setup IR LED as on
  delay(500);
  int val = leer();
  Serial.print("With IR on lecture\t");
  Serial.println(val);
  //digitalWrite(emitter,LOW);
  delay(2000);
}

int leer(void)
{
  int lec = 0;
  for(int i =0; i< 10 ; i++)
  {
    lec += analogRead(receiver);
  }
  //devuelvo un promedio de 5 lecturas realizadas
  return lec/10;
}


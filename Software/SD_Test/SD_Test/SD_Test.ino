#include <SD.h>

File myFile;
void setup()
{
  SD.begin( 4 );

  myFile = SD.open( "data.txt", FILE_WRITE );

  String dataString = "Arduino On - Reading: ";
  
  // read three sensors and append to the string
  dataString += String( analogRead( A0 ) );
  
  // if the file is available, write to it:
  if ( myFile )
  {
    myFile.println(dataString);
  }
  
  myFile.close();
}

void loop()
{}

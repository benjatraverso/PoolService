#include <LiquidCrystal.h>

LiquidCrystal lcd(10, 9, 8, 7, 6, 5);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print( "Nivel de luz: ");//leave static label in first row
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print( analogRead( A0 ) );//refresh new lecture in second row
  delay(1000);
}

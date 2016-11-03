#include <LiquidCrystal.h>
 
/*******************************************************
 
This program is used to test the LCD module display and 5 buttons.
 
********************************************************/
 
// Select the pin used on LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 
// define the button
int lcd_key     = 0;
int adc_key_in  = 0;
 
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
 
//read the button value
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);          // read analog A0 value 
 // when read the 5 key values in the vicinity of the following：0,144,329,504,741
 // By setting different threshold, you can read the one button
 if (adc_key_in > 1000) return btnNONE; 
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
 
 // V1.0 Use the following version threshold：
/*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
*/
 
  
 return btnNONE; 
}
 
void setup()
{
 lcd.begin(16, 2);              // star
 lcd.setCursor(0,0);
 lcd.print("Push the buttons"); // display“Push the buttons”
}
  
void loop()
{
 lcd.setCursor(9,1);            // The cursor is set at second. and have 9 spaces
 lcd.print(millis()/1000);      // Output waiting time
 lcd.setCursor(0,1);            // The cursor moves to the beginning of the second line.
 lcd_key = read_LCD_buttons();  // read key
 
 switch (lcd_key)               // display key
 {
   case btnRIGHT:
     {
     lcd.print("RIGHT ");
     break;
     }
   case btnLEFT:
     {
     lcd.print("LEFT   ");
     break;
     }
   case btnUP:
     {
     lcd.print("UP    ");
     break;
     }
   case btnDOWN:
     {
     lcd.print("DOWN  ");
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT");
     break;
     }
     case btnNONE:
     {
     lcd.print("NONE  ");
     break;
     }
 }
}

/*
  ArduiOrbital LCD 
  LiquidCrystal Library - Serial Input

 The circuit:
 * LCD RS pin to digital pin 2
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground (MUST)
 * LCD Contrast pin to digital pin 3 ( a 100uF from pin3 to gnd can be needed)
 
 This code is under GPL2.
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 8, 4, 5, 6, 7);

void setup() {
  //configurar pwm para ese contraste.
  pinMode(3, OUTPUT);
  //FIXME: leer de la eeprom
  analogWrite(3,32);
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  lcd.clear();
  if (1==1) {
    lcd.setCursor(2,1);
    lcd.print("ArduiOrbital LCD");
    lcd.setCursor(2,2);
    lcd.print("(C) 2016 A. ALEA");
  } else {
    lcd.print("(C) 2016 A. ALEA");
    lcd.setCursor(0,2);
    lcd.print("ArduOrbital LCD");
  }
  delay(1000);
  // initialize the serial communications:
  Serial.begin(57600);
}

  byte c,num;
  byte col, fil;
  byte dir,lenght,cha;
  byte dat[7];                  

void loop()
{
    while (Serial.available() > 0) {
       c=Serial.read();
       if (c!=0xFE) {  
           lcd.write(c);
       } else {
           while (Serial.available() == 0) delay (1) ;
           c=Serial.read();
           switch (c) {
                case 0x35: // FIXME get custom data (16bytes)
                  //Serial.write("1234567"); // 8 bytes.
                  //Serial.write((byte)0x00); Serial.write((byte)0x00); 
                  //Serial.write((byte)0x00); Serial.write((byte)0x00); 
                  //Serial.write((byte)0x00); Serial.write((byte)0x00); 
                  Serial.write((byte)0x34); Serial.write((byte)0x12); 
                  break;
                case 0x36: // get version
                  Serial.write(0x19); //dummy value ver 1.9
                  break;
                 case 0x37: // get model
                  Serial.write(0x09); //dummy value 0x09 = 20x4  alphanumeric
                  break;
                case 0x40: // FIXME set screen boot
                   break;
               case 0x43: // line wrap on
                  //FIXME
                  break;
               case 0x44: // line wrap off
                  //FIXME
                  break;
                case 0x47: // set position
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   fil=Serial.read();
                   lcd.setCursor(col-1,fil-1);
                   break;
                case 0x48: // home
                   lcd.home();
                   break;
                case 0x4a: // guion bajo ON
                   lcd.cursor();
                   break;
                case 0x4b: // guion bajo OFF
                   lcd.noCursor();
                   break;
               case 0x4c: // cursor atras
                  //FIXME
                  break;
                case 0x4d: // cursor alante
                  //FIXME
                  break;
                case 0x4e: // createchar
                   while (Serial.available() == 0) delay (1) ;
                   cha=Serial.read();
                   for(num=0;num<=7;num++){
                      while (Serial.available() == 0) delay (1) ;
                      dat[num]=Serial.read();
                   }
                   lcd.createChar(cha,dat);                  
                   break;                
                case 0x50: // guardar y fijar contraste
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   analogWrite(3,col);
                   // FIXME: save in the eeprom the contrast
                   break;
                case 0x51: // FIXME autoscroll on
                   break;
                case 0x52: // FIXME autoscroll off
                   break;
                case 0x53: // parpadeo ON
                   lcd.blink();
                   break;
                case 0x54: // parpadeo OFF
                   lcd.noBlink();
                   break;

                case 0x41: // trans key press on
                   break;
                case 0x4F: // trans key press off
                   break;
                case 0x26: // poll key press
                    Serial.write((byte)0x00); //dummy value NO-KEYS
                    break;
                case 0x45: // clear key buffer
                   lcd.noBlink();
                   break;
                case 0x55: // set debounce
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0x7e: // set repeat
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0x60: // set autorepeat off
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0xD5: // assing keypad code
                   break;


                case 0x56: // FIXME GPO off
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0x57: // FIXME GPO on
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0xc3: // FIXME GPO initial state
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;

                case 0x58: // clear
                   lcd.clear();
                   break;
                case 0x91: // fijar contraste
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   analogWrite(3,col);
                   break;
                case 0x98: // fijar y grabar brillo
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0x99: // fijar brillo
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();

                   break;
                case 0xa4: // DUMMY set serial speed 
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0xc0: // FIXME load custom bank
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0xc1: // FIXME save custom character
                   break;
                case 0xc2: // FIXME save custom screenboot character
                   break;
                case 0x6d: // FIXME set custom character to medium number (1x2)
                   break;
                case 0x6f: // FIXME set custom character to medium number
                   while (Serial.available() == 0) delay (1) ;
                   fil=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   num=Serial.read();
                   // FIXME                    
                   break;
                case 0x6e: // FIXME set custom character to big number (1x4)
                   break;
                case 0x23: // FIXME set custom character to big number
                   while (Serial.available() == 0) delay (1) ;
                   fil=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   num=Serial.read();
                   // FIXME                    
                   break;
                case 0x68: // FIXME set custom character to horizontal bar
                   break;
                case 0x7C: // FIXME put horizontal bar
                   while (Serial.available() == 0) delay (1) ;
                   fil=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   dir=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   lenght=Serial.read();
                   // FIXME                    
                   break;
                case 0x73: // FIXME set custom character to vert bar narrow (2 pixel) 
                   break;
                case 0x76: // FIXME set custom character to vert bar wide (5 pixel) 
                   break;
                case 0x3d: // FIXME put vert. bar
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   lenght=Serial.read();
                   // FIXME                    
                   break;
              
           }
      }
    }
  
}

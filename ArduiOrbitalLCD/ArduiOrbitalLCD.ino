/*
  ArduiOrbital LCD 
  This code is under GPL2.
  V 0.2beta

 The circuit:
 * LCD RS pin to digital pin 2
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground (MUST)
 * LCD Contrast pin to digital pin 3 ( a 100uF from pin3 to gnd can be needed)
 
*/

/* EEPROM MAP
0x0001 (001)- contrast
0x0002 (001)- bright
0x0003 (001)- GPO initial state 
0x000A (320) - CHAR MAP 5 maps x 8 char x 8 bytes
0x014A (080) - Initial screen
0x019A 
*/


// include the library code:
#include <LiquidCrystal.h>

byte rf,rc;
byte remember;
byte linewrap=1;
byte autoscroll=0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 8, 4, 5, 6, 7);

//charmap for small char (1x2)
scm[2][10]={
		{4,2,6,6,5,7,7,1,3,3},
		{5,2,7,6,1,6,3,2,3,1}
		}
//charmap for large char (3x4)
lcm[3x4][10];


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
    lcd.setCursor(10,3);
    lcd.print("V 0.2beta");
    rf = 4;
    rc = 20;
  } else {
    lcd.print("(C) 2016 A. ALEA");
    lcd.setCursor(2,2);
    lcd.print("ArduOrbital LCD");
    rf = 1;
    rc = 19;
  }
  delay(1000);
  // initialize the serial communications:
  Serial.begin(57600);
}

  byte c,num;
  byte col, fil;
  byte dir,lenght,cha;
  byte dat[8];                  

void loadMapFromEEprom (byte map){
}
void saveCharToEEprom (byte map, byte idx){
}

void loop()
{
    while (Serial.available() > 0) {
       c=Serial.read();
       if (c!=0xFE) {  
           lcd.write(c);
           rc++;
           if (rc>=21) {
              rc=1;
              if (autoscroll==0) {
                 if (linewrap==1) {
                    rf++;
                    if (rf>4) {
                        rf = 1;
                    }
                    lcd.setCursor(rc-1,rf-1);
                 } else {
                    rf+=2;
                    if (rf==5) {
                        rf = 2;
                    } else if (rf==6) {
                        rf = 1;
                    }
                    lcd.setCursor(rc-1,rf-1); // en mi display no es necesario.                
                 }
              } else {
                 // FIXME SCROLL
              }
          }
       } else {
           while (Serial.available() == 0) delay (1) ;
           c=Serial.read();
           switch (c) {

// COMUNICATIONS
                case 0x33: // DUMMY set i2c address 
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0x39: // DUMMY set serial speed 
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0xa4: // DUMMY set serial speed 
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0xA0: // DUMMY set protocol 
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;

// TEXT
                case 0x51: // FIXME autoscroll on
                    autoscroll=1; 
               break;
                case 0x52: // autoscroll off
                   autoscroll=0;
               break;
                case 0x58: // clear
                   lcd.clear();
                   rf = 0;
               rc = 0;
                   break;
                case 0x40: // FIXME set screen boot
                   for(num=0;num<80;num++){
                      while (Serial.available() == 0) delay (1) ;
                      col=Serial.read();
                   }
                   // FIXME
                   break;
               case 0x43: // line wrap on
                  linewrap=1;
                  break;
               case 0x44: // line wrap off
                  linewrap=1;
                  break;
                case 0x47: // set position
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   fil=Serial.read();
                   lcd.setCursor(col-1,fil-1);
               	   rc = col;
                   rf = fil;
                   break;
                case 0x48: // home
                   lcd.home();
                   rf = 0;
               rc = 0;
                   break;
               case 0x4c: // cursor atras              
			// FIXME: debe obedecer lo del wrap.
			if (rc<=1) {
                rc=20;
                     if (rf<=1) {
                    rf=4;
                } else {
                    rf--;
                }
              } else {
                rc--;
                  }
                  lcd.setCursor(rc-1,rf-1);
                  break;
                case 0x4d: // cursor alante
              if (rc>=20) {
                rc=1;
                     if (rf>=4) {
                    rf=1;
                } else {
                    rf++;
                }
              } else {
                rc++;
                  }
                  lcd.setCursor(rc-1,rf-1);
                  break;
                case 0x4a: // guion bajo ON
                   lcd.cursor();
                   break;
                case 0x4b: // guion bajo OFF
                   lcd.noCursor();
                   break;
                case 0x53: // parpadeo ON
                   lcd.blink();
                   break;
                case 0x54: // parpadeo OFF
                   lcd.noBlink();
                   break;


// SPECIAL CHAR.
                case 0x4e: // createchar
                   while (Serial.available() == 0) delay (1) ;
                   cha=Serial.read();
                   for(num=0;num<=7;num++){
                      while (Serial.available() == 0) delay (1) ;
                      dat[num]=Serial.read();
                   }
                   lcd.createChar(cha,dat);                  
                   break;                
                case 0xc1: // save custom character
                   while (Serial.available() == 0) delay (1) ;
                   num=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   cha=Serial.read();
              saveCharToEEprom(num,cha);
                   break;
                case 0xc0: // load custom bank
                   while (Serial.available() == 0) delay (1) ;
                   loadMapFromEEprom(Serial.read());
                   break;
                case 0xc2: // save custom screenboot character
                   while (Serial.available() == 0) delay (1) ;
                   cha=Serial.read();
              saveCharToEEprom(0x0,cha);
                   break;
                case 0x6d: // set custom character to medium number (1x2)
			   dat[0]=dat[1]=dat[2]=dat[3]=dat[4]=dat[5]=dat[6]=dat[7]=0x01;
			   lcd.createChar(2,dat);
		         dat[0]=0xff;
		         lcd.createChar(1,dat);
		         dat[7]=0xff;
		         lcd.createChar(6,dat);
			    dat[1]=dat[2]=dat[3]=dat[4]=dat[5]=dat[6]=0x81;
		         lcd.createChar(3,dat);
		         dat[7]=0x81;
		         lcd.createChar(4,dat);
		         dat[0]=0x81;
		         dat[7]=0xFF;
		         lcd.createChar(5,dat);
		         dat[0]=0xFF;
			    dat[1]=dat[2]=dat[3]=dat[4]=dat[5]=dat[6]=0x80;
		         lcd.createChar(7,dat);
                   break;
                case 0x6f: // FIXME put medium number in screen
                   while (Serial.available() == 0) delay (1) ;
                   fil=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   num=Serial.read();
                   lcd.setCursor(col-1,fil-1);
                   lcd.write(scm[0][num]);
                   lcd.setCursor(col-1,fil);
                   lcd.write(scm[1][num]);
                   lcd.setCursor(rc-1,rf-1);
                   break;
                case 0x6e: // set custom character to big number (3x4)
                   loadMapFromEEprom(0x4);
              break;
                case 0x23: // FIXME set custom character to big number
                   while (Serial.available() == 0) delay (1) ;
                   fil=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   num=Serial.read();
                   lcd.setCursor(col-1,0);
                   lcd.write(scm[0][num]);
                   lcd.write(scm[0][num+10]);
                   lcd.write(scm[0][num+20]);
                   lcd.setCursor(col-1,1);
                   lcd.write(scm[1][num]);
                   lcd.write(scm[1][num+10]);
                   lcd.write(scm[1][num+20]);
                   lcd.setCursor(col-1,2);
                   lcd.write(scm[2][num]);
                   lcd.write(scm[2][num+10]);
                   lcd.write(scm[2][num+20]);
                   lcd.setCursor(col-1,1);
                   lcd.write(scm[3][num]);
                   lcd.write(scm[3][num+10]);
                   lcd.write(scm[3][num+20]);
                   lcd.setCursor(rc-1,rf-1);

                   // FIXME                    
                   break;
                case 0x68: // FIXME set custom character to horizontal bar
                   loadMapFromEEprom(0x2);
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
                   loadMapFromEEprom(0x2);
              break;
                case 0x76: // FIXME set custom character to vert bar wide (5 pixel) 
                   loadMapFromEEprom(0x2);
              break;
                case 0x3d: // FIXME put vert. bar
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   lenght=Serial.read();
                   // FIXME                    
                   break;


// GPO
                case 0x56: // FIXME GPO off
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0x57: // FIXME GPO on
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0xC3: // FIXME GPO initial state
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;

// 1 WIRE
                case 0xC8: // FIXME GPO off
                   while (Serial.available() == 0) delay (1) ;
                   num=Serial.read();
                   switch (num){
                       case 0x01:
                         while (Serial.available() == 0) delay (1) ;
                         col=Serial.read();
                         while (Serial.available() == 0) delay (1) ;
                         col=Serial.read();
                         Serial.write(0x03);
                         break;
                       case 0x02:
                          break;
                   }
                   break;

// KEYPAD
                case 0x41: // trans key press on
               // FIXME
                   break;
                case 0x4F: // trans key press off
               // FIXME
                   break;
                case 0x26: // poll key press
               // FIXME
                    Serial.write((byte)0x00); //dummy value NO-KEYS
                    break;
                case 0x45: // clear key buffer
               // FIXME
                   break;
                case 0x55: // set debounce
               // FIXME
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0x7e: // set repeat
               // FIXME
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0x60: // set autorepeat off
               // FIXME
                   break;
                case 0xD5: // assing keypad code
               // FIXME
               for(num=0;num<50;num++){
                      while (Serial.available() == 0) delay (1) ;
                      col=Serial.read();
                   }
                   break;

// DISPLAY FUNCTIONS
                case 0x42: // display ON
               // FIXME
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0x46: // display OFF
               // FIXME
                   break;
                case 0x99: // fijar brillo
               // FIXME
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0x98: // fijar y grabar brillo
               // FIXME
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   break;
                case 0x50: //  fijar contraste
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   analogWrite(3,col);
                   break;
                case 0x91: // FIXME guardar y fijar contraste
                   while (Serial.available() == 0) delay (1) ;
                   col=Serial.read();
                   analogWrite(3,col);
                   // FIXME: save in the eeprom the contrast
                   break;

// DATA SEC
                case 0x93: // FIXME set remenber
                  while (Serial.available() == 0) delay (1) ;
                  remember=Serial.read();
                  break;
                case 0xCA: // FIXME data lock
                   while (Serial.available() == 0) delay (1) ;
                   if (Serial.read()== 0xF5) {
                       while (Serial.available() == 0) delay (1) ;
                        if (Serial.read()== 0xA0) {
                            while (Serial.available() == 0) delay (1) ;
                           col=Serial.read();
                        }
                   }
                  break;
                case 0xCB: // FIXME set and save data lock
                   while (Serial.available() == 0) delay (1) ;
                   if (Serial.read()== 0xF5) {
                       while (Serial.available() == 0) delay (1) ;
                        if (Serial.read()== 0xA0) {
                            while (Serial.available() == 0) delay (1) ;
                           col=Serial.read();
                        }
                   }
                  break;
                case 0x34: // FIXME write custom data (16bytes)
               for(num=0;num<=16;num++){
                      while (Serial.available() == 0) delay (1) ;
                      col=Serial.read();
                   }
                   break;
                case 0x35: // FIXME get custom data (16bytes)
                  //Serial.write("1234567"); // 8 bytes.
                  //Serial.write((byte)0x00); Serial.write((byte)0x00); 
                  //Serial.write((byte)0x00); Serial.write((byte)0x00); 
                  //Serial.write((byte)0x00); Serial.write((byte)0x00); 
                  Serial.write((byte)0x34); Serial.write((byte)0x12); 
                  break;

// MISC
                case 0x36: // get version
                  Serial.write(0x19); //dummy value ver 1.9
                  break;
                case 0x37: // get model
                  Serial.write(0x09); //dummy value 0x09 = 20x4  alphanumeric
                  break;

              
           }
      }
    }
  
}




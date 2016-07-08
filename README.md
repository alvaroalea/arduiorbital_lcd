# arduiorbital_lcd
This is a simple prorgram that used de same protocol that matrixorbital in order to be able to use a 20x4 LCD with Lcd4Linux, Lcdproc, lcdsmartie, etc...

Not all commands are used, but some are emulated in order to not show garbage in screen.


As by now its 100% fully funcional with Lcd4Linux, lcdproc and lcdsmartie.

## Bugs

I'm testing it with a Arduino Leonardo, it must run in a Arduino Uno or Mega, but you must modify or implement the serial part of the protocol.

## To be implemented

No keyboard input or GPO yet.
No eeprom code, so not custom startup screen or custom characters

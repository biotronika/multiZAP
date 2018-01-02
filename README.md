# multiZAP

Library dependances:

Wire.h
Keypad.h
LiquidCrystal_I2C.h
EEPROM.h
   ^
   |
   |                                   
   |                               
   |     DS1803.h   AD9850.h       
   |       ^         ^             
   | ------'         |                Arduino.h
   | | --------------'                 ^
   | | |                               |
   def.h<--------------------bioZAP_func.h
                              ^  ^  ^  
                              |  |  |   
                              |  |  |  
                              |  |  | 
     -------------------------'  |  |
     |       --------------------'  |
     |      |                       |
   lcd.h   calib.h  prog.h          |
     ^        ^        ^            |
     |        |        |            |
     ----- menu.h------+------------'
              ^
              |
         multiZAP.ino
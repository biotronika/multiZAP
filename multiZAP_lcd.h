/*
 * multiZAP_lcd.h
 *
 *  Created on: 29 gru 2017
 *      Author: elektros230
 */

#ifndef MULTIZAP_LCD_H_
#define MULTIZAP_LCD_H_

//#include <Arduino.h>
#include <multiZAP_def.h>
//#include <bioZAP_func.h>
//#include <LiquidCrystal_I2C.h>


//Progress bar lcd characters
byte bar0[8] = {
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0};

byte bar1[8] = {
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10};

byte bar2[8] = {
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18};

byte bar3[8] = {
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C};

byte bar4[8] = {
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E};

byte bar5[8] = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F};

byte char6[8] = {
  B000000,
  B001100,
  B010010,
  B111111,
  B000000,
  B000000,
  B000000,
  B000000};

byte char7[8] = {
  B000000,
  B000000,
  B000000,
  B111111,
  B010010,
  B001100,
  B000000,
  B000000};



void lcd_init(){
	//Initialize LCD display
	lcd.init();
	lcd.backlight();
	lcd.createChar(0, bar0);
	lcd.createChar(1, bar1);
	lcd.createChar(2, bar2);
	lcd.createChar(3, bar3);
	lcd.createChar(4, bar4);
	lcd.createChar(5, bar5);
	lcd.createChar(6, char6);
	lcd.createChar(7, char7);
}


void lcd_hello(boolean pcConnection){
	//hello screen
	lcd.backlight();
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(HRDW_VER);
	lcd.setCursor(0,1);
  //lcd.print(HRDW_VER);
  //lcd.print(" ");
	lcd.print(SOFT_VER);

    Serial.print('>');
	if (pcConnection){
		lcd.print("  ");
		lcd.print("[PC]");

	}

}

void message (String messageText) {
// Message in #2 line
	//lcd.clear();
	lcd.setCursor(0,1);
	lcd.print("                ");
	lcd.setCursor(0,1);
	lcd.print(messageText);
}

long inputVal (String dialogText, long defaultVal){
/* Input dialog with [#] as end
 * Return 0 - 2,147,483,647
 * Escape [*] return -1
 * Allows [D] as decimal separator. Result is multiple x100 times
 */

	//boolean isEnd = false;
	String in="0";
    int col = 0;

    Serial.print(INPUT_SIGN_KEYPAD);
	Serial.print(dialogText);
	Serial.print(" ");
	Serial.println(defaultVal);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(dialogText);

    if (defaultVal>=0) {
      lcd.setCursor(0,1);
      lcd.print(INPUT_SIGN);
      lcd.print(defaultVal);
    }
    lcd.setCursor(0,1);


    do {
    	char key = keypad.getKey();
    		if (key != NO_KEY) {

               if (key=='#') break;
               if (key=='D') key='.';
               if (key=='*') {

            	   Serial.println(INPUT_BACK_KEYPAD+"esc");
            	   return -1;
               }
               if (col==0) {
                 lcd.print("                ");
                 lcd.setCursor(0,1);
                 lcd.print(INPUT_SIGN);
               }
               //lcd.setCursor(col,1);
               lcd.print(key);
               col++;

               //TODO: Do support for dot character and x100 multiplier
               in=in+key;
    		}

    } while (1);


    if (col==0 && defaultVal != -1) {

    	Serial.print(INPUT_BACK_KEYPAD);
    	Serial.println(defaultVal);
      return defaultVal;
    } else {

    	//Decimal character support
    	if (in.indexOf('.') != -1) {
    		in = in.substring( 0, in.indexOf('.') ) +
    			  ( in.substring( in.indexOf('.')+1 ) + "00" ).substring(0,2);
    		//message(in);
    		//Serial.println(in);
    	}

    	Serial.print(INPUT_BACK_KEYPAD);
    	Serial.println(in.toInt());
    	return in.toInt();


    }

}

int progressBar (byte progress) {
  // Show progress bar in #2 line
    lcd.setCursor(0,1);
    //lcd.print(" ");
    lcd.print(progress);
    lcd.print("%  ");
    lcd.setCursor(4,1);
    for (int i=0; i<(progress/10);i++) lcd.write(5);
    lcd.write(map( progress%10,0,9,0,5));
    lcd.print("          ");
    lcd.setCursor(14,1);
    lcd.write(6);
    lcd.setCursor(15,1);
    lcd.write(7);
    return 0;
}



#ifdef ENCODER_PROTOTYPE
void inputVal2 (){
//Input dialog with # as end
//TODO: To remove

	//Serial.print(String dialogTex);
	Serial.print(" inputVal2 ");
	//Serial.println(defaultVal);
    String in="0";
    boolean wiper = true;
    long defaultVal;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("gain_w0 setp_w1");
    lcd.setCursor(0,1);
    lcd.print(">");
    lcd.print(ds1803.get_wiper0());
    lcd.setCursor(8,1);
    lcd.print(" ");
    lcd.print(ds1803.get_wiper1());

    defaultVal=ds1803.get_wiper0();

    do {
     char key = keypad.getKey();
 	 int dir = encoder_data();

 	 if (dir!=0){
 		 if(dir > 0) {
 			 defaultVal++;
 		 } else {
 			 defaultVal--;
 		 }

		 if (wiper){

			lcd.setCursor(0,1);

			//wiper0=defaultVal;

			ds1803.set_wiper0(defaultVal);
		 } else {

			lcd.setCursor(8,1);

			//wiper1=defaultVal;

			ds1803.set_wiper1(defaultVal);
		 }
		 lcd.print(">");
	     lcd.print(defaultVal);
	     lcd.print("  ");

	     //TODO: calibration view
	     lcd.setCursor(13,1);
	     lcd.print(analogRead(outVoltagePin));
	     lcd.print("  ");

 	 }

      if (key != NO_KEY) {
        if (key=='*') {

            //EEPROM.write(EEPROM_WIPER0_ADDRESS,ds1803.get_wiper0()); // gain
            //EEPROM.write(EEPROM_WIPER1_ADDRESS,ds1803.get_wiper1()); // setpoint

            //lcd.clear();
            //lcd.setCursor(0,0);
            //lcd.print("EEPROM saved");

            //delay(50);

        	break;
        }

        if (key=='#') {
        	wiper = !wiper;

        	if (wiper){
        		defaultVal = ds1803.get_wiper0();
        	} else {
        		defaultVal = ds1803.get_wiper1();
        	}

        	if (wiper){
        		lcd.setCursor(8,1);
        		lcd.print(" ");
        		lcd.setCursor(0,1);
        	} else {
        		lcd.setCursor(0,1);
        		lcd.print(" ");
        		lcd.setCursor(8,1);
        	}
        	lcd.print(">");
			lcd.print(defaultVal);

			lcd.print("  ");
		}

      }
    } while (1);

    //lcd.noCursor();



    do key = keypad.getKey(); while (key != NO_KEY);


}
#endif /* ENCODER_PROTOTYPE */



#endif /* MULTIZAP_LCD_H_ */

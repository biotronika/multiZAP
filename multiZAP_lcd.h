/*
 * multiZAP_lcd.h
 *
 *  Created on: 29 gru 2017
 *      Author: elektros230
 */

#ifndef MULTIZAP_LCD_H_
#define MULTIZAP_LCD_H_


#include "bioZAP_func.h"

void message (String messageText, byte row = LCD_SCREEN_LINE);
long inputVal (String dialogText, long defaultVal = -1, byte defaultDigits = 8);
void progressBar (unsigned int totalTimeSec, unsigned int leftTimeSec);


void lcd_init(){
	//Initialize LCD display
	lcd.init();
	lcd.backlight();
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

void message (String messageText, byte row ) {
// Message in #2 line
	//lcd.clear();
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
	lcd.print( messageText );
}

long inputVal (String dialogText, long defaultVal, byte defaultDigits){
/* Input dialog with [#] as end
 * Return 0 - 2,147,483,647
 * Escape [*] return -1
 * Allows [D] as decimal separator. Result is multiple x100 times
 */

	String in="0";
    byte col = 0;

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

               in=in+key;
               if (col >= defaultDigits) {
            	   delay(70);
            	   break;
               }
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
    	}

    	Serial.print(INPUT_BACK_KEYPAD);
    	Serial.println(in.toInt());

    	return in.toInt();

    }
}

void progressBar (unsigned int totalTimeSec, unsigned int leftTimeSec) {
//Showing progress with left time in formats: 999m (greater then 10min), 120s (less then 10min)

	// Show progress bar in LCD_PBAR_LINE line - first is 0
    lcd.setCursor(0,LCD_PBAR_LINE);


    //TODO: left time formats
    //lcd.print(percent);
    //lcd.print("%  ");

    if (leftTimeSec>600){

        lcd.print( int(leftTimeSec/60) );
        lcd.print("m");
    } else {

        lcd.print(leftTimeSec);
        lcd.print("s");
    }

    if (totalTimeSec!=0) {

    	byte percent = 100 * leftTimeSec / totalTimeSec;

    	lcd.setCursor(4,LCD_PBAR_LINE);
		for (int i=0; i<(percent/10);i++) lcd.write(5);
		//lcd.write(map( progress%10,0,9,0,5));
		//map( progress%10,0,9,0,5)
		lcd.print("          ");

		lcd.setCursor(4,LCD_PBAR_LINE);
		lcd.write('[');
		lcd.setCursor(15,LCD_PBAR_LINE);
		lcd.write(']');

    }

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

/*
 * multiZAP_lcd.h
 *
 *  Created on: 29 gru 2017
 *      Author: elektros230
 */

#ifndef MULTIZAP_LCD_H_
#define MULTIZAP_LCD_H_


#include "bioZAP_func.h"
#include <avr/pgmspace.h>
#include "lang_EN.h"

void message (String messageText, byte row = LCD_SCREEN_LINE);
void message (byte msgNo, byte row= LCD_SCREEN_LINE);
long inputVal (byte msgNo,/*String dialogText,*/ long defaultVal = -1, byte defaultDigits = 8);
void progressBar (unsigned int totalTimeSec, unsigned int leftTimeSec);



unsigned long _lastProgressBarShowed = 0;


void lcd_init(){
	//Initialize LCD display
	lcd.init();
	lcd.backlight();
}


void lcd_hello(boolean pcConnection){
	//TODO:hello from ROM
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
// String message - avoid of lack of SRAM!
	//lcd.clear();
	lcd.setCursor(0, row);
	lcd.print("                ");
	lcd.setCursor(0, row);
	lcd.print( messageText );
}

void message (byte msgNo, byte row){
// Message read from ROM memory without using SRAM

	lcd.setCursor(0, row);

	for (b=0; b<16; b++)
		lcd.print( char(pgm_read_byte(msg[msgNo] + b)) );

}



long inputVal (byte msgNo,/*String dialogText,*/ long defaultVal, byte defaultDigits){
/* Input dialog with [#] as end
 * Return 0 - 2,147,483,647
 * Escape [*] return -1
 * Allows [D] as decimal separator. Result is multiple x100 times
 */

	String in="0";
    byte col = 0;

    //Serial.print(INPUT_SIGN_KEYPAD);
	//Serial.print(dialogText);
	//Serial.print(" ");
	//Serial.println(defaultVal);

    lcd.clear();
    //lcd.setCursor(0,0);
    //message (msgNo, 0);
    //lcd.print(dialogText);
    message (msgNo, 0);

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

            	   //Serial.println(INPUT_BACK_KEYPAD+"esc");
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

    	//Serial.print(INPUT_BACK_KEYPAD);
    	//Serial.println(defaultVal);

    	return defaultVal;

    } else {

    	//Decimal character support
    	if (in.indexOf('.') != -1) {
    		in = in.substring( 0, in.indexOf('.') ) +
    			  ( in.substring( in.indexOf('.')+1 ) + "00" ).substring(0,2);
    	}

    	//Serial.print(INPUT_BACK_KEYPAD);
    	//Serial.println(in.toInt());

    	return in.toInt();

    }
}

void progressBar (unsigned int totalTimeSec, unsigned int leftTimeSec) {
//Showing progress with left time in formats: 999m (greater then 10min), 120s (less then 10min)

	//Show ones a second
	if ( millis() > _lastProgressBarShowed + 1000 ) {
		_lastProgressBarShowed = millis();

		// Show progress bar in LCD_PBAR_LINE line - first is 0
		lcd.setCursor( 0, LCD_PBAR_LINE );

		if (leftTimeSec>600){

			lcd.print( int( leftTimeSec/60 ) );
			lcd.print("m   ");
		} else if (leftTimeSec<60) {

			lcd.print( leftTimeSec );
			lcd.print("s   ");
		} else {
			//Minutes section
			lcd.print( int( leftTimeSec/60 ) );
			lcd.print(':');

			//Seconds section
			if (leftTimeSec % 60 <10) lcd.print('0');
			lcd.print(leftTimeSec % 60);
		}

		if (totalTimeSec!=0) {

			byte percent = 5 + 100 * leftTimeSec / totalTimeSec;

			lcd.setCursor(5,LCD_PBAR_LINE);
			for (int i=0; i<(percent/10);i++) lcd.write(255); //lcd.write('#');

			lcd.print("          ");

			lcd.setCursor(4,LCD_PBAR_LINE);
			lcd.write('[');

			lcd.setCursor(15,LCD_PBAR_LINE);
			lcd.write(']');
		}
	}

}





#endif /* MULTIZAP_LCD_H_ */

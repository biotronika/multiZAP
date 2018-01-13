/*
 * multiZAP_menu.h
 *
 *  Created on: 31 dec 2017
 *      Author: elektros230
 */

#ifndef MULTIZAP_MENU_H_
#define MULTIZAP_MENU_H_


//#include <Arduino.h>
#include "multiZAP_def.h"
#include "multiZAP_calib.h"
#include "multiZAP_lcd.h"
#include "bioZAP_func.h"





void key_A(){
    //Freq = inputVal("Input frequency", 100000);
    Freq = inputVal(12, 100000);
    if (Freq>0) {

    	//unsigned int period = inputVal("Input period", 10);
    	unsigned int period = inputVal(3, 10);
    	//message(String(period));

    	//Serial.print("freq ");
    	//Serial.print(Freq);
    	//Serial.print(" ");
    	//Serial.println(period);

    	//Start freq
    	freq(Freq, period);
    	//message(String(freqStopMillis-freqStartMillis));
    	//delay(3000);


    }

}

void key_B(){
	/*TODO: Configuration parameters
	 * vmin - EEPROM
	 * vampl - EEPROM
	 * auto_turn_off_time - EEPROM
	 * AD985x 0/1 - EEPROM
	 * default_program - EEPROM
	 * multiZAP -/+/++ - EEPROM
	 * auto off after script therapy end
	 */

}


void key_C(){
	// Calibrate - set new vmin and vampl and saving in EEPROM

		get_v_EEPROM();

		//int vampl = inputVal("Input vampl", last_v_ampl,3);
		int vampl = inputVal(4, last_v_ampl,3);
		//int vmin = inputVal("Input vmin", last_v_min, 3);
		int vmin = inputVal(5, last_v_min, 3);
		Freq = 100000; //inputVal("Input freq", 100000);

		//message("Calibrating...");
		message(0);
		byte wiper0 = calib_gain_wiper_ampl(vampl, Freq);
		byte wiper1 = calib_setp_wiper_vmin(vmin);

		if ( wiper0 * wiper1 ){
			//Save last vampl and vmin
			set_v_EEPROM();
			//message("Saved in memory",0);
			message(6,0);
		} else {
			//message("Wiper's error!",0);
			message(7,0);
		}

	/*
		message("w0:");
		lcd.print(wiper0);
		lcd.print("  w1:");
		lcd.print(wiper1);
		delay(3000);
	*/
}


void key_D(){
	//SD card express scan

}

void key_asterix(){
//Manually turn device off
	//int ret=inputVal("Off?  1-yes 0-no", 1);
	int ret=inputVal(8, 0);
	if (ret) digitalWrite(powerPin, LOW);

}



void key_0(){
	if ((byte)EEPROM.read(0)!=255 && (byte)EEPROM.read(0)!=0) {
		//User program execute

		if ( !( (wiper0 = calib_gain_wiper_ampl(last_v_ampl, 100000)) > 0  &&
				(wiper1 = calib_setp_wiper_vmin(last_v_min)) > 0                  ) ) {

			//message("Error calibration");
			message(9);
		}
		//message("User program ...");
		message(10);

		executeCmd("exe\n",true);  //or exe();
		//TODO: off();
    } else {
    	//message("No program in memory!");
    	message(11);
    }


}



void key_9(){
// For service and test issues
/*	int vampl = inputVal("Input vampl", last_v_ampl,3);
	int vmin = inputVal("Input vmin", last_v_min, 3);
	Freq = inputVal("Input freq", 100000);

	message("va:");
	lcd.print(vampl);
	lcd.print("  vm:");
	lcd.print(vmin);
	delay(3000);

	message("Calibrating...");
	byte wiper0= calib_gain_wiper_ampl(vampl, Freq);
	byte wiper1= calib_setp_wiper_vmin(vmin);
	lcd.clear();
	lcd.print(Freq);
	message("w0:");
	lcd.print(wiper0);
	lcd.print("  w1:");
	lcd.print(wiper1);
	delay(3000);*/

}

void keyPressed(char key){

	switch (key){

		  case 'A':
			  key_A();
			  key=NO_KEY;
			  break;

		  case 'B':
			  key_B();
			  break;

		  case 'C':
			  key_C();
			  break;

		  case 'D':
			  key_D();
			  break;

		  case '*':
			  key_asterix();
			  break;

		  case '#':
			  //key_hash();
			  break;

		  case '0':
			  key_0();
			  break;

		  default:
			  ;

			  break;
	}
}

#endif /* MULTIZAP_MENU_H_ */

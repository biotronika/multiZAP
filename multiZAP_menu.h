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
    Freq = inputVal("Input frequency", 100000);
    if (Freq>0) {

    	unsigned int period = inputVal("Input period", 10);
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

int key_B(){
	/*TODO: Configuration parameters
	 * vmin - EEPROM
	 * vampl - EEPROM
	 * auto_turn_off_time - EEPROM
	 * AD985x 0/1 - EEPROM
	 * default_program - EEPROM
	 * multiZAP -/+/++ - EEPROM
	 * auto off after script therapy end
	 */

	return 0;
}


int key_C(){
    // Select program
   byte program = 1; //byte(inputVal("Wybierz prog 1-3"));

    lcd.clear();
    lcd.setCursor(0,1);
    //lcd.print("prog: ");
    //lcd.print(program);

    lcd.setCursor(0,0);
    if (program==1) {

        lcd.print("1-Chlamydia");
        message("25min  #-start");
        delay(3000);
          do{
              key = keypad.getKey();
              if (key=='*') digitalWrite(powerPin, LOW);
          } while (key=='#');
          wipersON();
          //program1();
          wipersOFF();
          digitalWrite(powerPin, LOW);

    }

    if (program==2) {

        lcd.print("2-Borelioza");
        message("30min  #-start");
        delay(3000);
          do{
              key = keypad.getKey();
              if (key=='*') digitalWrite(powerPin, LOW);
          } while (key=='#');
          wipersON();
          //program2();
          wipersOFF();
          digitalWrite(powerPin, LOW);

    }

    if (program==3) {

        lcd.print("3-Candida");
        message("21min  #-start");
        delay(3000);
          do{
              key = keypad.getKey();
              if (key=='*') digitalWrite(powerPin, LOW);
          } while (key=='#');
          wipersON();
          //program3();
          wipersOFF();
          digitalWrite(powerPin, LOW);

    }
	return 0;
}


int key_D(){
// Setting new vmin and vampl and saving in EEPROM

	get_v_EEPROM();
	int vampl = inputVal("Input vampl", last_v_ampl,3);
	int vmin = inputVal("Input vmin", last_v_min, 3);
	Freq = inputVal("Input freq", 100000);
	message("Calibrating...");
	byte wiper0 = calib_gain_wiper_ampl(vampl, Freq);
	byte wiper1 = calib_setp_wiper_vmin(vmin);
	if ( wiper0 + wiper1 ){
		//Save last vampl and vmin
		set_v_EEPROM();
		message("Saved in memory",0);
	} else {
		message("Wiper's error!",0);
	}
	message("w0:");
	lcd.print(wiper0);
	lcd.print("  w1:");
	lcd.print(wiper1);
	delay(3000);

	return 0;
}

int key_asterix(){
//Manually turn device off
	// ret=inputOption("Turn off?","yes","no","maybe");
	int ret=inputVal("Off?  1-yes 0-no", 1);
	if (ret) digitalWrite(powerPin, LOW);

	return ret;
}

int key_hash(){



	return 0;
}

int key_0(){
	if ((byte)EEPROM.read(0)!=255 && (byte)EEPROM.read(0)!=0) {
		//User program execute

		if ( !( (wiper0 = calib_gain_wiper_ampl(400, 100000)) > 0  &&
				(wiper1 = calib_setp_wiper_vmin(200)) > 0                  ) ) {

			message("Error calibration");
		}
		message("User program ...");
		executeCmd("exe\n",true);
		off();
    } else {
    	message("No program in memory!");
    }

	return 0;
}

int key_1(){

	return 0;
}

int key_2(){

	return 0;
}

int key_3(){

	return 0;
}

int key_4(){

	return 0;
}

int key_5(){

	return 0;
}

int key_6(){

	return 0;
}

int key_7(){

	return 0;
}

int key_8(){

#ifdef ENCODER_PROTOTYPE
	// For service and test issues
	inputVal2();
#endif

	return 0;
}

int key_9(){
// For service and test issues
	int vampl = inputVal("Input vampl", last_v_ampl,3);
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
	delay(3000);
	return 0;
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
			key_hash();
			break;

		  case '0':
			key_0();
			break;

		  case '1':
			key_1();
			break;

		  case '2':
			key_2();
			break;

		  case '3':
			key_3();
			break;

		  case '4':
			key_4();
			break;

		  case '5':
			key_5();
			break;

		  case '6':
			key_6();
			break;

		  case '7':
			key_7();
			break;

		  case '8':
			key_8();
			break;

		  case '9':
			key_9();
			break;

	}
}

#endif /* MULTIZAP_MENU_H_ */

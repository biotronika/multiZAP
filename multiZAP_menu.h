/*
 * multiZAP_menu.h
 *
 *  Created on: 31 dec 2017
 *      Author: elektros230
 */

#ifndef MULTIZAP_MENU_H_
#define MULTIZAP_MENU_H_


//#include <Arduino.h>
//#include <multiZAP_def.h>
#include <multiZAP_calib.h>
#include <multiZAP_lcd.h>
//#include <bioZAP_func.h>



int key_A(){
    Freq = inputVal("Input frequency", 783);
    if (Freq<=0) {
    	return 0;
    }
    int period = inputVal("Input period", 10);
    message(String(Freq));
    wipersON();
    Serial.print("freq ");
    Serial.print(Freq);
    Serial.print(" ");
    Serial.println(period);
    freq(Freq, period);
    wipersOFF();

	return 0;
}

int key_B(){

    // Select program
    byte program = inputVal("Wybierz prog 1-3");

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

    if (program==9) {
        //ds1803.set_wiper0(wiper0);
        //ds1803.set_wiper1(wiper1);
      	//Start with default parameters
      	if (ds1803.get_wiper0()==0){
      		calib_gain_wiper_ampl(400, 100);
      	}


        digitalWrite(buzerPin, HIGH);
        delay(200);
        digitalWrite(buzerPin, LOW);
        currentFreq++;
        if (currentFreq > 4)  currentFreq = 0;
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(freqs[currentFreq]);
        //ad9850 << freqs[currentFreq];
        calib_gain_wiper_ampl(400, /*100,*/ freqs[currentFreq]);
        calib_setp_wiper_vmin(100);



    }






	return 0;
}


int key_C(){
	if ((byte)EEPROM.read(0)!=255 && (byte)EEPROM.read(0)!=0) {
      //User program execute
      executeCmd("exe\n",true);
      //off();
    } else {
    	message("No program in memory!");
    }

	return 0;
}


int key_D(){
	/*TODO: Configuration parameters
	 * vmin - EEPROM
	 * vampl - EEPROM
	 * auto_turn_off_time - EEPROM
	 * AD985x 0/1 - EEPROM
	 * default_program - EEPROM
	 * multiZAP -/+/++ - EEPROM
	 *
	 */

	return 0;
}

int key_asterix(){
//Manually turn device off
	int ret=inputVal("Off?  1-yes 0-no", 1);
	if (ret) digitalWrite(powerPin, LOW);

	return ret;
}

int key_hash(){



	return 0;
}

int key_0(){



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
	int vampl = inputVal("Input vampl", 400);
	int vmin = inputVal("Input vmin", 200);
	int Freq = inputVal("Input freq", 100000);
	message("Calibrating...");
	int wiper0= calib_gain_wiper_ampl(vampl, Freq);
	int wiper1= calib_setp_wiper_vmin(vmin);
	message("w0:");
	lcd.print(wiper0);
	lcd.print("  w1:");
	lcd.print(wiper1);
	return 0;
}





#endif /* MULTIZAP_MENU_H_ */

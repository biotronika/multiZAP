// multiZAP See: biotronika.pl

#include "Arduino.h"
#include "multiZAP_menu.h"
#include "bioZAP_func.h"
//#include <avr/pgmspace.h>

byte refresh = 0;
//unsigned long lastCmdLineShowed = 0;

void setup (){

	pinMode(buzerPin, OUTPUT);
	//pinMode(powerPin,  OUTPUT);

	//Check if started with key # pressed or not
	pinMode(powerPin,  INPUT);
	pcConnection = (bat() < USB_POWER_SUPPLY_LEVEL);

	if ( !(digitalRead(powerPin)==HIGH) && !pcConnection ) {


		pinMode(powerPin,  OUTPUT);
		digitalWrite(powerPin, HIGH);
		lcd_init();

		rechargeBattery();

	} else {

		//Turn on
		pinMode(powerPin,  OUTPUT);
		digitalWrite(powerPin, HIGH);
		lcd_init();
	}


	//Turning on is done.
	beep(50);
	//for (int j=0; j<3;j++){ beep(50); wait(100); }


	Wire.begin();

	//Initialize generator & pots
	wipersOFF();





	if (!pcConnection) {
		Wire.begin();

		//Initialize generator & pots
		wipersOFF();

		//Calibrating battery level
		minBatteryLevel=initBatteryLevel();

		//Get last_v_ampl and last_v_min from EEPROM
		get_v_EEPROM();

		//Calibration
//		message("Calibrating...");
		message(0);

		wiper0 = calib_gain_wiper_ampl(last_v_ampl, 10000);
		wiper1 = calib_setp_wiper_vmin(last_v_min);

		if ( wiper0 * wiper1 == 0 ) {

/*			message("Error calibration",0);
			message("w0:     w1:",1);
			lcd.setCursor(4, 1);
			lcd.print(wiper0);
			lcd.setCursor(11, 1);
			lcd.print(wiper1);*/
			message(9);
			do{
				key = keypad.getKey();
			} while (key==NO_KEY);
			digitalWrite(powerPin, LOW);
		}
		wipersOFF();

	} else {

		//Initialize serial communication
		Serial.begin(9600);
	    Serial.println(WELCOME_SCR);
	    //Serial.print("Device ");
	    Serial.print(HRDW_VER);
	    Serial.print(" ");
	    Serial.println(SOFT_VER);
	    Serial.print(">");
	}

	lcd_hello(pcConnection);

	lastOperationMillis=millis();
}

void loop(){


 	//if (!pcConnection) {

		key = keypad.getKey();

		if (key != NO_KEY){
			beep(100);

			if (pcConnection) {
				//message("Turning off -",0);
				//message("unplug USB cable",1);
				message(1,0);
				message(2,1);
				digitalWrite(powerPin, LOW);
				while(1);
			} else {
				keyPressed(key);
				refresh=1;// Refresh command LCD line
			}


			beep(100);
			if (!freqStartMillis) lcd_hello(pcConnection); //Show menu when not executing freq

			//Count to off
			lastOperationMillis=millis();

			key=NO_KEY;

		}


	if (programStartMillis && !pcConnection){
		//Next command
		//if (freqStartMillis==0){
		if (millis()>freqStopMillis){
			exe(adr); //next command

			lastOperationMillis=millis();

			if (!adr) {
				programStartMillis = 0;
				programStopMillis = 0;
				freqStartMillis = 0;
				freqStopMillis = 0;
				wipersOFF();

				lcd_hello(pcConnection);

			} else {
				refresh = 1; //Refresh command line on LCD screen
			}

		}

		if (refresh) {
			message(line.substring(0, line.length()-1) ,1);
			refresh = 0;
		}

		/*if ( millis() > lastCmdLineShowed + 1000 ) {
			lastCmdLineShowed = millis();

			//message (line.substring(0, line.length()-1) ,1);
		}*/

	}

	if (freqStartMillis && !pcConnection){
		//freqStartTime<>0 means we are during freq function

/*		if (millis()>freqStopMillis){
			//freqStopMillis=0;
			//freqStartMillis=0;
			;
		}*/

		if (programStopMillis) {
			//Show progress bar of full program
			progressBar( (programStopMillis-programStartMillis)/1000 , (programStopMillis-millis())/1000 );



		} else {
			//Show progress bar of freq command
			progressBar( (freqStopMillis-freqStartMillis)/1000 , (freqStopMillis-millis())/1000 );

			if (millis()>freqStopMillis){
				//Stop freq function

				freqStartMillis = 0;
				freqStopMillis = 0;
				wipersOFF();

				lcd_hello(pcConnection);
				if (!programStartMillis) beep(200); //Beep only in A_key funct.
				lastOperationMillis=millis();
			}

		}

	}

	// PC controlled program
	if (stringComplete && pcConnection) {

		//Restart timeout interval to turn off.
		//startInterval=millis();

		executeCmd(inputString, true);
		Serial.print('>'); //Cursor for new command

		// clear the command string
		inputString = "";
		stringComplete = false;
		lastOperationMillis=millis();
	}


	//Auto turn off
	if (  !freqStartMillis && ( lastOperationMillis + pauseTimeOut < millis() )  ){
		beep(500);
		message(1,0);
		message(2,1);
		digitalWrite(powerPin, LOW);
		while(1);
	}

}











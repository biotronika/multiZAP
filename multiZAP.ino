// multiZAP See: biotronika.pl

#include "Arduino.h"
#include "multiZAP_menu.h"
#include "bioZAP_func.h"




void setup (){

	pinMode(buzerPin, OUTPUT);
	//pinMode(powerPin,  OUTPUT);

	//Check if started with key # pressed or not
	pinMode(powerPin,  INPUT);
	pcConnection = (bat() < USB_POWER_SUPPLY_LEVEL);

	if ( !(digitalRead(powerPin)==HIGH) && !pcConnection ) {
	//if ( false) {

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


	Wire.begin();

	//Initialize generator & pots
	wipersOFF();

	//Calibrating battery level
	minBatteryLevel=initBatteryLevel();

	//Turning on is done.
	beep(50);
	//for (int j=0; j<3;j++){ beep(50); wait(100); }



	//Count to off
	startInterval=millis();

	//Get last_v_ampl and last_v_min from EEPROM
	get_v_EEPROM();

	if (!pcConnection) {
		//Calibration
		message("Calibrating...");
		if ( !( (wiper0 = calib_gain_wiper_ampl(last_v_ampl, 100000)) > 0  &&
				(wiper1 = calib_setp_wiper_vmin(last_v_min)) > 0                  ) ) {

			message("Error calibration");
			do{
				key = keypad.getKey();
			} while (key==NO_KEY);
		}
		wipersOFF();

	} else {

		//Initialize serial communication
		Serial.begin(9600);
	    Serial.println(WELCOME_SCR);
	    Serial.print("Device ");
	    Serial.print(HRDW_VER);
	    Serial.print(" ");
	    Serial.println(SOFT_VER);
	}

	lcd_hello(pcConnection);
}

void loop(){


 	//if (!pcConnection) {

		key = keypad.getKey();

		if (key != NO_KEY){

			keyPressed(key);

			lcd_hello(pcConnection);
			beep(100);

			//Count to off
			startInterval=millis();

			key=NO_KEY;

		}


	if (freqStartMillis){
		//freqStartTime<>0 means we are during freq function


		if (programStartMillis) {
			//Show progress bar of full program
			progressBar( (programStopMillis-programStartMillis)/1000 , (programStopMillis-millis())/1000 );

			if (millis()>freqStopMillis){
				//Next command
				;
			}

		} else {
			//Show progress bar of freq command
			progressBar( (freqStopMillis-freqStartMillis)/1000 , (freqStopMillis-millis())/1000 );

			if (millis()>freqStopMillis){
				//Stop freq function

				freqStartMillis = 0;
				freqStopMillis = 0;
				wipersOFF();

				lcd_hello(pcConnection);
				beep(200);
			}

		}





	}

	// PC controlled program
	if (stringComplete && pcConnection) {

		//Restart timeout interval to turn off.
		startInterval=millis();

		executeCmd(inputString, true);
		Serial.print('>'); //Cursor for new command

		// clear the command string
		inputString = "";
		stringComplete = false;
	}


}











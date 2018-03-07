// multiZAP See: biotronika.pl

#include "Arduino.h"
#include "multiZAP_menu.h"
#include "bioZAP_func.h"
//#include <avr/pgmspace.h>



byte refresh = 0;
//unsigned long lastCmdLineShowed = 0;

void setup (){

	//User socket pin3
	pinMode(pin3Pin, OUTPUT);

	pinMode(buzerPin, OUTPUT);
	//pinMode(powerPin,  OUTPUT);

	//Check if started with key # pressed or not
	pinMode(powerPin,  INPUT);


	pcConnection = (bat() < USB_POWER_SUPPLY_LEVEL);


	if ( !(digitalRead(powerPin)==HIGH) && !pcConnection ) {
	// !!! digitalRead(powerPin) you can do before turnOn_init()

		//pinMode(powerPin,  OUTPUT);
		//digitalWrite(powerPin, HIGH);

		turnOn_init();

		rechargeBattery();

	} else {

		//Turn on
		//pinMode(powerPin,  OUTPUT);
		//digitalWrite(powerPin, HIGH);

		turnOn_init();
	}

#ifdef SERIAL_DEBUG
	pcConnection = false;
#endif

	//Turning on is done.
	beep(50);

	Wire.begin();

	//Initialize generator & pots
	wipersOFF();

	delay(300);
	//readLabelPointers(0);

	if (!pcConnection) {
		Wire.begin();

		//Initialize generator & pots
		wipersOFF();

		//Calibrating battery level
		//minBatteryLevel = MIN_BATTERY_LEVEL; //initBatteryLevel();

		//Get last_v_ampl and last_v_min from EEPROM
		get_v_EEPROM();

		//Calibration
		message(0); //"Calibrating..."

		wiper0 = calib_gain_wiper_ampl(last_v_ampl, 100000);
		wiper1 = calib_setp_wiper_vmin(last_v_min);

		if ( (wiper0 * wiper1) == 0 ) {

			message(9,0);  //"Error calibration"
			message(16,1); //"w0:     w1:      "

			lcd.setCursor(4, 1);
			lcd.print(wiper0);
			lcd.setCursor(11, 1);
			lcd.print(wiper1);

			do{
				key = keypad.getKey();
			} while (key==NO_KEY);
			digitalWrite(powerPin, LOW);
		}
		wipersOFF();

#ifndef SERIAL_DEBUG
	} else {
#endif

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

		if (millis()>freqStopMillis){
#ifdef SERIAL_DEBUG
	  	//Serial.print("start adr: ");
		//Serial.println(adr);
#endif
			exe(adr, currentProgram); //currentProgram); //next command

			lastOperationMillis=millis();

			if (!adr) {
				programStartMillis = 0;
				programStopMillis = 0;
				freqStartMillis = 0;
				freqStopMillis = 0;
				wipersOFF();

				currentProgram = -1;
				lcd_hello(pcConnection);

			} else {
				refresh = 1; //Refresh command line on LCD screen
			}

		}

		if (refresh) {
			message(line.substring(0, line.length()-1) ,1);
			refresh = 0;
		}


	}

	if (freqStartMillis /*&& !pcConnection*/){
		//freqStartTime<>0 means we are during freq function


		if (programStopMillis) {
			//Show progress bar for full program
			progressBar( long((programStopMillis-programStartMillis)/1000) , long((programStopMillis-millis())/1000) );



		} else {
			//Show progress bar for freq command
			progressBar( (freqStopMillis-freqStartMillis)/1000 , (freqStopMillis-millis())/1000 );

			if (millis()>freqStopMillis){
				//Stop freq function

				freqStartMillis = 0;
				freqStopMillis = 0;
				wipersOFF();
				currentProgram=-1;


				lcd_hello(pcConnection);
				if (!programStartMillis) beep(200); //Beep only in A_key funct.
				lastOperationMillis=millis();
			}

		}

	}

	// PC controlled program
	if (stringComplete && pcConnection) {

		executeCmd(inputString);

		//Cursor for new command
		Serial.print('>');

		// clear the command string
		inputString = "";
		stringComplete = false;

		//Restart timeout interval to turn off.
		lastOperationMillis=millis();
	}


	//Auto turn off
	if (  !freqStartMillis && ( lastOperationMillis + pauseTimeOut < millis() )  ){

		beep(500);

		message(2); //"unplug USB cable"

		digitalWrite(powerPin, LOW);

		while(1);
	}


	//Check battery voltage
	checkBattLevel();

}











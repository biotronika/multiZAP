// multiZAP See: biotronika.pl

#include "Arduino.h"
#include <multiZAP_menu.h>
//#include <bioZAP_func.h>

boolean pcConnection = false;


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

#ifdef ENCODER_PROTOTYPE
	encoder_begin(encoderPin_A,encoderPin_B);
#endif

	Wire.begin();

	//Initialize generator & pots
	wipersOFF();

	//Calibrating battery level
	minBatteryLevel=initBatteryLevel();

	//Turning on is done.
	//beep(50);
	for (int j=0; j<3;j++){ beep(50); wait(100); }

	//Initialize serial communication
	Serial.begin(9600);
    Serial.println(WELCOME_SCR);
    Serial.print("Device ");
    Serial.print(HRDW_VER);
    Serial.print(" ");
    Serial.println(SOFT_VER);

	//Count to off
	startInterval=millis();

	//Calibration
	message("Calibrating...");
    //                                      4.0V 1.0V 1kHz
	if ( !( (wiper0 = calib_gain_wiper_ampl(400, 100000)) > 0  &&
		    (wiper1 = calib_setp_wiper_vmin(100)) > 0                  ) ) {

		message("Error calibration");
		do{
			key = keypad.getKey();
		} while (key==NO_KEY);
	}

	//beep(500); wait(500);


	wipersOFF();

	lcd_hello(pcConnection);

	//beep(500); wait(500);
}

void loop(){

	//beep(500); wait(500);

	key = keypad.getKey();

	if (key != NO_KEY){

		switch (key){

		  	  case 'A':
		  		  key_A();
		  		  key=NO_KEY;
		  		  break;

		  	  case 'B':
		  		  //key_B();
		  		  lcd.clear();
		  		  lcd.print('B');
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
		lcd_hello(pcConnection);
		beep(200);

		//Count to off
		startInterval=millis();

		key=NO_KEY;

	}

	// PC controlled program
	if (stringComplete) {

		//Restart timeout interval to turn off.
		startInterval=millis();

		executeCmd(inputString, true);
		Serial.print('>'); //Cursor for new command

		// clear the command string
		inputString = "";
		stringComplete = false;
	}

      //if (millis()-startInterval > pauseTimeOut) off();

}











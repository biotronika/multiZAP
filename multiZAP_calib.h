/*
 * multiZAP_calib.h
 *
 *  Created on: 24 dec 2017
 *      Author: elektros230
 */

#ifndef CALIB_H_
#define CALIB_H_

#include <Arduino.h>
//#include "bioZAP_func.h"
//#include "DS1803.h"
//#include "AD9850.h"

#include "multiZAP_def.h"

#define EEPROM_VAMPL_ADDRESS 1019				// v_ampl - multiZAP
#define EEPROM_VMIN_ADDRESS 1021				// v_min - multiZAP

#define VMIN_MIN 50								//vmin & vampl extremes
#define VMIN_MAX 400
#define VAMPL_MIN 100
#define VAMPL_MAX 800




/*************************************************************************************/

//Function prototypes
byte calib_gain_wiper_ampl(int v_ampl=400,  long freq=100000);
byte calib_setp_wiper_vmin(int v_min);
byte calib_flat_wiper_vmin(int v_min);
int last_v_ampl = 0;
int last_v_min = 0;


/*************************************************************************************/


void get_v_EEPROM(){
	EEPROM.get( EEPROM_VAMPL_ADDRESS, last_v_ampl );
	EEPROM.get( EEPROM_VMIN_ADDRESS, last_v_min );

	//For new devices were EEPROM memory is not set properly
	last_v_ampl= constrain( last_v_ampl, VAMPL_MIN, VAMPL_MAX );
	last_v_min = constrain( last_v_min, VMIN_MIN, VMIN_MAX );
}

void set_v_EEPROM(){
	EEPROM.put( EEPROM_VAMPL_ADDRESS, last_v_ampl );
	EEPROM.put( EEPROM_VMIN_ADDRESS, last_v_min );

}


int _getVampl(){

	//Delay for extremely low frequency
	int _delay =0;
	if (ad9850.get_frequency() < 2000){
		_delay = 1;
	}

	//First start point read
	int x = analogRead(outVoltagePin);

	//Starting to measure values
	float _vmin= x * ONE_GRADE;
	float _vmax= x * ONE_GRADE;

	//Measuring ampl. pk-pk value
	for( int i=0; i<200; i++){
		x = analogRead(outVoltagePin);

#ifdef SERIAL_DEBUG
/*
		Serial.print("i:");
		Serial.println(i);
		Serial.print(" _vmin:");
		Serial.println(_vmin);
		Serial.print(" _vmax:");
		Serial.println(_vmax);
		Serial.print(" x:");
		Serial.println(x);
*/

#endif

		if ( x * ONE_GRADE > _vmax) {
			_vmax = x * ONE_GRADE;
		}

		if ( x * ONE_GRADE < _vmin) {
			_vmin = x * ONE_GRADE;
		}

		if(_delay>0) delayMicroseconds(659);

		//Against harmonics measurement method
		//if (i>100) delayMicroseconds(random(200));
		if (i>150) {
			delayMicroseconds(17);
		} else if (i>100) {
			delayMicroseconds(37);
		} else if (i>50) {
			delayMicroseconds(83);
		}


	}
	return (_vmax-_vmin) * 100.0;

}


int _getVMin(){

	//Delay for extremely low frequency
	int _delay = 0;
	if (ad9850.get_frequency() < 2000){
		_delay = 1;
	}

	//First start point read
	int x = analogRead(outVoltagePin);

	//Starting to measure values
	float _vmin = x * ONE_GRADE;


	//Measuring vmin value
	for( int i=0; i<200; i++){
		x = analogRead(outVoltagePin);

		if ( x * ONE_GRADE < _vmin) {
			_vmin = x * ONE_GRADE;
		}

		if(_delay>0) delayMicroseconds(659);

		//Against harmonics measurement method
		//if (i>100) delayMicroseconds(random(200));
		if (i>150) {
			delayMicroseconds(17);
		} else if (i>100) {
			delayMicroseconds(37);
		} else if (i>50) {
			delayMicroseconds(83);
		}
	}
	return _vmin * 100;
}


byte calib_gain_wiper_ampl(int v_ampl,  long freq){
	// Find v_min level suitable to begin calibration
	// Work with proper gain and frequency set

	//Searching point range
	byte i_min = 0;
	byte i_max = 255;
	byte i = 128; //ds1803.get_wiper0();
	//if (i>i_max) i_max=i;


	if (v_ampl<=800 && v_ampl>=100){
	last_v_ampl = v_ampl;

		if (v_ampl>=600){
			//Calibrate v_ampl with v_min=100 - 1.0[V]
			ds1803.set_wiper1( calib_flat_wiper_vmin(100) );
		} else {
			//Calibrate v_ampl with v_min=200 - 2.0[V]
			ds1803.set_wiper1( calib_flat_wiper_vmin(200) );
		}

		//Set work frequency - up to 100kHz
		ad9850.set_frequency(freq); // 1000 = 10.00Hz
		//TODO: Do support for more than 100kHz

		ds1803.set_wiper0(i);

		do {
			int a =_getVampl();
#ifdef SERIAL_DEBUG
		Serial.println(a);
#endif
			//Check
			if ( v_ampl > a ){
				i_min=i;
			} else {
				i_max=i;
			}

			//Calculate next point
			i = i_min + round((i_max-i_min)/2.0);

			ds1803.set_wiper0(i); //Set calculated value to setpoint wiper
			//delay(20);

		} while ( abs(i_max-i_min) > 1 ); //1 or 0 are good enough

		return i;
	} else {
		return 0;
	}



}

byte calib_setp_wiper_vmin(int v_min){
// Find v_min level suitable to begin calibration
// Work with proper gain and frequency set

	//Searching point range
	byte i_min = 0;
	byte i_max = 100;
	byte i = ds1803.get_wiper1();
	if ( i>i_max ) i_max=i;

	v_min = constrain( v_min, VMIN_MIN, VMIN_MAX );

	//if ( v_min<=400 && v_min>=50 ){

		last_v_min = v_min;

		do {

			//Check
			if ( v_min > _getVMin() ){
				i_min=i;
			} else {
				i_max=i;
			}

			//Calculate next point
			i=i_min+round( (i_max-i_min)/2.0 );

			ds1803.set_wiper1(i); //Set calculated value to setpoint wiper

		} while ( abs(i_max-i_min)>1 ); //1 or 0 are good enough

		if ( i_min > 0){

			//Linearity v_min
			if ( v_min<=100 && last_v_ampl<=200 ) {
				ds1803.set_wiper1(i_min+5);
				i=i_min+5;
			}
			//DONE: remove form calib.h
			//wiper0 = ds1803.get_wiper0();
			//wiper1 = ds1803.get_wiper1();

			return i;
		} else {
			return 0;
		}


		return i;
/*	} else {
		return 0;
	}*/

}

byte calib_flat_wiper_vmin(int v_min){
//Find v_min level suitable to begin calibration

	ds1803.set_wiper0(0); // Gain to 0
	ad9850.powerDown();   // Generator to off
	unsigned int aim = v_min/(ONE_GRADE*100);  //Pin outVoltagePin needed level

	//Searching point range
	byte i_min = 0;
	byte i_max = 100;
	byte i = 50;

	//last_v_min = v_min;

	//Averaging algorithm
	unsigned int x=0;
	byte probes=10;

	ds1803.set_wiper1(i);

	do {

		//Averaging
		x=0;
		for (byte j=0; j<probes; j++){
			x+=analogRead(outVoltagePin)/probes;
		}

		//Check
		if (aim > x){
			i_min=i;
		} else {
			i_max=i;
		}

		//Calculate next point
		i=i_min+round((i_max-i_min)/2);

		ds1803.set_wiper1(i); //Set calculated value to set point wiper

	} while (abs(i_max-i_min)>1); //1 or 0 are good enough

	return i;
}


#endif /* CALIB_H_ */

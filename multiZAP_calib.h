/*
 * multiZAP_calib.h
 *
 *  Created on: 24 dec 2017
 *      Author: elektros230
 */

#ifndef CALIB_H_
#define CALIB_H_

#include <Arduino.h>
#include "bioZAP_func.h"
//#include "DS1803.h"
//#include "AD9850.h"
//#include "multiZAP_def.h"




/*************************************************************************************/

//Function prototypes
byte calib_gain_wiper_ampl(int v_ampl=400, /*int v_min=200,*/ double freq=100000);
byte calib_setp_wiper_vmin(int v_min);
byte calib_flat_wiper_vmin(int v_min);
//byte wiper0 = 0;
//byte wiper1 = 0;

/*************************************************************************************/

int _last_v_ampl = 0;


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
	return (_vmax-_vmin) * 100;

}

int _getVMax(){

	//Delay for extremely low frequency
	int _delay = 0;
	if (ad9850.get_frequency() < 2000){
		_delay = 1;
	}
	//First start point read
	int x = analogRead(outVoltagePin);

	//Starting to measure values
	float _vmax = x * ONE_GRADE;


	//Measuring vmax value
	for( int i=0; i<200; i++){
		x = analogRead(outVoltagePin);

		if ( x * ONE_GRADE > _vmax) {
			_vmax = x * ONE_GRADE;
		}

		if(_delay>0) delay(_delay);

	}
	return _vmax * 100;
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


byte calib_gain_wiper_ampl(int v_ampl, /*int v_min,*/ double freq){
	// Find v_min level suitable to begin calibration
	// Work with proper gain and frequency set

	//Searching point range
	byte i_min = 0;
	byte i_max = 255;
	byte i = 128; //ds1803.get_wiper0();
	//if (i>i_max) i_max=i;

	//wiper0 = 0;
	//wiper1 = 0;
	_last_v_ampl = 0;

	if (v_ampl<=800 && v_ampl>=100){

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

			//Check
			if ( v_ampl > _getVampl() ){
				i_min=i;
			} else {
				i_max=i;
			}

			//Calculate next point
			i = i_min + round((i_max-i_min)/2.0);

			ds1803.set_wiper0(i); //Set calculated value to setpoint wiper
			//delay(20);

		} while ( abs(i_max-i_min) > 1 ); //1 or 0 are good enough

		//DONE: remove linearity to calib_setp_wiper_vmin
//		byte i_min=calib_setp_wiper_vmin(v_min);
/*
		if ( i_min > 0){

			//Linearity v_min
			if (v_min<=100 && v_ampl<=200) {
				ds1803.set_wiper1(i_min+5);
				i=i_min+5;
			}
			//TODO: remove form calib.h
			wiper0 = ds1803.get_wiper0();
			wiper1 = ds1803.get_wiper1();

			return i;
		} else {
			return 0;
		}
*/
		_last_v_ampl = v_ampl;
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

	if ( v_min<=400 && v_min>=50 ){

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
			if ( v_min<=100 && _last_v_ampl<=200 ) {
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
	} else {
		return 0;
	}

}

byte calib_flat_wiper_vmin(int v_min){
//Find v_min level suitable to begin calibration

	ds1803.set_wiper0(0); // Gain to 0
	ad9850.powerDown();   // Generator to off
	uint16_t aim = v_min/(ONE_GRADE*100);  //Pin outVoltagePin needed level

	//Searching point range
	byte i_min = 0;
	byte i_max = 100;
	byte i = 50;

	//Averaging algorithm
	uint16_t x=0;
	uint8_t probes=10;

	ds1803.set_wiper1(i);

	do {

		//Averaging
		x=0;
		for (int j=0; j<probes; j++){
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

		ds1803.set_wiper1(i); //Set calculated value to setpoint wiper

	} while (abs(i_max-i_min)>1); //1 or 0 are good enough

	return i;
}


#endif /* CALIB_H_ */

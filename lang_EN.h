/*
 * lang_EN.h
 *
 *  Created on: 13 Jan 2018
 *      Author: elektros230
 */

#ifndef LANG_EN_H_
#define LANG_EN_H_

//Translate to your own language
//Saved in ROM
const char msg[][16] PROGMEM   =  {

//		"-Exacly 16 cols-"
		"Calibrating...  ",	//	0
		"Upload scrpt and",	//	1
		"unplug USB cable",	//	2
		"Input period    ", //	3
		"Input vampl     ", // 	4
		"Input vmin      ",	// 	5
		"Saved in memory ", // 	6
		"Wiper's error!  ",	// 	7
		"Trn off *yes #no", // 	8
		"Error calibrat. ",	// 	9
		"User program ...",	//	10
		"No EEPROM script",	//	11
		"Input frequency ",	//  12
		HRDW_VER,			//  13
		SOFT_VER,			//	14
		"                ",	//	15 Empty string
		"w0:     w1:     "};//  16


#endif /* LANG_EN_H_ */

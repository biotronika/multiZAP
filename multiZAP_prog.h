/*
 * multiZAP_programs.h
 *
 *  Created on: 31 dec 2017
 *      Author: elektros230
 */

#ifndef MULTIZAP_PROGRAMS_H_
#define MULTIZAP_PROGRAMS_H_
#include <avr/pgmspace.h>
#include "multiZAP_def.h"

const char internalProgram[] PROGMEM   = {

		":1\n"
		"#Chlamydia 25min\n"
		"wait 3000\n"
		"pbar 100 1500\n"
		"beep 100\n"
		"freq 471050 120\n"
		"freq 47100 60\n"
		"freq 47150 60\n"
		"freq 47200 60\n"
		"freq 47900 120\n"
		"freq 62000 120\n"
		"freq 94010 120\n"
		"freq 94290 120\n"
		"freq 188010 120\n"
		"freq 188590 120\n"
		"freq 376030 120\n"
		"freq 377170 120\n"
		"freq 752050 120\n"
		"freq 754340 120\n"
		"beep 500\n"
		"off\n"

		":2\n"
#ifdef EN_H_
		"#Lyme disease 30min.\n"   //Lyme disease
#else
		"#Borelioza 30min.\n"   //Lyme disease
#endif
		"wait 3000\n"
		"pbar 100 1800\n"
		"beep 100\n"
		"freq 20300 180\n"
		"freq 41200 180\n"
		"freq 41400 180\n"
		"freq 58900 180\n"
		"freq 64000 180\n"
		"freq 66700 180\n"
		"freq 84000 180\n"
		"freq 107200 180\n"
		"freq 108700 180\n"
		"freq 855400 180\n"
		"beep 500\n"
		"off\n"

        ":3\n"
		"#Candida 21min.\n"
		"wait 3000\n"
		"pbar 100 1260\n"
		"beep 100\n"
		"freq 2048500 180\n"
		"freq 5115500 180\n"
		"freq 5115600 180\n"
		"freq 5394000 180\n"
		"freq 5891400 180\n"
		"freq 5891600 180\n"
		"freq 8874000 180\n"
		"beep 500\n"
		"off\n"

        ":4\n"
		"#Empty\n"
		"wait 3000\n"
		"off\n"

        ":5\n"
		"#Empty\n"
		"wait 3000\n"
		"off\n"

        ":6\n"
		"#Empty\n"
		"wait 3000\n"
		"off\n"

        ":7\n"
		"#Empty\n"
		"wait 3000\n"
		"off\n"

        ":8\n"
#ifdef EN_H_
		"#Earth rhythm 8min.\n"
#else
		"#Rytm Ziemi 8min.\n"
#endif
		"wait 3000\n"
		"beep 100\n"
		"freq 783 480\n"
		"beep 500\n"
		"off\n"


        ":9\n"
		"#TEST\n"
		"wait 3000\n"
		"pbar 100 30\n"
		"beep 100\n"
		"freq 783 5\n"
		"freq 1000 5\n"
		"freq 20000 5\n"
		"freq 300000 5\n"
		"freq 4000000 5\n"
		"freq 58916000 5\n"
		"beep 500\n"
		"off\n"

		"@"

};



#endif /* MULTIZAP_PROGRAMS_H_ */

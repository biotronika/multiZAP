/**********************************************************
 * bioZAP_func.h
 * 		multiZAP and free-PEMF devices library.
 * 		BIOzap script therapy language.
 * 		See https://biotronika.pl
 *
 *  Created on: 2017-01-01
 *      Author: elektros230 & hehaka
 **********************************************************/

#ifndef BIOZAP_FUNC_H_
#define BIOZAP_FUNC_H_

//Target device definition
//TODO: Automate switch based on project include definition file
#define MULTIZAP
//#define SERIAL_DEBUG


#include <Arduino.h>

#include "multiZAP_def.h"
#include "DS1803.h"
#include "AD9850.h"
#include "multiZAP_prog.h"
#include <avr/pgmspace.h>



//BIOzap
#define WELCOME_SCR "Free BIOzap interpreter welcome! See https://biotronika.pl"
#define PROGRAM_SIZE 1000     	// Maximum program size
#define PROGRAM_BUFFER 128    	// SRAM buffer size, used for script loading  TODO lack of memory
#define MAX_CMD_PARAMS 3      	// Count of command params
#define LCD_SCREEN_LINE 1     	// LCD user line number, -1 = no lcd, 0 = first, 1= second
#define LCD_PBAR_LINE 0			// LCD progress bar line
#define MIN_FREQ_OUT 1        	// 0.01 Hz

#ifdef FREE_PEMF
#define MAX_FREQ_OUT 5000     					// 50Hz
#endif

#ifdef MULTIZAP
#define MAX_FREQ_OUT 90000000 					// 900kHz
#define EEPROM_VAMPL_ADDRESS 1019				// v_ampl - multiZAP
#define EEPROM_VMIN_ADDRESS 1021				// v_min - multiZAP
#endif

#define SCAN_STEPS 100        	// For scan function purpose - default steps
#define MAX_LABELS 9          	// Number of labels in script therapy


#define INPUT_SIGN '>'			//Serial and LCD input sign


//Battery staff
#define batPin PIN_A7                           // Analog-in battery level
#define BATTERY_VOLTAGE_RATIO 0.153             // Include divider 10k/4,7k resistors. 5V*(10k+4,7k)/4,7k = 0,0153 (x10)
#define EEPROM_BATTERY_CALIBRATION_ADDRESS 1023 // Memory address of battery correction factor - 100 means RATIO x 1,00

#define MIN_BATTERY_LEVEL 90                    // 90 means 9.0 V  (x10), less then that turn off
#define USB_POWER_SUPPLY_LEVEL 65               // Maximum USB voltage level means 6.5V

// During charging battery minimum increasing voltage after x milliseconds.
const unsigned long checkDeltaBatteryIncreasingVoltageTime = 600000UL;

//BIOzap
String inputString = "";                // a string to hold incoming serial data
String param[MAX_CMD_PARAMS];           // param[0] = cmd name
boolean stringComplete = false;         // whether the string is complete
boolean memComplete = false;
unsigned long lastFreq = MIN_FREQ_OUT;  // Uses scan function
int minBatteryLevel = 0;

byte wiper0 = 0;
byte wiper1 = 0;

unsigned long freqStartMillis = 0;
unsigned long freqStopMillis = 0;
unsigned long programStartMillis = 0;
unsigned long programStopMillis = 0;

long Freq = 100000; //783; //7.83Hz
int adr=0;
String line;
byte currentProgram = 0;




//Labels & jumps
//String labelName[MAX_LABELS];           	// TODO: Labels names e.g. :MY_LABEL
int labelPointer[MAX_LABELS+1];  				// TODO: Next line of label
int labelLoops[MAX_LABELS+1];    				// TODO: Number of left jump loops
//byte labelsPointer = 0;                 	// TODO: Pointer of end label table


//Serial buffer
char memBuffer[PROGRAM_BUFFER];

const unsigned long pauseTimeOut = 600000UL;    // 600000 Time of waiting in pause state as turn power off. (60000 = 1 min.)
//const unsigned int btnTimeOut = 5000UL;         // Choose therapy program time out. Counted form released button.


int programNo = -1;              // TODO: to reconstruct in free-PEMF deprecated: 0 = PC connection, 1= first program etc.
								// New: 0 = default program in memory, 1-9 or 1-3 = standard programs , -1 = PC

//function prototypes
int readEepromLine(int fromAddress, String &lineString);
void getParams(String &inputString);
int executeCmd(String cmdLine);
void eepromUpload(int adr = 0);
boolean readSerial2Buffer(int &endBuffer);
void rechargeBattery();
int initBatteryLevel();
void checkBattLevel();


void off();
void beep( unsigned int period);
void freq(unsigned long Freq, unsigned int period);
//void rec(unsigned int Freq, unsigned long period);  //deprecated
 //int rec();
 //int sin();
 int bat();
void wait( unsigned long period);
void exe(int &adr, byte prog=255);
void scan(unsigned long Freq, unsigned long period);
 int mem();
void ls();
void rm();
void cbat();
void pbar(byte percent, unsigned int period);
int jump(int labelNumber, int &adr);

void wipersON();
void wipersOFF();


void wipersON(){
  ds1803.set_wiper0(wiper0);
  ds1803.set_wiper1(wiper1);
}

void wipersOFF(){
  ds1803.set_wiper0(0);
  ds1803.set_wiper1(0);
  ad9850.powerDown();
}



String formatLine(int adr, String line){
  String printLine;
  printLine.reserve(22);
  printLine = "000"+String(adr,DEC);
  printLine = printLine.substring(printLine.length()-3,  printLine.length());
  printLine+=": "+line; //end marker for appending program
  return printLine;
}

int executeCmd(String cmdLine ){
// Main interpreter function
	//digitalWrite(powerPin, HIGH);
	//lcd.clear();
	//lcd.print("Executing program...");

	getParams(cmdLine);

    if ( param[0]=="mem" ) {
// Upload therapy to EEPROM
    	mem();


    } else if ( param[0]=="ls" ) {
//List therapy
    	ls();

    } else if (param[0].charAt(0)=='#') {
// Comment
    	;


    } else if (param[0].charAt(0)==':') {
// Label - setup for new label jump counter
    	b = param[0].substring(1).toInt();
    	if (b>0 && b<MAX_LABELS){
    		if(param[1].length()>1) {
    			labelLoops[b] = param[1].toInt();

#ifdef SERIAL_DEBUG
        Serial.print("label: ");
        Serial.print(b);
        Serial.print(" ");
        Serial.println(labelLoops[b]);
#endif
    		} else {
    			labelLoops[b] = -1; //Infinity loop
    		}
    	}


    } else if (param[0]==""){
// Emptyline
    	;


    } else if (param[0]=="rm"){
// Remove, clear script therapy from memory
    	rm();


    } else if (param[0]=="bat"){
// Print battery voltage
        //Serial.println(bat());


    } else if (param[0]=="beep"){
// Beep [time_ms]
   	    beep( param[1].toInt() );
        //Serial.println("OK");


    } else if (param[0]=="jump"){
// Jump [label number]

#ifdef SERIAL_DEBUG
        Serial.print("jump1 adr: ");
        Serial.println(adr);
#endif
   	    if (  jump(param[1].toInt(), adr)  )  {

#ifdef SERIAL_DEBUG
        Serial.print("jump2: ");
        Serial.println(param[1].toInt());
        Serial.print("jump2 adr: ");
        Serial.println(adr);
#endif
   	    	return adr;
   	    }




    } else if (param[0]=="off"){
// Turn off
    	off();


    } else if (param[0]=="wait"){
// Wait millis
    	delay(param[1].toInt());
      	//Serial.println("OK");


    } else if (param[0]=="freq" || param[0]=="rec" || param[0]=="sin"){
// Generate rectangle signal - rec [freq] [time_sec]
    	//TODO: Different result functions - to divide
    	freq(param[1].toInt(), param[2].toInt());
      	//Serial.println("OK");


    } else if (param[0]=="scan"){
// Scan from lastFreq  - scan [freq to] [time_ms]
    	scan(param[1].toInt(), param[2].toInt());
    	//Serial.println("Error: Not supported");


    } else if (param[0]=="pbar"){
// Progress bar - pbar [percent] [time_sec_to_end]
    	pbar(param[1].toInt(), param[2].toInt());
    	//Serial.println("OK");


    }  else {
//Unknown command

    	if (pcConnection) Serial.println("Unknown command: "+param[0]);

    }

    return 0;
}

///////////////////////////// bioZAP functions ///////////////////////////////


void pbar(byte percent, unsigned int period){
// Scaling progress bar on lcd, and show remaining time

	programStopMillis = millis() + long(period) * 1000;

	programStartMillis = constrain( programStopMillis - long(period) * 100000 / percent ,
									0,
									programStopMillis );
#ifdef SERIAL_DEBUG
	//Serial.println("pbar: ");
	//Serial.println(programStartMillis);
	//Serial.println(programStopMillis);
#endif


}


void cbat(){
// Calibrate battery voltage

	//Correction factor
	b = 100 * param[1].toInt()/(int(analogRead(batPin)*BATTERY_VOLTAGE_RATIO));

	EEPROM.put(EEPROM_BATTERY_CALIBRATION_ADDRESS, b);
	Serial.println("OK");
}

void rm(){
// Remove, clear script therapy from memory
	EEPROM.put(0, '@');

//	for(int i=0; i<PROGRAM_SIZE; i++){
//		EEPROM.put(i, 255);
		//if (!(i % 128)) Serial.print(".");
//	}
	//Serial.println("OK");
}

void ls(){
//List script therapy
	int adr=0;
	int endLine;
	String line;

	if (param[1]=="-n") {
		Serial.println("Adr  Command");

		while ((endLine = readEepromLine(adr,line)) && (adr<PROGRAM_SIZE) ){
		  Serial.print(formatLine(adr,line));
		  adr = adr + endLine;
		}

		//End marker (@) informs an user where to start appending of program
		if (adr<PROGRAM_SIZE) {
			Serial.println(formatLine(adr,"@"));
		}

	} else {

		for(int i=0; i<PROGRAM_SIZE; i++){
			char eeChar=(char)EEPROM.read(i);

			if ((eeChar=='@') || (eeChar==char(255))) {
				break;
			}

			Serial.print(eeChar);
		}
	}

}

int mem(){
// Upload therapy to EEPROM

	if (param[1]=="\0") {
		eepromUpload();

	} else if (param[1]=="@") {
		//Find script end
		int endAdr=0;
		for (int i=0; i<PROGRAM_SIZE; i++){
		  if ((byte)EEPROM.read(i)==255 || (char)EEPROM.read(i)=='@'){
			endAdr=i;

			break;
		  }
		}
		if (pcConnection) Serial.println(formatLine(endAdr,"appending from..."));
		eepromUpload(endAdr);

	} else if (param[1].toInt()>0 && param[1].toInt()<PROGRAM_SIZE) {
		eepromUpload(param[1].toInt());
	} else {
		if (pcConnection) Serial.print("Error: unknown parameter ");
		if (pcConnection) Serial.println(param[1]);
		return -1;
	}

	return 0;
}

int jump(int labelNumber, int &adr){

	if (labelNumber>0 && labelNumber<MAX_LABELS){
#ifdef SERIAL_DEBUG
			Serial.print("jump3 lblPtr: ");
			Serial.println(labelPointer[labelNumber]);
#endif

		if (labelPointer[labelNumber]) {


			if (labelLoops[labelNumber] > 0) {

				adr = labelPointer[labelNumber];	//Jump to new position
				labelLoops[labelNumber]--;			//Decrees jump counter
				return adr;

			} else if(labelLoops[labelNumber]==-1) { //Unlimited loop

				adr = labelPointer[labelNumber];
				return adr;

			}
		}
	}
	return 0;
}

int readFlashLine(int fromAddress, String &lineString){
	  //Read one line from EEPROM memory
	  int i = 0;
	  lineString="";

#ifdef SERIAL_DEBUG
	  	//Serial.print("readFlashLine1 fromAddress: ");
		//Serial.println(fromAddress);
#endif

	  do {

	    char eeChar = char( pgm_read_byte(&internalProgram[fromAddress+i])  )  ;

#ifdef SERIAL_DEBUG
	  	//Serial.print("readFlashLine2 eeChar: ");
		//Serial.println(eeChar);
#endif
	    if ( eeChar==char('@') ) {
	      if (i>0) {
	        eeChar='\n';
	      } else {
	        i=0;
	        break;
	      }
	    }
	    lineString+=eeChar;
	    i++;
	    if (eeChar=='\n') break;
	  } while (1);
#ifdef SERIAL_DEBUG
	  	//Serial.print("readFlashLine3 i: ");
		//Serial.println(i);
#endif
	  return i;
}

int readLabelPointers(byte prog){
	/* Initialize Labels pointers and jump loops
	 * prog:
	 * 0 - user program, jumps have counters,
	 * 1-9 Internal programs,
	 */
	int i;
	int adr=0;

	for(i=1; i<MAX_LABELS+1; i++ )
		labelLoops[i] = 0;

	i=0;

	do {
		if (prog) {
			//Internal program addresses
			adr = readFlashLine(i,line);
			getParams(line);
		} else {
			//EEPROM program labels
			adr = readEepromLine(i,line);
			getParams(line);
		}

		if (line.length()>1)
		if (line[0]==':'){
			byte lblNo = line[1]-48;
			if(lblNo>0 && lblNo<10){
				labelPointer[lblNo] = i+line.length();  // Next line of label
				//labelPointer[lblNo] = adr;  // Next line of label
				if (param[1].length()){

					labelLoops[lblNo] = param[1].toInt();

				} else {
					labelLoops[lblNo] = -1;
				}

				if (lblNo==prog && prog>0) return labelPointer[lblNo];

			}
		}

		i+=line.length();
		//i=adr;

	} while(adr);

#ifdef SERIAL_DEBUG
	for (i=1; i<MAX_LABELS+1;i++){
		Serial.print("Label: ");
		Serial.print(i);
		Serial.print(" loops: ");
		Serial.print(labelLoops[i]);
		Serial.print(" ptr: ");
		Serial.println(labelPointer[i]);
	}
#endif

/*	do {
		adr = readFlashLine(i,line);

		if (line.length()>1)
		if (line[0]==':'){
			byte lblNo = line[1]-48;
			if(lblNo>0 && lblNo<10){
				labelPointer[lblNo] = i+line.length();  // Next line of label
				labelLoops[lblNo] = -1;

				if (lblNo==prog) return labelPointer[lblNo];

			}
		}

		i+=line.length();
	} while(adr);*/

	return 0;
}

void exe(int &adr, byte prog){
//Execute program
	int endLine;

	//First time of internal program call
	if (!adr && prog){
		adr = readLabelPointers(prog);

	} else if (!adr) {
		readLabelPointers(0);
	}

#ifdef SERIAL_DEBUG
		Serial.print("exe1 prog: ");
		Serial.println(prog);
	  	Serial.print("exe1 adr: ");
		Serial.println(adr);
#endif
		if (prog==0){
			//EEPROM memory

			endLine = readEepromLine(adr,line);

		} else {
			//Flash memory

			endLine = readFlashLine(adr,line);


#ifdef SERIAL_DEBUG
		Serial.print("exe2 endLine: ");
		Serial.println(endLine);
	  	Serial.print("exe2 adr: ");
		Serial.println(adr);
#endif
		}



	//endLine = readEepromLine(adr,line);
	if (endLine){
	//int endLine = readEepromLine(adr,line);

#ifdef SERIAL_DEBUG
		Serial.print("exe3 ");
		Serial.print(adr);
		Serial.print(": ");
		Serial.println(line);
#endif
		//executeNext line
		int adrJump = executeCmd(line);
		if(adrJump){
			adr=adrJump;
		} else {
			adr = adr + endLine;
		}
		endLine =0;

	} else {
		adr=0;


	}


}

void scan(unsigned long Freq, unsigned long period){
	// Scan from lastFreq to Freq used SCAN_STEPS by period
;
}


void freq(unsigned long Freq, unsigned int period){
/*Start generate frequency
 *  Freq - frequency e.g. 1000 = 10.00Hz
 *  period - time in seconds
 */

	//For scan() function propose
	lastFreq =constrain( Freq, MIN_FREQ_OUT, MAX_FREQ_OUT);

	//start
	wipersON();
	ad9850.set_frequency(Freq);

	freqStartMillis = millis();
	freqStopMillis = freqStartMillis + long(period) * 1000;

}






void off() {
  // Power off function

	// Turn coil off by making digital out to low
	ad9850.powerDown();

	// Turn electrodes off by setting both pots to 0
	wipersOFF();

	digitalWrite(powerPin, LOW);


}

int bat() {
  // Get battery voltage function

  return (  analogRead(batPin) *
            BATTERY_VOLTAGE_RATIO *
            (byte)EEPROM.read(EEPROM_BATTERY_CALIBRATION_ADDRESS) /
            100
          );
}


void beep( unsigned int period ) {
  // beep [period_ms]

  digitalWrite(buzerPin, HIGH);
  delay(period);
  digitalWrite(buzerPin, LOW);
}

/***************** bioZAP functions end *************************/

int readEepromLine(int fromAddress, String &lineString){
  //Read one line from EEPROM memory
  int i = 0;
  lineString="";
  do {
    char eeChar=(char)EEPROM.read(fromAddress+i);

    if ((eeChar==char(255)) ||(eeChar==char('@'))) {

        return 0;
/*      if (i>0) {
        eeChar='\n';
      } else {
        i=0;
        return 0;
        break;
      }*/
    }

    lineString+=eeChar;
    i++;
    if (eeChar=='\n') break;

  } while (1);

  return i;
}

void getParams(String &inputString){
  for (int i=0; i<MAX_CMD_PARAMS; i++) param[i]="";

  int from =0;
  int to =0;
  for (int i=0; i<MAX_CMD_PARAMS; i++){
    to = inputString.indexOf(' ',from); //Wykryj spacje

    if (to==-1) {
      to = inputString.indexOf('\n',from); //Wykryj NL #10
      if (to>0) param[i] = inputString.substring(from,to);
      param[i].trim();
      break;
    }

    if (to>0) param[i] = inputString.substring(from,to);
    param[i].trim();
    from = to+1;
  }
}



void checkBattLevel() {
  //Check battery level

  if ( analogRead(batPin) < minBatteryLevel) {
    //Emergency turn off



    for (int x=0; x<10; x++){
      digitalWrite(buzerPin, HIGH);   // Turn buzzer on
      delay(100);
      digitalWrite(buzerPin, LOW);    // Turn buzzer off
      delay(200);
    }

    beep(500);
    off();
  }

}

void rechargeBattery() {
//Charger is plugged

	// turn power ON because of # key stop issue
	//digitalWrite(powerPin, HIGH);

	lcd.init();
	lcd.print("Battery charging");
	//digitalWrite(powerPin, LOW); // turn power relay off


	unsigned long startInterval = millis();
	int startBatLevel = analogRead(batPin);

	//Turn backlight off
	delay(5000);
	lcd.noBacklight();
	digitalWrite(powerPin, LOW); // turn power relay off

	do {
		if ( millis() - startInterval > checkDeltaBatteryIncreasingVoltageTime) {
		  if (analogRead(batPin)-startBatLevel <= 0) { //no increasing voltage
			  //Battery charged

			  digitalWrite(powerPin, HIGH);

			  lcd.init();
			  lcd.print("Battery charged");

			  beep(200);

			  delay(5000);
			  lcd.noBacklight();

			  digitalWrite(powerPin, LOW);

			  // ... and charging further.
			  while (1);
		  }

		  //Start new charging period with new values
		  startInterval = millis();
		  startBatLevel = analogRead(batPin);
		}
	}  while (1); //forever loop
}

int initBatteryLevel(){

	  //Auto-correction voltage - for new device - see bat & cbat commands
	  if ( (byte)EEPROM.read(EEPROM_BATTERY_CALIBRATION_ADDRESS) > 130 ||
	       (byte)EEPROM.read(EEPROM_BATTERY_CALIBRATION_ADDRESS) < 70 ) {
	    EEPROM.put(EEPROM_BATTERY_CALIBRATION_ADDRESS,100); // 100 =  x 1.00
	  }

	  //Define minimum battery level uses in working for performance purpose.
	  return 	/*0-1023*/ 100 *
	                       MIN_BATTERY_LEVEL /
	                       BATTERY_VOLTAGE_RATIO /
	                       (byte)EEPROM.read(EEPROM_BATTERY_CALIBRATION_ADDRESS);
}

void serialEvent() {
 //if (!eepromLoad) {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if (inChar!='\r'){
      inputString += inChar;
    }

    Serial.print(inChar); //echo

    // if the incoming character is a newline, set a flag
    if (inChar == '\n') {
      stringComplete = true;
    }

    if (inChar == '@') {
      memComplete = true;
    }
  }
 //}
}

void eepromUpload(int adr) {
  unsigned int i = 0;
  boolean flagCompleted;
  boolean bufferFull = false;
  int endBuffer;
  //eepromLoad = true;

  do {
    //Serial.print(char(XON));
    bufferFull = readSerial2Buffer(endBuffer);
    b =0; // buffer pointer
    flagCompleted = false;
    while (!flagCompleted){

      flagCompleted = !(i+adr<PROGRAM_SIZE) || (memBuffer[b]=='@') || !(b < endBuffer);
      if (memBuffer[b]==';') memBuffer[b]='\n';   //Semicollon as end line LF (#10) for windows script
      if (memBuffer[b]=='\r') memBuffer[b] = ' '; //#13 -> space, No continue because of changing script length
      EEPROM.write(i+adr, memBuffer[b]);
      //Serial.print(memBuffer[b]);
      i++; b++;
    }
    //End of shorter program then PROGRAM_SIZE size

  } while (bufferFull);
  if (i+adr<PROGRAM_SIZE) EEPROM.write(i+adr, '@');
  //eepromLoad=false;
}

boolean readSerial2Buffer(int &endBuffer) {
    int i ; //= 0; //buffer indicator
    char c;

    //while(true) {
    for( i=0; i<PROGRAM_BUFFER; i++){

          while(!Serial.available());

          c = Serial.read();

          memBuffer[i] = c;
          //Echo
          //Serial.print(c);
          endBuffer = i;

          if (c == '@' ) {
        	  beep(30);
        	  return false;
            //break;
          }

          //i++;
    }

    return true;
}



#endif /* BIOZAP_FUNC_H_ */

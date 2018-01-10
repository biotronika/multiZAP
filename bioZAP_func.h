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
//#define FREE_PEMF

#include <Arduino.h>

#ifdef MULTIZAP
#include "multiZAP_def.h"
#include "DS1803.h"
#include "AD9850.h"
//#include "multiZAP_lcd.h"
#endif



//BIOzap
#define WELCOME_SCR "Free BIOzap interpreter welcome! See https://biotronika.pl"
#define PROGRAM_SIZE 1000     	// Maximum program size
#define PROGRAM_BUFFER 1    	// SRAM buffer size, used for script loading  TODO lack of memory
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
#define XON 17  //0x11			//TODO: to remove
#define XOFF 19 //0x13          //TODO: to remove
#define MAX_LABELS 8          	// Number of labels in script therapy

#ifndef INPUTS_DEF_
#define INPUTS_DEF_
#define INPUT_SIGN '>'			//Serial and LCD input sign
#define INPUT_SIGN_KEYPAD '*'	//Serial sign if command were inserted from keypad
#define INPUT_BACK_KEYPAD '<'	//Inputed back data from keypad
#endif

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
boolean Xoff = false;					//TODO: XON/XOFF to remove

#ifdef FREE_PEMF
byte pwm = 50;                          // TODO: Pulse width modulation is rectangle signal fulfillment : 0-100%, default 50
boolean outputDir = false;
byte coilState = LOW;
#endif

#ifdef MULTIZAP
byte wiper0 = 0;
byte wiper1 = 0;

unsigned long freqStartMillis = 0;
unsigned long freqStopMillis = 0;
unsigned long programStartMillis = 0;
unsigned long programStopMillis = 0;

#endif

//TODO: hehaka
//Labels & jumps
/*
String labelName[MAX_LABELS];           // TODO: Labels names e.g. :MY_LABEL
unsigned int labelPointer[MAX_LABELS];  // TODO: Next line of label
unsigned int labelLoops[MAX_LABELS];    // TODO: Number of left jump loops
byte labelsPointer = 0;                 // TODO: Pointer of end label table
*/


//Serial buffer
char memBuffer[PROGRAM_BUFFER];

const unsigned long pauseTimeOut = 600000UL;    // 600000 Time of waiting in pause state as turn power off. (60000 = 1 min.)
const unsigned int btnTimeOut = 5000UL;         // Choose therapy program time out. Counted form released button.


unsigned long pauseTime =0;

volatile boolean pause = false; // true = pause on
unsigned long pressTime = 0;    // Time of pressing the button
unsigned long startInterval;    // For unused timeout off.
int programNo = -1;              // TODO: to reconstruct in free-PEMF deprecated: 0 = PC connection, 1= first program etc.
								// New: 0 = default program in memory, 1-9 or 1-3 = standard programs , -1 = PC
byte hr = 0;                    // User pulse from hrmPin

//function prototypes
int readEepromLine(int fromAddress, String &lineString);
void getParams(String &inputString);
void executeCmd(String cmdLine, boolean directMode = false);
void eepromUpload(int adr = 0);
boolean readSerial2Buffer(int &endBuffer);
void rechargeBattery();
int initBatteryLevel();
void checkBattLevel();


void off();
void beep( unsigned int period);
void freq(unsigned long Freq, unsigned int period);
void rec(unsigned int Freq, unsigned long period);  //deprecated
 int rec();
 int sin();
 int bat();
void wait( unsigned long period);
void exe();
void scan(unsigned long Freq, unsigned long period);
 int mem();
void ls();
void rm();
void cbat();


#ifdef MULTIZAP
void pbar(uint8_t percent, uint32_t period); 		//TODO
void print(String *str); 							//TODO
void wipersON();
void wipersOFF();
#endif

#ifdef MULTIZAP
void wipersON(){
  ds1803.set_wiper0(wiper0);
  ds1803.set_wiper1(wiper1);
}

void wipersOFF(){
  ds1803.set_wiper0(0);
  ds1803.set_wiper1(0);
  ad9850.powerDown();
}
#endif

#ifdef FREE_PEMF

#endif


String formatLine(int adr, String line){
  String printLine;
  printLine.reserve(22);
  printLine = "000"+String(adr,DEC);
  printLine = printLine.substring(printLine.length()-3,  printLine.length());
  printLine+=": "+line; //end marker for appending program
  return printLine;
}

void executeCmd(String cmdLine, boolean directMode){
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
// Label (ignore)
//TODO: for hehaka
    	;


    } else if (param[0]==""){
// Emptyline
    	;


    } else if (param[0]=="rm"){
// Remove, clear script therapy from memory
    	rm();


    } else if (param[0]=="print"){
// Print command
		#ifdef MULTIZAP
    	//TODO: message(param[1]);
		#endif

      if (cmdLine.length()>6) {   //TODO: I don't remember why 6 - to check (elektros)
    	  Serial.println(cmdLine.substring(6,cmdLine.length()-1));
      } else {
    	  Serial.println();
      }


    } else if (param[0]=="bat"){
// Print battery voltage
        Serial.println(bat());


    } else if (param[0]=="cbat"){
// Calibrate battery voltage
    	cbat();


    } else if (param[0]=="hr"){
// Print heart rate
        Serial.println(hr);


    } else if (param[0]=="beep"){
// Beep [time_ms]
        beep(param[1].toInt());
        Serial.println("OK");


    } else if (param[0]=="off"){
// Turn off
    	off();


    } else if (param[0]=="chp"){
// Change output signal polarity
    	Serial.println("Error: multiZAP doesn't support");


    } else if (param[0]=="wait"){
// Wait millis
    	wait(param[1].toInt());
      	Serial.println("OK");


    } else if (param[0]=="freq" || param[0]=="rec" || param[0]=="sin"){
// Generate rectangle signal - rec [freq] [time_sec]
    	//TODO: Different result functions - to divide
    	freq(param[1].toInt(), param[2].toInt());
      	Serial.println("OK");


    } else if (param[0]=="scan"){
// Scan from lastFreq  - scan [freq to] [time_ms]
    	scan(param[1].toInt(), param[2].toInt());
    	Serial.println("OK");


    } else if (param[0]=="exe"){
// Execute eeprom program only in direc mode
    	if ( directMode) {
    		exe();
    	} else {
    		Serial.println("Error: can't execute program from eeprom program!");
    	}


    }  else {
//Unknown command

    	Serial.println("Unknown command: "+param[0]);

    }


}

///////////////////////////// bioZAP functions ///////////////////////////////

#ifdef MULTIZAP
void pbar(uint8_t percent, uint32_t period){
// Scaling progress bar on lcd, and show remaining time
//TODO: elektros
	;
}

void print(String *str){
// Shows script therapy message on lcd display
// TODO: elektros
	;
}
#endif

void cbat(){
// Calibrate battery voltage

	//Correction factor
	byte i = 100 * param[1].toInt()/(int(analogRead(batPin)*BATTERY_VOLTAGE_RATIO));

	EEPROM.put(EEPROM_BATTERY_CALIBRATION_ADDRESS, i);
	Serial.println("OK");
}

void rm(){
// Remove, clear script therapy from memory

	for(int i=0; i<PROGRAM_SIZE; i++){
		EEPROM.put(i, 255);
		if (!(i % 128)) Serial.print(".");
	}
	Serial.println("OK");
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
		Serial.println(formatLine(endAdr,"appending from..."));
		eepromUpload(endAdr);

	} else if (param[1].toInt()>0 && param[1].toInt()<PROGRAM_SIZE) {
		eepromUpload(param[1].toInt());
	} else {
		Serial.print("Error: unknown parameter ");
		Serial.println(param[1]);
		return -1;
	}

	return 0;
}

void exe(){
//Execute program

	int adr=0;
	String line;

	while (int endLine = readEepromLine(adr,line)){


		Serial.print("executing: ");
		Serial.print(line);


		if (line.startsWith("repeat")) {
			adr=0;
			line="";
		}

		executeCmd(line);
		adr = adr + endLine;
	}


  	Serial.println("Script done.");
  	Serial.println("OK");

}

void scan(unsigned long Freq, unsigned long period){
	// Scan from lastFreq to Freq used SCAN_STEPS by period

	long scanSteps=SCAN_STEPS;
	long stepPeriod = period /scanSteps;

	if (stepPeriod < 1) {
		scanSteps = period;
		stepPeriod=1;
	}

 	long startFreq = lastFreq;
 	long stepFreq = long( constrain(Freq, MIN_FREQ_OUT, MAX_FREQ_OUT) - lastFreq ) / scanSteps;

 /*
 	Serial.println(Freq);
 	Serial.println(lastFreq);
 	Serial.println(long(Freq-lastFreq));
 	Serial.println(startFreq);
 	Serial.println(stepPeriod);
 	Serial.println(scanSteps);
 	Serial.println(stepFreq);
*/

 	for (int i=0; i<scanSteps; i++) {
 		freq(startFreq+(i*stepFreq), stepPeriod);
 	}
}

#ifdef MULTIZAP
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
#endif

#ifdef FREE_PEMF
void freq(unsigned long Freq, unsigned int period) {
//Rectangle signal generate, Freq=783 for 7.83Hz, period in seconds

	//For scan() function propose
	lastFreq =constrain( Freq, MIN_FREQ_OUT, MAX_FREQ_OUT) ;

	unsigned long interval = 50000/constrain(Freq, MIN_FREQ_OUT, MAX_FREQ_OUT);
	unsigned long timeUp = millis() + (period*1000);
	unsigned long serialStartPeriod = millis();
	unsigned long startInterval = millis();
	unsigned long pausePressed;

  boolean pause = false;


//.......

}
#endif


int rec(){

	//-1 not supported
	return -1;
}

int sin(){

	//0 - That's it (only sin)
	return 0;
}

void off() {
  // Power off function

	// Turn coil off by making digital out to low
	ad9850.powerDown();

	// Turn electrodes off by setting both pots to 0
	wipersOFF();
	//ds1803.set_wiper0(0);
	//ds1803.set_wiper1(0);

	// Turn power off if not USB power
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

void wait( unsigned long period) {
  // wait [period_ms]

  unsigned long serialStartPeriod = millis();
  unsigned long startInterval = millis();

  while(millis()-startInterval <= period){
    //time loop

	//TODO serial break command - mark @

    //count each second
    if (millis()-serialStartPeriod >= 1000) {
      Serial.print('.');
      serialStartPeriod = millis();
    }
  }

}

void beep( unsigned int period ) {
  // beep [period_ms]

  unsigned long serialStartPeriod = millis();
  unsigned long startInterval = millis();

  digitalWrite(buzerPin, HIGH);
  while(millis()-startInterval <= period){
    //time loop

	//TODO serial break command - mark @

    //count each second
    if (millis()-serialStartPeriod >= 1000) { //one second
      Serial.print('.');
      serialStartPeriod = millis();
    }
  }

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


	  Serial.println();
	  Serial.print("Error: battery too low: ");
	  Serial.println(bat());



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
  boolean Xoff = false;
  int endBuffer;
  //eepromLoad = true;

  do {
    //Serial.print(char(XON));
    Xoff = readSerial2Buffer(endBuffer);
    int b =0; // buffer pointer
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

  } while (Xoff);
  if (i+adr<PROGRAM_SIZE) EEPROM.write(i+adr, 255);
  //eepromLoad=false;
}

boolean readSerial2Buffer(int &endBuffer) {
    int i = 0; //buffer indicator
    char c;

    boolean Xoff = false;
    int highBufferLevel = 0.7 * PROGRAM_BUFFER;

    Serial.write(XON);
    //Serial.print("\nXON\n");

    while(true) {
      if (Xoff) {
        //after send Xoff

          if (Serial.available()){
            c = Serial.read();
            memBuffer[i] = c;
            //Serial.print(c);
            endBuffer = i;
            i++;

          } else {
            break;
          };
           //if (i>= PROGRAM_BUFFER) break;


      } else {
        //before send Xoff

          Xoff = (i > highBufferLevel);

          while(!Serial.available());

          c = Serial.read();

          memBuffer[i] = c;
          //Serial.print(c);
          endBuffer = i;

          if (c == '@' ) {
            break;
          }

          i++;
          if (Xoff) {
            for (int j=0; j<64; j++)
            Serial.write(XOFF);
            //Serial._rx_complete_irq();
            //Serial._tx_udr_empty_irq();
            //Serial.print("\nXOFF\n");
          }
      }



    }
  return Xoff;

}



#endif /* BIOZAP_FUNC_H_ */

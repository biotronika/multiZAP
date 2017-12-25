// multiZAP See: biotronika.pl  biotronics.eu
// #3 generation of prototype
// Chris Czoba (c) krzysiek@biotronika.pl

#define HRDW_VER "multiZAP NANO3"
#define SOFT_VER "2017-11-12"

#include <Wire.h> 
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>


// Class decalration
//***************************************************************************

class bio_AD9850{
 public:
      bio_AD9850(int CLK, int FQUP, int Data, int quartzClock=125 ); 
      void set_frequency(double Freq);
      void operator<<(double Freq);
      void powerDown();

 private:
      int _CLK;
      int _FQUP;
      int _Data;
      int _quartzClock;
     
      void write(byte word);
      void clock_CLK();
      void clock_FQUP();
};

bio_AD9850::bio_AD9850(int CLK, int FQUP, int Data,int quartzClock)
{   
    _CLK = CLK;
    _FQUP = FQUP;
    _Data = Data;
    _quartzClock =quartzClock;
    

    pinMode(_FQUP, OUTPUT);
    pinMode(_CLK , OUTPUT);
    pinMode(_Data, OUTPUT);

    digitalWrite(_FQUP, LOW);
    digitalWrite(_CLK, LOW);
    digitalWrite(_Data, LOW);
}
  
void bio_AD9850::set_frequency(double Freq)
{
  byte w,w0;
  long int y;
  double x;

  w0= 0x00;    //Phase=0, PowerDown=LOW, ControlBits=00

  x=4294967295/_quartzClock;
  //x=4294967295/125;   //Suitable for 125M Crystal AD9850 
  //x=4294967295/30;    //Suitable for 30M Crystal AD9851
  
  Freq=Freq/100000000; //add two zeros as 1/100 Hz - bioZAP comaptible
  Freq=Freq*x;
  y=Freq;

  clock_FQUP();
    
   w=(y>>=0); write(w); //write w4
   w=(y>>8);  write(w); //write w3
   w=(y>>16); write(w); //write w2
   w=(y>>24); write(w); //write w1
   write(w0);           //write w0

  clock_FQUP();
}
  
void bio_AD9850::powerDown()
{
  byte w,w0;
  w = 0x0;
  w0 = B00000100;
    
  clock_FQUP();

  write(w); //write w4
  write(w); //write w3
  write(w); //write w2
  write(w); //write w1
  write(w0);//write w0

  clock_FQUP();
}

void bio_AD9850::write(byte word)
{ 
  byte i;
  for(i=0; i<8; i++)
  {
    digitalWrite(_Data, (word>>i)&0x01);
    clock_CLK();
  }
}

void bio_AD9850::clock_CLK()
{
    digitalWrite(_CLK, HIGH);
    digitalWrite(_CLK, LOW);
}

void bio_AD9850::clock_FQUP()
{
    digitalWrite(_FQUP, HIGH);
    digitalWrite(_FQUP, LOW);
}

void bio_AD9850::operator<<(double frequency)
{
  set_frequency(frequency);
} 

//***************************************************************************

class bio_DS1803{
 public:
      bio_DS1803(int I2C_Address); //, int I2C_Wiper0, int I2C_Wiper1)
      void set_wiper0(int value);
      void set_wiper1(int value);

 private:
      int _I2C_Address;
      int _I2C_Wiper0;
      int _I2C_Wiper1;
      int _I2C_Wiper0_Value;
      int _I2C_Wiper1_Value;
  
};

bio_DS1803::bio_DS1803(int I2C_Address) //, int I2C_Wiper0, int I2C_Wiper1)
{
      
    _I2C_Address = I2C_Address;
    _I2C_Wiper0 = 0xA9; //I2C_Wiper0;
    _I2C_Wiper1 = 0xAA; //I2C_Wiper1;
    _I2C_Wiper0_Value = -1; //not init
    _I2C_Wiper1_Value = -1; //not init

}

void bio_DS1803::set_wiper0(int value)
{
    if (value != _I2C_Wiper0_Value){
      _I2C_Wiper0_Value = value;
      Wire.beginTransmission( _I2C_Address ); 
      Wire.write( _I2C_Wiper0 );
      Wire.write( value ); 
      Wire.endTransmission(); 
      delay(10);
    }
}


void bio_DS1803::set_wiper1(int value)
{
    if (value != _I2C_Wiper1_Value){
      _I2C_Wiper1_Value = value;
      Wire.beginTransmission( _I2C_Address ); 
      Wire.write( _I2C_Wiper1 );
      Wire.write( value ); 
      Wire.endTransmission();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
      delay(10);
    }
  
}
//***************************************************************************

#define AD9850__CLK A1
#define AD9850__FQUP A2
#define AD9850__DATA A3
#define powerPIN 4
#define buzerPin 13

#define DS1803_I2C_ADDRESS 0x28
#define EEPROM_WIPER0_ADDRESS 1022
#define EEPROM_WIPER1_ADDRESS 1023

bio_AD9850 AD9850(AD9850__CLK, AD9850__FQUP, AD9850__DATA);
//bio_AD9850 AD9850(AD9850__CLK, AD9850__FQUP, AD9850__DATA, 30); //for AD9851
bio_DS1803 DS1803(DS1803_I2C_ADDRESS);

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const byte ROWS = 4; //rows
const byte COLS = 4; //columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {12, 11, 10, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6, 5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char key;


int col =0;
int pos = 0; 
int val;
long Freq = 1000000; //100kHz

byte wiper0;
byte wiper1;


//              1kHz    100kHz    400kHz    100kHz    10Hz
long freqs[] = {44000, 783, 1179, 1000, 2000};
byte currentFreq = 0;

//
long inputVal (String dialogText,long defaultVal = -1);

//Progress bar lcd characters
byte bar0[8] = {
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0,
  0x0};

byte bar1[8] = {
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10};

byte bar2[8] = {
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18};

byte bar3[8] = {
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C};

byte bar4[8] = {
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E};

byte bar5[8] = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F};

byte char6[8] = {
  B000000,
  B001100,
  B010010,
  B111111,
  B000000,
  B000000,
  B000000,
  B000000};

byte char7[8] = {
  B000000,
  B000000,
  B000000,
  B111111,
  B010010,
  B001100,
  B000000,
  B000000};

                          
void setup (){
  Wire.begin();

    //lcd.begin(); //  
  lcd.init();
  
  lcd.createChar(0, bar0);
  lcd.createChar(1, bar1);
  lcd.createChar(2, bar2);
  lcd.createChar(3, bar3);
  lcd.createChar(4, bar4);
  lcd.createChar(5, bar5);
  lcd.createChar(6, char6);
  lcd.createChar(7, char7);
  




  pinMode(buzerPin, OUTPUT);
  pinMode(powerPIN,  OUTPUT); 
  //Turn on
  digitalWrite(powerPIN, HIGH);


  
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(HRDW_VER);
  lcd.setCursor(0,1);
  //lcd.print(HRDW_VER);
  //lcd.print(" ");
  lcd.print(SOFT_VER);



  // initialize serial communication
  Serial.begin(9600);
  
  //digitalWrite(buzzPin, HIGH);
  digitalWrite(buzerPin, HIGH);
  delay(50);
  //digitalWrite(buzzPin, LOW);
  digitalWrite(buzerPin, LOW);

  wiper0=(byte)EEPROM.read(EEPROM_WIPER0_ADDRESS);
  wiper1=(byte)EEPROM.read(EEPROM_WIPER1_ADDRESS);
  delay(20);
  DS1803.set_wiper0(wiper0);
  delay(20);
  DS1803.set_wiper1(wiper1); 

  AD9850.powerDown(); 
  //delay(200);
  //AD9850 << freqs[currentFreq];
  do{
    key = keypad.getKey();
   } while(key==NO_KEY);
  
}
 
void loop(){ 

  //Na wejsciu (wzmocnienie)
  //DS1803.set_wiper0( map( analogRead(potGrey) , 0, 1023, 0, 255) );
  

  //napiecie odniesienia
  //DS1803.set_wiper1( map( analogRead(potYellow) , 0, 1023, 0, 255) );

  //if (Serial.available() > 0) {
  //              // read the incoming byte:
  //  String newFreq = Serial.readString();
  //  Freq = newFreq.toInt();
  //  AD9850 << Freq;
  //}
  

  key = keypad.getKey();
  

  if (key != NO_KEY){
    //Serial.print(key);
    
    if (key=='A') {
      Freq = inputVal("Ust. czest. x100");
      wipersON();
      freq(Freq);
      key=NO_KEY; 
    }

    if (key=='#') {
      lcd.clear();
      lcd.print("multiZAP alpha");
      message("Zatrzymano");
      wipersOFF();
      key=NO_KEY;     
    }

    if (key=='B') {
      // Select program
      byte program = inputVal("Wybierz prog 1-3");
      
      lcd.clear();
      lcd.setCursor(0,1);
      //lcd.print("prog: ");
      //lcd.print(program);
      
      lcd.setCursor(0,0);
      if (program==1) {
        
          lcd.print("1-Chlamydia");
          message("25min  #-start");
          delay(3000);
            do{
                key = keypad.getKey();
                if (key=='*') digitalWrite(powerPIN, LOW);
            } while (key=='#');
            wipersON();
            program1();
            wipersOFF();
            digitalWrite(powerPIN, LOW);
        
      }

      if (program==2) {
        
          lcd.print("2-Borelioza");
          message("30min  #-start");
          delay(3000);
            do{
                key = keypad.getKey();
                if (key=='*') digitalWrite(powerPIN, LOW);
            } while (key=='#');
            wipersON();
            program2();
            wipersOFF();
            digitalWrite(powerPIN, LOW);
        
      }
 
      if (program==3) {
        
          lcd.print("3-Candida");
          message("21min  #-start");
          delay(3000);
            do{
                key = keypad.getKey();
                if (key=='*') digitalWrite(powerPIN, LOW);
            } while (key=='#');
            wipersON();
            program3();
            wipersOFF();
            digitalWrite(powerPIN, LOW);
        
      }

      if (program==9) {
        
          lcd.print("9-Test");
          message("2min  #-start");
          delay(3000);
            do{
                key = keypad.getKey();
                if (key=='*') digitalWrite(powerPIN, LOW);
            } while (key=='#');
            wipersON();
            program0();
            wipersOFF();
            digitalWrite(powerPIN, LOW);
        
      }
      

      key=NO_KEY; 
    }
    

    //TODO:Testowe przełaczanie częstotliwosci
    if (key=='C') {
      DS1803.set_wiper0(wiper0);
      DS1803.set_wiper1(wiper1);  
      digitalWrite(buzerPin, HIGH);
      delay(200);
      digitalWrite(buzerPin, LOW);
      currentFreq++;
      if (currentFreq > 4)  currentFreq = 0;
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print(freqs[currentFreq]);
      AD9850 << freqs[currentFreq];
      key=NO_KEY; 
    }

  if (key=='D') {
    wiper0 = inputVal("Set w0_gain",wiper0);
    EEPROM.write(EEPROM_WIPER0_ADDRESS,wiper0); // ?
    DS1803.set_wiper0(wiper0);
    
    wiper1 = inputVal("Set w1_vmin",wiper1);
    EEPROM.write(EEPROM_WIPER1_ADDRESS,wiper1); // ?
    DS1803.set_wiper1(wiper1);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("zapper++");
      //Clear LCD
    key=NO_KEY; 
  }
    
  if (key=='*') digitalWrite(powerPIN, LOW);
    
  }

}

void wipersON(){
  DS1803.set_wiper0(wiper0);
  DS1803.set_wiper1(wiper1); 
}

void wipersOFF(){
  DS1803.set_wiper0(0);
  DS1803.set_wiper1(0); 
  AD9850.powerDown();
}

 

void message (String messageText) {
  // Message in #2 line
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(messageText);
}

long inputVal (String dialogText,long defaultVal){
  //Input dialog with # as end
    String in="0";
    int col = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(dialogText);
    if (defaultVal>=0) {
      lcd.setCursor(0,1);
      lcd.print(defaultVal);
    }
    lcd.setCursor(0,1);
    

    do {
      char key = keypad.getKey();
      if (key != NO_KEY) {
        if (key=='#') break;
        if (col==0) {   
          lcd.print("                ");
          lcd.setCursor(0,1);
          //lcd.cursor();
        }
        //lcd.setCursor(col,1);
        lcd.print(key);
        col++;
        in=in+key;
      }
    } while (1);

    //lcd.noCursor();
    
    if (col==0 && defaultVal!=-1) {
      return defaultVal;
    } else {
      return in.toInt();
    }
   
}

int progressBar (byte progress) {
  // Show progress bar in #2 line
    lcd.setCursor(0,1);
    //lcd.print(" ");
    lcd.print(progress);
    lcd.print("%  ");
    lcd.setCursor(4,1);
    for (int i=0; i<(progress/10);i++) lcd.write(5);
    lcd.write(map( progress%10,0,9,0,5));  
    lcd.print("          ");
    lcd.setCursor(14,1);
    lcd.write(6);
    lcd.setCursor(15,1);
    lcd.write(7);
}

int wait(unsigned long waitPeriod, unsigned long &currentPeriod, unsigned long totalPeriod){
    //Wait waitPeriod and display progress bar
    int steps = waitPeriod/1000; 

    for( steps; steps--; steps<=0) {
      progressBar (100*currentPeriod/totalPeriod);
      
      delay(1000);
      
      if (keypad.getKey()=='*') digitalWrite(powerPIN, LOW);
      currentPeriod+=1000;
    }
    progressBar (100*totalPeriod/currentPeriod);
    delay(waitPeriod%1000);  //rest period to wait
    return 0;
}

void freq(unsigned long Freq){
  // Genenrate signal 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(Freq/100);
    lcd.print(".");
    if (Freq % 100 < 10) lcd.print("0"); 
    lcd.print(Freq % 100);
    lcd.print("Hz ");

    AD9850 << Freq;
    
}

void program1 (){
  //Test 1k, 10k, 100k, 400k, 900k, 10Hz every 20sec.
  unsigned long totalPeriod = 1500000;
  unsigned long currentPeriod = 0;
  freq(471050); wait(120000, currentPeriod, totalPeriod); 
  freq(47100); wait(60000, currentPeriod, totalPeriod); 
  freq(47150); wait(60000, currentPeriod, totalPeriod); 
  freq(47200); wait(60000, currentPeriod, totalPeriod); 
  freq(47900); wait(120000, currentPeriod, totalPeriod); 
  freq(62000); wait(120000, currentPeriod, totalPeriod); 
  freq(94010); wait(120000, currentPeriod, totalPeriod); 
  freq(94290); wait(120000, currentPeriod, totalPeriod); 
  freq(188010); wait(120000, currentPeriod, totalPeriod); 
  freq(188590); wait(120000, currentPeriod, totalPeriod); 
  freq(376030); wait(120000, currentPeriod, totalPeriod); 
  freq(377170); wait(120000, currentPeriod, totalPeriod); 
  freq(752050); wait(120000, currentPeriod, totalPeriod); 
  freq(754340); wait(120000, currentPeriod, totalPeriod); 
}

void program2 (){
  //Test 1k, 10k, 100k, 400k, 900k, 10Hz every 20sec.
  unsigned long totalPeriod = 1800000;
  unsigned long currentPeriod = 0;
  freq(20300); wait(180000, currentPeriod, totalPeriod); 
  freq(41200); wait(180000, currentPeriod, totalPeriod); 
  freq(41400); wait(180000, currentPeriod, totalPeriod); 
  freq(58900); wait(180000, currentPeriod, totalPeriod); 
  freq(64000); wait(180000, currentPeriod, totalPeriod); 
  freq(66700); wait(180000, currentPeriod, totalPeriod); 
  freq(84000); wait(180000, currentPeriod, totalPeriod); 
  freq(107200); wait(180000, currentPeriod, totalPeriod); 
  freq(108700); wait(180000, currentPeriod, totalPeriod);  
  freq(855400); wait(180000, currentPeriod, totalPeriod); 
}

void program3 (){
  //Test 1k, 10k, 100k, 400k, 900k, 10Hz every 20sec.
  unsigned long totalPeriod = 1260000;
  unsigned long currentPeriod = 0;
  freq(2048500); wait(180000, currentPeriod, totalPeriod); 
  freq(5115500); wait(180000, currentPeriod, totalPeriod); 
  freq(5115600); wait(180000, currentPeriod, totalPeriod); 
  freq(5394000); wait(180000, currentPeriod, totalPeriod); 
  freq(5891400); wait(180000, currentPeriod, totalPeriod); 
  freq(5891600); wait(180000, currentPeriod, totalPeriod); 
  freq(8874000); wait(180000, currentPeriod, totalPeriod); 

}
void program0 (){
  //Test 1k, 10k, 100k, 400k, 900k, 10Hz every 20sec.
  unsigned long totalPeriod = 120000;
  unsigned long currentPeriod = 0;
  freq(1000); wait(20000, currentPeriod, totalPeriod); 
  freq(10000); wait(20000, currentPeriod, totalPeriod); 
  freq(100000); wait(20000, currentPeriod, totalPeriod); 
  freq(10000000); wait(20000, currentPeriod, totalPeriod); 
  freq(40000000); wait(20000, currentPeriod, totalPeriod); 
  freq(90000000); wait(20000, currentPeriod, totalPeriod); 
}


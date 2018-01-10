/***************************************************
** 						                          **
**  Controller Library for AD9850 Module/Shield   **
** 						                          **
**  Downloaded from:				              **
**  www.arduino-projekte.de			              **
**						                          **
****************************************************
 * AD9850.cpp
 *
 *    Modified: 2017-12-26
 * Modified by: elektros230
 */
#include <Arduino.h>


class AD9850{
 public:
      AD9850(int CLK, int FQUP, int Data, int quartzClock=125 );
      void set_frequency(double Freq);
      double AD9850::get_frequency();
      //void operator<<(double Freq);
      void powerDown(void);
      //void reset(int _RST);

 private:
      int _CLK;
      int _FQUP;
      int _Data;
      int _quartzClock;
      double _Freq;

      void write(byte word);
      void clock_CLK();
      void clock_FQUP();
};

AD9850::AD9850(int CLK, int FQUP, int Data, int quartzClock)
{
    _CLK = CLK;
    _FQUP = FQUP;
    _Data = Data;
    _quartzClock = quartzClock;
    _Freq = 0;


    pinMode(_FQUP, OUTPUT);
    pinMode(_CLK , OUTPUT);
    pinMode(_Data, OUTPUT);

    digitalWrite(_FQUP, LOW);
    digitalWrite(_CLK, LOW);
    digitalWrite(_Data, LOW);
}

double AD9850::get_frequency(){

	return _Freq;
}

void AD9850::set_frequency(double Freq)
{
  byte w,w0;
  long int y;
  double x;

  _Freq=Freq;

  w0= 0x00;    //Phase=0, PowerDown=LOW, ControlBits=00

  x=4294967295/_quartzClock;
  //x=4294967295/125;   //Suitable for 125M Crystal AD9850
  //x=4294967295/30;    //Suitable for 30M Crystal AD9851

  Freq=Freq/100000000; //added two zeros as 1/100 Hz - BIOzap compatible
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

void AD9850::powerDown()
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

void AD9850::write(byte word)
{
  byte i;
  for(i=0; i<8; i++)
  {
    digitalWrite(_Data, (word>>i)&0x01);
    clock_CLK();
  }
}

void AD9850::clock_CLK()
{
    digitalWrite(_CLK, HIGH);
    digitalWrite(_CLK, LOW);
}

void AD9850::clock_FQUP()
{
    digitalWrite(_FQUP, HIGH);
    digitalWrite(_FQUP, LOW);
}


/*void AD9850::reset(int _RST)
{
	digitalWrite(_CLK, 0);
	digitalWrite(_FQUP, 0);

	//Reset signal
	digitalWrite(_RST, 0);
	digitalWrite(_RST, 1);
	delayMicroseconds(100);
	digitalWrite(_RST, 0);

	clock_CLK();

	clock_FQUP();

}*/


/*void AD9850::operator<<(double frequency)
{
  set_frequency(frequency);
}*/


/***************************************************
** 						                          **
**  Controller Library for AD9850 Module/Shield   **
** 						                          **
**  Downloaded from:				              **
**  www.arduino-projekte.de			              **
**						                          **
****************************************************
 * AD9850.h
 *
 *    Modified: 2017-12-26
 * Modified by: elektros230
 */

#ifndef AD9850_H_
#define AD9850_H_

#include <Arduino.h>

class AD9850{
 public:
      AD9850(int CLK, int FQUP, int Data, int quartzClock=125 );
      void set_frequency(double Freq);
      double get_frequency();
      void operator<<(double Freq);
      void powerDown(void);
      //void reset(int _RST);

 private:
      int _CLK;
      int _FQUP;
      int _Data;
      int _quartzClock;

      void write(byte word);
      void clock_CLK();
      void clock_FQUP();
};

#endif /* AD9850_H_ */

/*
 * DS1803.cpp
 *
 *  Created on: 26 dec 2017
 *      Author: elektros230
 *
 */


#include "Arduino.h"
#include <Wire.h>


class DS1803{
 public:
      DS1803(int I2C_Address);
      void set_wiper0(int value);
      void set_wiper1(int value);
      int get_wiper0(void);
      int get_wiper1(void);

 private:
      int _I2C_Address;
      int _I2C_Wiper0;
      int _I2C_Wiper1;
      int _I2C_Wiper0_Value;
      int _I2C_Wiper1_Value;

};

DS1803::DS1803(int I2C_Address)
{

    _I2C_Address = I2C_Address;
    _I2C_Wiper0 = 0xA9; //I2C_Wiper0;
    _I2C_Wiper1 = 0xAA; //I2C_Wiper1;
    _I2C_Wiper0_Value = -1; //not init
    _I2C_Wiper1_Value = -1; //not init

}

void DS1803::set_wiper0(int value)
{
    if (value != _I2C_Wiper0_Value){
      _I2C_Wiper0_Value = value;
      Wire.beginTransmission( _I2C_Address );
      Wire.write( _I2C_Wiper0 );
      Wire.write( value );
      Wire.endTransmission();

    }
}

int DS1803::get_wiper0(void)
{
	return _I2C_Wiper0_Value;
}

int DS1803::get_wiper1(void)
{
	return _I2C_Wiper1_Value;
}

void DS1803::set_wiper1(int value)
{
    if (value != _I2C_Wiper1_Value){
      _I2C_Wiper1_Value = value;
      Wire.beginTransmission( _I2C_Address );
      Wire.write( _I2C_Wiper1 );
      Wire.write( value );
      Wire.endTransmission();

    }

}

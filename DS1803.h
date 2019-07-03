/*
 * DS1803.h
 *
 *  Created on: 26 dec 2017
 *      Author: elektros230
 */

#ifndef DS1803_H_
#define DS1803_H_

class DS1803 {
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

#endif /* DS1803_H_ */

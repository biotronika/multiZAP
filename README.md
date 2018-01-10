**multiZAP**
To compile code and upload using Arduino IDE:
1. Download files and put all of them into multiZAP folder (it must has exactly that name). Copy also AD9850.h AD9850.cpp DS1803.h DS1803.cpp to multiZAP main folder.
2. Open multiZAP.ino file in Arduino IDE.
3. Install Keypad library in Arduino (Sketch->Include Library->Add ZIP. Library..., and point to right zip file).
4. Install LiquidCrystal I2C library.
5. Check if you have Wire & EEPROM libraries already installed (Sketch->Include Library-> see on list: Wire and EEPROM).
6. Configure board (Tools->Board->Arduino Nano)  (Tools->Processor->ATmega328).
7. Install Arduino Nano drivers (see biotronika.pl website).
8. Configure serial port. Plug USB cable to PC and multiZAP, and Tolls->Port->select right COM port.
9. Compile and upload. Sketch->Upload. Wait until on down side of Arduino IDE window see Done uploading.

See: [https://biotronika.pl]

**Software not working correctly yet!!!**


**Keypad manual functions:**
 
```
[#] - Turn on - press [#] and hold 3 seconds

[*] - Turn off  - and pres [#] (yes) or [*]

[A] - Generate frequency 
	1. Set frequency e.g. 10000 = 100.00Hz or use [D] as decimal separator e.g. [1][0][0][D] and press [#]
	2. Set time in seconds and press [#]

[D] - Calibrate and save settings in EEPROM memory
	1. Set vampl - pk-pk apmplitude of sine signal e.g. 400 = 4.00V and press [#] (1.00 - 8.00V)
	2. Set vmin - minimum extremum of sine signal e.g. 100 = 1.00V and press [#] (0.50 - 4.00)
	3. Set calibration frequency or simply press [#]
	
[0] - Start EEPROM therapy script

```

Enjoy :)

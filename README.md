# multiZAP
To compile code and upload in Arduino IDE to your device:
1. Dowload all files into multiZAP folder (it must has that name). Copy also AD9850.h AD9850.cpp DS1803.h DS1803.cpp to multiZAP main folder.
2. Open multiZAP.ino file in Arduino IDE.
3. Install Keypad library in Arduino (Sketch->Include Library->Add ZIP. Library..., and point to right zip file).
4. Install LiquidCrystal I2C library.
5. Check if you have Wire & EEPROM libraries already installed (Sketch->Include Library-> see on list: Wire and EEPROM).
6. Configure board (Tools->Board->Arduino Nano)  (Tools->Processor->ATmega328).
7. Install Arduino Nano drivers (see biotronika.pl website).
8. Configure serial port. Plug USB cable to PC and multiZAP, and Tolls->Port->select right COM port.
9. Compile and upload. Sketch->Upload. Wait until on down side of Arduino IDE window see Done uploading.

See: https://biotronika.pl


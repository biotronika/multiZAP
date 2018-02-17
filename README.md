## multiZAP

See: [biotronika.pl](https://biotronika.pl)

### To compile code and upload using Arduino IDE:
1. Download files and put all of them into multiZAP folder (it must has exactly that name). Copy also AD9850.h AD9850.cpp DS1803.h DS1803.cpp to multiZAP main folder.
2. Open multiZAP.ino file in Arduino IDE.
3. Optional select Polish language by changing **#define EN_H_** into **#define PL_H_** in **multiZAP_def.h**. English is default.
4. Install Keypad library in Arduino (Sketch->Include Library->Add ZIP. Library..., and point at proper zip file).
5. Install LiquidCrystal I2C library (where above :)
6. Check if you have Wire & EEPROM libraries already installed (Sketch->Include Library-> see on list: Wire and EEPROM).
7. Configure board (Tools->Board->Arduino Nano)  (Tools->Processor->ATmega328).
8. Install Arduino Nano driver - **biotronika.pl** website: [CH341SER.ZIP]( https://biotronika.pl/sites/default/files/2016-12/CH341SER.ZIP).
9. Configure serial port. Plug USB cable to PC and multiZAP, and Tolls->Port->select right COM port.
10. Compile and upload. Sketch->Upload. Wait until on down side of Arduino IDE window see **Done uploading**.

### Downloading existing script therapy to multiZAP (Windows only)
1. Open **https://biotronika.pl/terapie** web site and select the script.
   If you do not read Polish, use google translator because web site not support English language yet.
2. Click get therapy script [**+Pobierz skrypt terapii**].
3. Download **terapia.zip** (therapy) and open **terapia.cmd** existing in zip file.
4. If Windows 8, 8.1 or 10 opens defender window, then click **More info** and button **Run Anyway**
5. Plug USB cable to your multiZAP. Be sure that the device was turned off before.
6. Provide proper COM port into **cmd.exe** window and click Enter.
7. Now choosen script from the web site is uploading to your multiZAP device. 
8. Wait a moment and finally click ``[*]`` key on multiZAP kepad and plug USB cable off.
9. Turn on device and click ``[0]`` to start script therapy.

### Creating your own script therapy
1. Create account on the web site.
2. Click new therapy **Nowa terapia** link and provide data into all fields.

or
1. Use Arduino **Serial monitor view**

or 
1. Use any serial communication program.

 Baud: **9600**
 Data size: **8**
 Parity: **none**
 DTR: **ON**
 End line: **CR**

**bioZAP** script language: [https://biotronika.pl/biozap](https://biotronika.pl/biozap)

[Supported bioZAP commands](https://github.com/biotronika/multiZAP/wiki)

## Keypad manual functions:
 
```
[#] - Turn on - press [#] and hold 3 seconds. [#] is also enter key.

[*] - Escape key or turn-off in main menu.

[A] - Generate frequency 
	1. Set frequency e.g. 10000 = 100.00Hz or use [D] as decimal separator e.g. [1][0][0][D] and press [#]
	2. Set time in seconds and press [#]

[C] - Calibrate and save settings in EEPROM memory
	1. Set vampl - pk-pk apmplitude of sine signal e.g. 400 = 4.00V and press [#] (1.00 - 8.00V)
	2. Set vmin - minimum extremum of sine signal e.g. 100 = 1.00V and press [#] (0.50 - 4.00)
	3. Set calibration frequency or simply press [#]
	
[0] - Start EEPROM therapy script

[1]-[9] Start standard program in flash memory

[B] - TODO: Settings:
	 * auto_turn_off_time - EEPROM
	 * AD985x 0/1 - EEPROM
	 * default_program - EEPROM
	 
[D] - TODO: multiZAP+ function - Select and start express-scan. You need SD card pluged.

```

Enjoy using your multiZap device and wishing you best of health :)


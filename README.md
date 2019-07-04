## multiZAP

See: [biotronics.eu](https://biotronics.eu)



### To compile code and upload using Arduino IDE:
1. Download files and put all of them into multiZAP folder (it must has exactly that name). 
2. Open multiZAP.ino file in Arduino IDE.
3. Optional select Polish language by changing **#define EN_H_** into **#define PL_H_** in **multiZAP_def.h**. English is default.
4. Install Keypad library in Arduino (Sketch->Include Library->Add ZIP. Library..., and point at proper zip file). 
5. Install LiquidCrystal I2C library (where above :)
6. Check if you have Wire & EEPROM libraries already installed (Sketch->Include Library-> see on list: Wire and EEPROM).

**Arduino NANO R3**
1. Configure board: Tools->Board->Arduino Nano and **Tools->Processor->ATmega328P (Old Bootloader)**. For experimental **Arduino NANO EVERY** see below.
2. Install Arduino Nano driver - **biotronics.eu** website: [CH341SER.ZIP]( https://biotronics.eu/sites/default/files/2016-12/CH341SER.ZIP).
3. Configure serial port. Plug mini-USB cable to PC and multiZAP, and Tolls->Port->select right COM port.
4. Compile and upload. Sketch->Upload. Wait until on down side of Arduino IDE window see **Done uploading**.

**Arduino NANO EVERY**
1. Tools->Board->Boards Manager...-> select Arduino megaAVR Boards by Arduino -> Select the newest version (e.g. 1.8.1) and click Install
2. Configure board: Tools->Board->Arduino Nano Every and **Tools->Register emulation->ATMEGA328**.
3. In file **multiZAP_def.h** remove comment in line //#define FIX_BUG_NANO_EVERY
4. For **Arduino NANO EVERY** you must use exactly Keypad & LiquidCrystal I2C libraries attached in zip files.
5. Configure serial port. Plug micro-USB cable to PC and multiZAP, and Tolls->Port->select right COM port.
6. Compile and upload. Sketch->Upload. Wait until on down side of Arduino IDE window see **Done uploading**.


### Downloading existing script therapy to multiZAP (Windows only)
1. Open **https://biotronics.eu/terapie** web site and select the script.
2. Click get therapy script [**Download therapy script**].
3. Download exe file e.g. 21.exe and open it.
4. If Windows 8, 8.1 or 10 opens defender window, then click **More info** and button **Run Anyway**
5. Plug USB cable to your multiZAP. Be sure that the device was turned off before.
6. Click **Connect** and select proper COM port window and click OK.
7. Click **Download to device**. Now chosen script from the web site is uploading to your multiZAP device. 
8. Wait a moment and finally click ``[*]`` key on multiZAP keypad and plug USB cable off.
9. Turn on device and click ``[0]`` to start script therapy.

### Creating your own script therapy
1. Create account on the web site.
2. Click new therapy **New therapy** link and provide data into all fields.


or 
1. Use downloade.exe application. You can download it from biotronic.eu website.


**bioZAP** script language: [bioZAP manual](https://github.com/biotronika/documentation/blob/master/bioZAP/bioZAP.pdf)

[Supported bioZAP commands](https://github.com/biotronika/multiZAP/wiki)

## Keypad manual functions:
 
```
[#] - Turn on - press [#] and hold 3 seconds. [#] is also enter key.

[*] - Escape key or turn-off in main menu.

[A] - Generate frequency 
	1. Set frequency e.g. 10000 = 100.00Hz or use [D] as decimal separator e.g. [1][0][0][D] and press [#]
	2. Set time in seconds and press [#]

[C] - Calibrate and save settings in EEPROM memory
	1. Set vampl - pk-pk amplitude of sine signal e.g. 400 = 4.00V and press [#] (1.00 - 8.00V)
	2. Set vmin - minimum extreme of sine signal e.g. 100 = 1.00V and press [#] (0.50 - 4.00)
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


## Change log
* 2019-07-03 - Fixed measuring the battery low voltage, which turn the device off.
* 2019-07-04 - Add FIX_BUG_NANO_EVERY directive to fix bug in **Arduino Nano Every** in Atmega328 compatibility mode (changed A7 and A6 pins)

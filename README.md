# WiiClassicToUsb
Dirty arduino code to use a "Wii classic controller (pro)" on an USB port

## Hardware needed
 * A Wii Classic controller (pro or not, analog triggers aren't supported anyway)
 * A microcontroller board (arduino or not, as long as it is supported by the Arduino IDE and has a "HID.h" library) with the following:
   * An I²C (TWI) port at 400kHz (fast I²C)
   * A 3.3V power supply and 3.3V I/O (you can try with 5V, but officially you should use 3.3V)
   * An USB device/HID port (supported by the Arduino's "HID.h")
   * Tested and working with:
     * Arduino Due
     * You can use any ATMega32u4-based board too, but I didn't test as the only one I have is 5V only.
 * A Wii extension <-> 0.1" adapter. Or some wires. I use wires.

## Software needed
 * The Arduino IDE. Only tested with the 1.6.13.
 * Something to check if everything is working

## How to use
 * Put the folder "wii_classic_to_usb" in you sketchbook folder
 * Put the folder "Joypad" in you "libraries" folder (can be in the sketchbook folder)
 * Compile and upload the code to your board
 * Connect the controller with the following wiring: (pull-up resistors are not needed on the TWI bus)
   * (Board) -> (Controller) (see [here](http://wiibrew.org/wiki/Wiimote/Extension_Controllers) for the connector pinout)
   * 3.3V    -> Vcc
   * GND     -> GND
   * SDA     -> SDA
   * SCL     -> SCL
 
## ~~Credits~~ Where I stole code from:
 * https://skyduino.wordpress.com/2011/11/16/mini-tutoriel-arduino-nunchunk-de-wii/
 * http://wiibrew.org/wiki/Wiimote/Extension_Controllers
 * The arduino's "Mouse" library. Actually I just used the lib's structure.

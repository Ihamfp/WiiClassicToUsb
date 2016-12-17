/*
  Joypad.h

Based on the default "Mouse" implementation of Arduino 1.6.13.
*/

#ifndef JOYPAD_h
#define JOYPAD_h

#include "HID.h"

#define JOYPAD_BUTTONS_COUNT 32 // should not be changed
#define JOYPAD_REPORT_ID 4 // can be changed in case of conflict

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

//================================================================================
//================================================================================
//  Joypad

class Joypad_
{
private:
	//void sendReport();
	// buttons
  uint32_t buttons;
  // joysticks
  int8_t joyLX;
  int8_t joyLY;
  int8_t joyRX;
  int8_t joyRY;
public:
  Joypad_(void);
  void begin(void);
  void end(void);
  void update(void);
  void releaseAll();
  void press(uint8_t button);
  void joystick(int8_t x, int8_t y);
  void rjoystick(int8_t x, int8_t y);
};
extern Joypad_ Joypad;

#endif
#endif

/*
  Joypad.cpp
*/

#include "Joypad.h"

#if defined(_USING_HID)

static const uint8_t _hidReportDescriptor[] PROGMEM = {
	0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
	0x09, 0x04,                    // USAGE (Mouse)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x09, 0x05,                    //   USAGE (Pointer)
	0xa1, 0x00,                    //   COLLECTION (Physical)
  0x85, JOYPAD_REPORT_ID,        //     REPORT_ID (1)
	
	// buttons
	0x05, 0x09,                    //     USAGE_PAGE (Button)
	0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
	0x29, JOYPAD_BUTTONS_COUNT,    //     USAGE_MAXIMUM (Button BUTTONS_COUNT)
	0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
	0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
	0x95, JOYPAD_BUTTONS_COUNT,    //     REPORT_COUNT (32)
	0x75, 0x01,                    //     REPORT_SIZE (1)
	0x81, 0x02,                    //     INPUT (Data,Var,Abs)
	// axis
	0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
	0x09, 0x30,                    //     USAGE (X)
	0x09, 0x31,                    //     USAGE (Y)
	0x09, 0x33,                    //     USAGE (Rx)
	0x09, 0x34,                    //     USAGE (Ry)    
//	0x15, 0x80,                    //     LOGICAL_MINIMUM (-127)
	0x15, 0x00,                    //     LOGICAL MINIMUM (0)
//	0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
	0x25, 0xff,                    //     LOGICAL_MAXIMUM (255)
	0x75, 0x08,                    //     REPORT_SIZE (8)
	0x95, 0x04,                    //     REPORT_COUNT (4)
	0x81, 0x02,                    //     INPUT (Data,Var,Rel)
	0xc0,                          //   END_COLLECTION
	0xc0                           // END_COLLECTION
};

struct data {
	uint32_t buttons;
	int8_t x;
	int8_t y;
	int8_t rx;
	int8_t ry;
};

//================================================================================
//================================================================================
//	Joypad

Joypad_::Joypad_(void)
{
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    HID().AppendDescriptor(&node);
}

void Joypad_::begin(void) 
{
}

void Joypad_::end(void) 
{
}

void Joypad_::update(void) {
	data report;
	report.buttons = buttons;
	report.x = joyLX;
	report.y = joyLY;
	report.rx = joyRX;
	report.ry = joyRY;
	
	HID().SendReport(JOYPAD_REPORT_ID, &report, sizeof(data));
}

void Joypad_::releaseAll(void) {
	buttons = 0;
}

void Joypad_::press(uint8_t button) {
	if (button > JOYPAD_BUTTONS_COUNT)
		return;
	buttons = (buttons | (1 << button));
}

void Joypad_::joystick(int8_t x, int8_t y) {
	joyLX = x;
	joyLY = y;
}

void Joypad_::rjoystick(int8_t x, int8_t y) {
	joyRX = x;
	joyRY = y;
}

Joypad_ Joypad;

#endif

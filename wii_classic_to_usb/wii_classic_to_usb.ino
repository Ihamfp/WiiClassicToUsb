#include <Wire.h>

#include <Joypad.h>

#define DEV_ADDR 0x52
#define CRYPT_KEY 0x17

//#define DEBUG // define to enable debug (no rly?)
//#define CHANGES_ONLY // define to send data to USB only if something changes
//#define STICK_CALIBRATION // define to enable stick calibration. I don't even know if it works well.
#define INVERT_Y // define to invert the Y axis of the sticks

byte stickCal[4];

void setup() {
  for (byte i=0; i<4; i++) {
    stickCal[i] = 0;
  }

#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Opened serial port!");
  Serial.print("Device address: ");
  Serial.println(DEV_ADDR, HEX);
#endif

  Wire.begin();
#ifdef DEBUG
  Serial.println("Starting transmission ...");
#endif
  Wire.beginTransmission(DEV_ADDR);
#ifdef DEBUG
  Serial.print("Initialization ... ");
#endif
  Wire.write(0x40);
  Wire.write(0x00);
  Wire.endTransmission();
#ifdef DEBUG
  Serial.println("Done");

  Serial.print("Initializing USB gamepad ... ");
#endif
  Joypad.begin();
#ifdef DEBUG
  Serial.println("Done");
#endif
}

void handshake() {
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(0x00);
  Wire.endTransmission();
}

byte buffer[6];
#ifdef CHANGES_ONLY
byte lastBuffer[6];
#endif
byte cnt = 0;
#ifdef STICK_CALIBRATION
byte stickCalibrated = 0; // the first time, they aren't initialized
#else
byte stickCalibrated = 2; // nah it's done by the PC
#endif

void parse() {
  // to fix: LX, LY, RX, RY
  // todo: switches
  byte LX = ((buffer[0] & 0b00111111) * 4) - stickCal[0];
  byte LY = ((buffer[1] & 0b00111111) * 4) - stickCal[1];
  byte RX = (((buffer[2] & 0b10000000) >> 4)
    | ((buffer[1] & 0b11000000) >> 2)
    | ((buffer[0] & 0b11000000))) - stickCal[2];
  byte RY = ((buffer[2] & 0b00011111) << 3) - stickCal[3];

#ifdef INVERT_Y
  LY = (255-LY);
  RY = (255-RY);
#endif

#ifdef STICK_CALIBRATION
  if (stickCalibrated == 1) {
    stickCal[0] = LX;
    stickCal[1] = LY;
    stickCal[2] = RX;
    stickCal[3] = RY;
#ifdef DEBUG
    Serial.print("Calibrated sticks: ");
    for (int i=0; i<4; i++) {
      Serial.print(stickCal[0], DEC);
      Serial.print("\t");
    }
    Serial.println();
#endif // DEBUG
    stickCalibrated++;
  } else if (stickCalibrated == 0) {
    stickCalibrated++;
  }
#endif // STICK_CALIBRATION

#ifdef DEBUG
  Serial.print("LX: ");
  Serial.print(LX, DEC);
  Serial.print("\tLY: ");
  Serial.print(LY, DEC);
  Serial.print("\tRX: ");
  Serial.print(RX, DEC);
  Serial.print("\tRY: ");
  Serial.println(RY, DEC);

  Serial.print(buffer[4], BIN);
  Serial.println(buffer[5], BIN);
#endif

  // Now we're talking ... on the USB
  if (stickCalibrated == 2) {
    Joypad.joystick(LX, LY);
    Joypad.rjoystick(RX, RY);
  
    Joypad.releaseAll();
    for (int i=1; i<8; i++) {
      if ((~buffer[4]) & bit(i))
        Joypad.press(i-1);
    }
    for (int i=0; i<8; i++) {
      if ((~buffer[5]) & bit(i))
        Joypad.press(i+7);
    }
  
    Joypad.update();
  }
}

void loop() {
  Wire.requestFrom(DEV_ADDR, 6);
  while (Wire.available()) {
    buffer[cnt] = (Wire.read() ^ CRYPT_KEY) + CRYPT_KEY;
    cnt++;
  }
#ifdef CHANGES_ONLY
  if (cnt >= 5 && stickCalibrated > 1) { // we should force parsing if the sticks aren't calibrated
    bool update = false;
    for (int i=0; i<6; i++) {
      if (buffer[i] != lastBuffer[i]) {
        lastBuffer[i] = buffer[i];
        update = true;
      }
    }
  } else if (stickCalibrated < 2) {
    parse();
  }
#else
  if (cnt >= 5) parse();
#endif

  cnt = 0;
  handshake();
  delay(2);
}

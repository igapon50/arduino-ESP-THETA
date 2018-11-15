/*
 Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */
#include <PS4BT.h>

#define Enable_I2C

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#ifdef Enable_I2C
#include <Wire.h>
// A4(SDA)とA5(SCL)は、I2C通信に使用する
static const int i2cAddress = 8;
#endif// Enable_I2C

USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

void setup() {
// シリアル通信の準備
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

// USB Host sheildの準備
  if (Usb.Init() == -1) {
    Serial.print(F("OSC did not start\r\n"));
    while (1); // Halt
  }
  Serial.print(F("PS4 Bluetooth Library Started\r\n"));

#ifdef Enable_I2C
// I2Cの準備
#ifdef ARDUINO_ESP32_DEV
  int SDA32 = 21;
  int SCL32 = 22;
  Wire.begin(SDA32, SCL32);
#else
  Wire.begin(); // join i2c bus (address optional for master)
#endif// ARDUINO_ESP32_DEV
#endif// Enable_I2C
}

void loop() {
  Usb.Task();
  if (PS4.connected()) {
    static char older_L_X = 'A';
    static char older_L_Y = 'A';
    static char older_R_X = 'A';
    static char older_R_Y = 'A';
    uint8_t L_X = PS4.getAnalogHat(LeftHatX);
    uint8_t L_Y = PS4.getAnalogHat(LeftHatY);
    uint8_t R_X = PS4.getAnalogHat(RightHatX);
    uint8_t R_Y = PS4.getAnalogHat(RightHatY);
    if((L_X == 0 && L_Y == 0) || (R_X == 0 && R_Y == 0)){ // あり得ない値が来たら無視する
      return;
    }
    char newer_L_X = 'A' + map(128 < L_X ? --L_X : L_X,0,255,0,15);
    char newer_L_Y = 'A' + map(128 < L_Y ? --L_Y : L_Y,0,255,0,15);
    char newer_R_X = 'A' + map(128 < R_X ? --R_X : R_X,0,255,0,15);
    char newer_R_Y = 'A' + map(128 < R_Y ? --R_Y : R_Y,0,255,0,15);
    if(older_L_X != newer_L_X || older_L_Y != newer_L_Y || older_R_X != newer_R_X || older_R_Y != newer_R_Y){
      older_L_X = newer_L_X;
      older_L_Y = newer_L_Y;
      older_R_X = newer_R_X;
      older_R_Y = newer_R_Y;
#ifdef Enable_I2C
      Wire.beginTransmission(i2cAddress);
      Wire.write(newer_L_X);
      Wire.write(newer_L_Y);
      Wire.write(newer_R_X);
      Wire.write(newer_R_Y);
      Wire.endTransmission();
#endif// Enable_I2C
      Serial.print(newer_L_X);
      Serial.print(newer_L_Y);
      Serial.print(newer_R_X);
      Serial.print(newer_R_Y);
      Serial.print(F("\r\n"));
    }

    if (PS4.getButtonClick(PS)) {
      Serial.print(F("PS\r\n"));
      PS4.disconnect();
    }
  }
}

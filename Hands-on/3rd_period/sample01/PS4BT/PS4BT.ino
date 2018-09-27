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
  Wire.begin(); // join i2c bus (address optional for master)
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
    char newer_L_X = 'A' + map(L_X,0,255,0,14);
    char newer_L_Y = 'A' + map(L_Y,0,255,0,14);
    char newer_R_X = 'A' + map(R_X,0,255,0,14);
    char newer_R_Y = 'A' + map(R_Y,0,255,0,14);
    if(older_L_X != newer_L_X || older_L_Y != newer_L_Y || older_R_X != newer_R_X || older_R_Y != newer_R_Y){
      older_L_X = newer_L_X;
      older_L_Y = newer_L_Y;
      older_R_X = newer_R_X;
      older_R_Y = newer_R_Y;
#ifdef Enable_I2C
      Wire.beginTransmission(8); //transmit to device #8
      Wire.write(newer_L_X);
      Wire.write(newer_L_Y);
      Wire.write(newer_R_X);
      Wire.write(newer_R_Y);
      Wire.endTransmission(); //stop transmitting
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

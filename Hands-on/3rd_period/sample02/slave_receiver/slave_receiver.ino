#include <Wire.h>

char newer_L_X;
char newer_L_Y;
char newer_R_X;
char newer_R_Y;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(115200);
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany) {
  String str = "";
  while (Wire.available()) {
    char c = Wire.read();
    str += String(c);
    Serial.print(c);
  }
  Serial.print(F("\r\n"));
  newer_L_X = str.charAt(0);
  newer_L_Y = str.charAt(1);
  newer_R_X = str.charAt(2);
  newer_R_Y = str.charAt(3);
}

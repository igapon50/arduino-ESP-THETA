#define THETA_V
//#define THETA_S
//#define THETA_SC
//#define THETA_SC_TYPE_HATSUNE_MIKU
#define AP_PASSWORD "00101594"

#define CL_SSID "cl_ssid"
#define CL_PASSWORD "cl_password"
#define WEB_SERVER "192.168.1.1"
#define WEB_PORT "80"
#include "thetav2_1.h"

void setup()
{
  Serial.begin(115200);
  delay(10);
  THETA_init();
}

int value = 0;

void loop()
{
  byte buff[6]={};
  static char older_L_X = 'A';
  ++value;

  char buffer[1024] = {'\0'};
  char newer_L_X = 'H';//buff[0];
  if(older_L_X != newer_L_X){
    older_L_X = newer_L_X;
    uint8_t L_X = (uint8_t)(newer_L_X - 'A');
    uint8_t index = map(L_X, 0, 14, 0, sizeof(exposureCompensation)/sizeof(exposureCompensation[0])); //要素数が15以下ならmapで変換
    snprintf(buffer, sizeof(buffer), POST_REQUEST_BODY_setOptions_exposureCompensation, exposureCompensation[index]);
    if(!THETA_connect()){
      Serial.println("connection failed");
      return;
    }
    CommandRequest(buffer);
    CommandRequest(POST_REQUEST_BODY_takePicture);
  }
  delay(1000);
}

/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#define _ESP32_
#ifdef _ESP32_
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <aJSON.h>

#define THETA_V
#ifdef THETA_V
#define AP_PASSWORD "00101594"
#define AP_SSID "THETAYL" AP_PASSWORD ".OSC"
#else
#define AP_PASSWORD "00111871"
#define AP_SSID "THETAXS" AP_PASSWORD ".OSC"
#endif
#define WEB_SERVER "192.168.1.1"
#define WEB_PORT "80"
#include "thetav2_1.h"

String password = AP_PASSWORD;
String ssid     = AP_SSID;
String host = WEB_SERVER;
String port = WEB_PORT;
String g_fileName;
int result_timeout = 10000; //10秒

void GettingStarted(WiFiClient& client)
{
  String jsonString = "";
  String fileUrl = "";
  String privious_fileUrl = "";
  char buffer[1024] = {'\0'};
  char buff[1024] = {'\0'};
  const char *requests[] = {
    POST_REQUEST_BODY_takePicture,
    NULL
  };

  {
    Serial.println("----------state----------");
    margeString_POST_Request(buffer, 1024, "", "/osc/state");
    Serial.println(buffer);
    client.print(String(buffer));
    unsigned long timeout = millis();
    while(client.available() == 0){
      if(millis() - timeout > result_timeout){
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    while(client.available()){
      String line = client.readStringUntil('\r');
      line.trim();
      jsonString += line;
      Serial.println(line);
    }
    int index = jsonString.indexOf("{");
    jsonString = jsonString.substring(index);
    jsonString += "}";
    // aJson.parseの引数がchar*なので、jsonString.c_str()が使えない
    int len = jsonString.length();
    memset(buff, '\0', 1024);
    jsonString.toCharArray(buff,len);
    Serial.println(buff);
    aJsonObject* root = aJson.parse(buff);
//    aJsonObject* root = aJson.parse(jsonString.c_str());
    if(NULL == root){
      Serial.println("aJson.parse() failed root");
      return;
    }
    aJsonObject* state = aJson.getObjectItem(root, "state");
    if(NULL == state){
      Serial.println("aJson.parse() failed state");
      return;
    }
    aJsonObject* _apiVersion = aJson.getObjectItem(state, "_apiVersion");
    if(NULL == _apiVersion){
      Serial.println("aJson.parse() failed _apiVersion");
      return;
    }
    const int _apiVersion1 = _apiVersion->valueint;
    Serial.print("_apiVersion : ");
    Serial.println(_apiVersion1);
    if(_apiVersion1 < 2){
      // _apiVersion=1の場合
      aJsonObject* _latestFileUrl = aJson.getObjectItem(state, "_latestFileUri");
      if(NULL == _latestFileUrl){
        Serial.println("aJson.parse() failed _latestFileUrl");
        return;
      }
      const char* fileUrl1 = _latestFileUrl->valuestring;
      privious_fileUrl = fileUrl1;
      Serial.print("fileUri : ");
      Serial.println(privious_fileUrl);
      Serial.println("----------startSession----------");
      margeString_POST_Request(buffer, 1024, POST_REQUEST_BODY_startSession);
      Serial.println(buffer);
      client.print(String(buffer));
      unsigned long timeout = millis();
      while(client.available() == 0){
        if(millis() - timeout > result_timeout){
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }
      // Read all the lines of the reply from server and print them to Serial
      jsonString = "";
      while(client.available()){
        String line = client.readStringUntil('\r');
        line.trim();
        jsonString += line;
        Serial.println(line);
      }
      int index = jsonString.indexOf("{");
      jsonString = jsonString.substring(index);
      jsonString += "}";
      // aJson.parseの引数がchar*なので、jsonString.c_str()が使えない
      int len = jsonString.length();
      memset(buff, '\0', 1024);
      jsonString.toCharArray(buff,len);
      Serial.println(buff);
      aJsonObject* root = aJson.parse(buff);
//    aJsonObject* root = aJson.parse(jsonString.c_str());
      if(NULL == root){
        Serial.println("aJson.parse() failed root");
        return;
      }
      aJsonObject* results = aJson.getObjectItem(root, "results");
      if(NULL == results){
        Serial.println("aJson.parse() failed results");
        return;
      }
      aJsonObject* sessionId = aJson.getObjectItem(results, "sessionId");
      if(NULL == sessionId){
        Serial.println("aJson.parse() failed sessionId");
        return;
      }
      const char* sessionId1 = sessionId->valuestring;
      Serial.print("sessionId : ");
      Serial.println(sessionId1);
      Serial.println("----------clientVersion----------");
      margeString_POST_Request(buffer, 1024, POST_REQUEST_BODY_clientVersion);
      String str_rep = buffer;
      str_rep.replace(SETTION_ID, sessionId1);
      Serial.println(str_rep);
      client.print(str_rep);
      timeout = millis();
      while(client.available() == 0){
        if(millis() - timeout > result_timeout){
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }
      // Read all the lines of the reply from server and print them to Serial
      jsonString = "";
      while(client.available()){
        String line = client.readStringUntil('\r');
        line.trim();
        jsonString += line;
        Serial.println(line);
      }
    }else{
      // _apiVersion=2の場合
      aJsonObject* _latestFileUrl = aJson.getObjectItem(state, "_latestFileUrl");
      if(NULL == _latestFileUrl){
        Serial.println("aJson.parse() failed _latestFileUrl");
        return;
      }
      const char* fileUrl1 = _latestFileUrl->valuestring;
      privious_fileUrl = fileUrl1;
      Serial.print("fileUrl : ");
      Serial.println(privious_fileUrl);
    }
  }

  Serial.println("----------command_list----------");
  for(int no = 0 ; requests[no] != NULL ; no++){
    margeString_POST_Request(buffer, 1024, requests[no]);
    Serial.println(buffer);
    // This will send the request to the server
    client.println(String(buffer));
    unsigned long timeout = millis();
    while(client.available() == 0){
        if(millis() - timeout > result_timeout){
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
        String line = client.readStringUntil('\r');
        line.trim();
        Serial.println(line);
    }
  }
  {
    Serial.println("----------再接続----------");
    client.stop();
//    delay(1000);
    if(!client.connect(host.c_str(), port.toInt())){
      Serial.println("connection failed");
      return;
    }
  }
  delay(8000);
  do{
    Serial.println("----------state----------");
    margeString_POST_Request(buffer, 1024, "", "/osc/state");
    Serial.println(buffer);
    // This will send the request to the server
    client.print(String(buffer));
    unsigned long timeout = millis();
    while(client.available() == 0){
      if(millis() - timeout > result_timeout){
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    // Read all the lines of the reply from server and print them to Serial
    jsonString = "";
    while(client.available()){
      String line = client.readStringUntil('\r');
      line.trim();
      jsonString += line;
      Serial.println(line);
    }
    int index = jsonString.indexOf("{");
    jsonString = jsonString.substring(index);
    jsonString += "}";
    // aJson.parseの引数がchar*なので、jsonString.c_str()が使えない
    int len = jsonString.length();
    memset(buff, '\0', 1024);
    jsonString.toCharArray(buff,len);
    Serial.println(buff);
    aJsonObject* root = aJson.parse(buff);
//    aJsonObject* root = aJson.parse(jsonString.c_str());
    if(NULL == root){
      Serial.println("aJson.parse() failed. root");
      return;
    }
    aJsonObject* state = aJson.getObjectItem(root, "state");
    if(NULL == state){
      Serial.println("aJson.parse() failed. state");
      return;
    }
    aJsonObject* _latestFileUrl = aJson.getObjectItem(state, "_latestFileUrl");
    if(NULL == _latestFileUrl){
      Serial.println("aJson.parse() failed. _latestFileUrl");
      return;
    }
    const char* latestfileUrl = _latestFileUrl->valuestring;
    fileUrl = latestfileUrl;
    Serial.print("fileUrl : ");
    Serial.println(fileUrl);
  }while(fileUrl.equals(privious_fileUrl));

  delay(5000);
  {
    Serial.println("----------GET,url----------");
    String url = "http://";
    String Thumbneil = "?type=thumb";
    url += host;
    fileUrl.replace(url.c_str(), "");
    margeString_GET_Request(buffer, 1024, (fileUrl + Thumbneil).c_str());
    Serial.println(buffer);
    // This will send the request to the server
    client.print(String(buffer));
    unsigned long timeout = millis();
    while(client.available() == 0){
      if(millis() - timeout > result_timeout){
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    long count = 0;
    while(client.available()){
      char ch = client.read();
      count++;
      Serial.printf("%02x",ch);
    }
    Serial.printf("count = %ld\r\n",count);
  }
  return;
}

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid.c_str(), password.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    g_fileName = "";
}

int value = 0;

void loop()
{
    delay(5000);
    ++value;

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    if (!client.connect(host.c_str(), port.toInt())) {
        Serial.println("connection failed");
        return;
    }

    GettingStarted(client);

    Serial.println();
    Serial.println("closing connection");
}


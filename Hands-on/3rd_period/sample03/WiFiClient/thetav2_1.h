#ifndef _thetav2_1_h
#define _thetav2_1_h

#ifdef ARDUINO_ESP32_DEV
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <aJSON.h>

#ifdef THETA_V
#define AP_SSID "THETAYL" AP_PASSWORD ".OSC"
#elif defined THETA_S
#define AP_SSID "THETAXS" AP_PASSWORD ".OSC"
#elif defined THETA_SC
#define AP_SSID "THETAYJ" AP_PASSWORD ".OSC"
#elif defined THETA_SC_TYPE_HATSUNE_MIKU
#define AP_SSID "THETAHM" AP_PASSWORD ".OSC"
#endif

#ifndef AUTH_UUID
#define AUTH_UUID "00000000-0000-0000-0000-000000000000"
#endif
#ifndef AP_SSID
#define AP_SSID "ap_ssid"
#endif
#ifndef AP_PASSWORD
#define AP_PASSWORD "ap_password"
#endif
#ifndef CL_SSID
#define CL_SSID "cl_ssid"
#endif
#ifndef CL_PASSWORD
#define CL_PASSWORD "cl_password"
#endif
#ifndef DIGEST_USER
#define DIGEST_USER AP_SSID
#endif
#ifndef DIGEST_PASSWORD
#define DIGEST_PASSWORD AP_PASSWORD
#endif
#ifndef WEB_SERVER
#define WEB_SERVER "192.168.1.1"
#endif
#ifndef WEB_PORT
#define WEB_PORT "80"
#endif
#define SETTION_ID "SID_0001"

String password = AP_PASSWORD;
String ssid     = AP_SSID;
String host = WEB_SERVER;
String port = WEB_PORT;
int result_timeout = 10000; //10秒
WiFiClient client;

static const char* const POST_REQUEST_BODY_takePicture = "{\r\n"
"	\"name\":\"camera.takePicture\"\r\n"
"}\r\n";

static const char* const POST_REQUEST_BODY_closeSession = "{\r\n"
"	\"name\":\"camera.closeSession\",\r\n"
"	\"parameters\":{\r\n"
"		\"sessionId\": \"" SETTION_ID "\"\r\n"
"	}\r\n"
"}\r\n";

static const char* const POST_REQUEST_BODY_clientVersion = "{\r\n"
"	\"name\":\"camera.setOptions\",\r\n"
"	\"parameters\":{\r\n"
"		\"sessionId\": \"" SETTION_ID "\",\r\n"
"		\"options\":{\r\n"
"			\"clientVersion\": 2\r\n"
"		}\r\n"
"	}\r\n"
"}\r\n";

static const char* const POST_REQUEST_BODY_startSession = "{\r\n"
"	\"name\":\"camera.startSession\",\r\n"
"	\"parameters\":{}\r\n"
"}\r\n";

static const char* const POST_REQUEST_BODY_getOptions__bluetoothPower = "{\r\n"
"	\"name\":\"camera.getOptions\",\r\n"
"	\"parameters\":{\r\n"
"		\"optionNames\":[\r\n"
"			\"_bluetoothPower\",\r\n"
"			\"_bluetoothPowerSupport\"\r\n"
"		]\r\n"
"	}\r\n"
"}\r\n";

static const char* const POST_REQUEST_BODY_setOptions__bluetoothPower_ON = "{\r\n"
"	\"name\":\"camera.setOptions\",\r\n"
"	\"parameters\":{\r\n"
"		\"options\":{\r\n"
"			\"_bluetoothPower\":\"ON\"\r\n"
"		}\r\n"
"	}\r\n"
"}\r\n";

static const char* const POST_REQUEST_BODY_setOptions__shutterVolume_0 = "{\r\n"
"  \"name\":\"camera.setOptions\",\r\n"
" \"parameters\":{\r\n"
"   \"options\":{\r\n"
"     \"_shutterVolume\":0\r\n"
"   }\r\n"
" }\r\n"
"}\r\n";

static const char* const POST_REQUEST_BODY_setOptions_sleepDelay_OFF = "{\r\n"
"  \"name\":\"camera.setOptions\",\r\n"
" \"parameters\":{\r\n"
"   \"options\":{\r\n"
"     \"sleepDelay\":65535\r\n"
"   }\r\n"
" }\r\n"
"}\r\n";

static const char* const POST_REQUEST_BODY_setBluetoothDevice = "{\r\n"
"	\"name\":\"camera._setBluetoothDevice\",\r\n"
"	\"parameters\":{\"uuid\":\""AUTH_UUID"\"}\r\n"
"}\r\n";

// POST http://192.168.1.1/osc/commands/execute
static const char* const POST_REQUEST_BODY_setAccessPoint = "{\r\n"
"	\"name\":\"camera._setAccessPoint\",\r\n"
"  \"parameters\":{\r\n"
"   \"ssid\":\"" CL_SSID "\",\r\n"
"		\"security\":\"WPA/WPA2 PSK\",\r\n"
"		\"password\":\"" CL_PASSWORD "\"\r\n"
"	}\r\n"
"}\r\n";

static const char* const POST_REQUEST_BODY__listAccessPoints = "{\r\n"
"  \"name\":\"camera._listAccessPoints\"\r\n"
"}\r\n";

static const char* const POST_REQUEST_BODY__deleteAccessPoint = "{\r\n"
"  \"name\":\"camera._deleteAccessPoint\"\r\n"
"  \"parameters\":{\r\n"
"   \"ssid\":\"" CL_SSID "\",\r\n"
"}\r\n";

static const char* const exposureCompensation[] = {"-2.0", "-1.7", "-1.3", "-1.0", "-0.7", "-0.3", "0.0", "0.3", "0.7", "1.0", "1.3", "1.7", "2.0"};
static const char* const POST_REQUEST_BODY_setOptions_exposureCompensation = "{\r\n"
"  \"name\":\"camera.setOptions\",\r\n"
" \"parameters\":{\r\n"
"   \"options\":{\r\n"
"     \"exposureCompensation\":%s\r\n"
"   }\r\n"
" }\r\n"
"}\r\n";

// dststr 合成後の文字列
// dstlen dststrのバッファサイズ
// path FileUrl
// ip 192.168.1.1(固定)
// port 80(無指定)
// method GET,POST(固定)
void margeString_GET_Request(char *dststr, size_t dstlen, const char* const path){
	static const char* const REQUEST_HEADER_1 = " HTTP/1.1\r\n"
	"Connection: keep-alive\r\n"
	"Content-Type: application/json\r\n"
	"Accept: application/json\r\n"
	"Content-Length: ";
	static const char* const REQUEST_HEADER_2 = "\r\nHost: "WEB_SERVER"\r\n"
	"User-Agent: Apache-HttpClient/4.5.3 (Java/1.8.0_144)\r\n"
	"Authorization: Basic cmVjZXB0b3I6cmVjZXB0b3I=\r\n"
	"\r\n";

	//パラメータノーチェック
	snprintf(dststr, dstlen, "GET %s%s%d%s", path, REQUEST_HEADER_1, 0, REQUEST_HEADER_2);
	return;
}

// dststr 合成後の文字列
// dstlen dststrのバッファサイズ
// body data 送信するデータ
// path /osc/commands/execute,/osc/state
// ip 192.168.1.1(固定)
// port 80(無指定)
// method GET,POST(固定)
void margeString_POST_Request(char *dststr, size_t dstlen, const char* const body, const char* const path = "/osc/commands/execute"){
	static const char* const REQUEST_HEADER_1 = " HTTP/1.1\r\n"
	"Connection: keep-alive\r\n"
	"Content-Type: application/json\r\n"
	"Accept: application/json\r\n"
	"Content-Length: ";
	static const char* const REQUEST_HEADER_2 = "\r\nHost: "WEB_SERVER"\r\n"
	"User-Agent: Apache-HttpClient/4.5.3 (Java/1.8.0_144)\r\n"
	"Authorization: Basic cmVjZXB0b3I6cmVjZXB0b3I=\r\n"
	"\r\n";

	//パラメータノーチェック
	size_t length = strlen(body);
	snprintf(dststr, dstlen, "POST %s%s%d%s%s", path, REQUEST_HEADER_1, length, REQUEST_HEADER_2, body);
	return;
}

// WiFiClient client;はグローバルで定義して参照させること。引数にすると接続が維持されないようだ。
int THETA_connect()
{
  return(client.connect(host.c_str(), port.toInt()));
}

// WiFiClient client;はグローバルで定義して参照させること。引数にすると接続が維持されないようだ。
void THETA_initialized()
{
  String jsonString = "";
  String privious_fileUrl = "";
  char buffer[1024] = {'\0'};
  char buff[1024] = {'\0'};

  {
    Serial.println("----------state----------");
    margeString_POST_Request(buffer, 1024, "", "/osc/state");
    Serial.println(buffer);
    client.print(String(buffer));
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > result_timeout) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    while (client.available()) {
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
    jsonString.toCharArray(buff, len);
    Serial.println(buff);
    aJsonObject* root = aJson.parse(buff);
    //    aJsonObject* root = aJson.parse(jsonString.c_str());
    if (NULL == root) {
      Serial.println("aJson.parse() failed root");
      return;
    }
    aJsonObject* state = aJson.getObjectItem(root, "state");
    if (NULL == state) {
      Serial.println("aJson.parse() failed state");
      return;
    }
    aJsonObject* _apiVersion = aJson.getObjectItem(state, "_apiVersion");
    if (NULL == _apiVersion) {
      Serial.println("aJson.parse() failed _apiVersion");
      return;
    }
    const int _apiVersion1 = _apiVersion->valueint;
    Serial.print("_apiVersion : ");
    Serial.println(_apiVersion1);
    if (_apiVersion1 < 2) {
      // _apiVersion=1の場合
      aJsonObject* _latestFileUrl = aJson.getObjectItem(state, "_latestFileUri");
      if (NULL == _latestFileUrl) {
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
      while (client.available() == 0) {
        if (millis() - timeout > result_timeout) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }
      // Read all the lines of the reply from server and print them to Serial
      jsonString = "";
      while (client.available()) {
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
      jsonString.toCharArray(buff, len);
      Serial.println(buff);
      aJsonObject* root = aJson.parse(buff);
      //    aJsonObject* root = aJson.parse(jsonString.c_str());
      if (NULL == root) {
        Serial.println("aJson.parse() failed root");
        return;
      }
      aJsonObject* results = aJson.getObjectItem(root, "results");
      if (NULL == results) {
        Serial.println("aJson.parse() failed results");
        return;
      }
      aJsonObject* sessionId = aJson.getObjectItem(results, "sessionId");
      if (NULL == sessionId) {
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
      while (client.available() == 0) {
        if (millis() - timeout > result_timeout) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }
      // Read all the lines of the reply from server and print them to Serial
      jsonString = "";
      while (client.available()) {
        String line = client.readStringUntil('\r');
        line.trim();
        jsonString += line;
        Serial.println(line);
      }
    } else {
      // _apiVersion=2の場合
      aJsonObject* _latestFileUrl = aJson.getObjectItem(state, "_latestFileUrl");
      if (NULL == _latestFileUrl) {
        Serial.println("aJson.parse() failed _latestFileUrl");
        return;
      }
      const char* fileUrl1 = _latestFileUrl->valuestring;
      privious_fileUrl = fileUrl1;
      Serial.print("fileUrl : ");
      Serial.println(privious_fileUrl);
    }
  }
  return;
}

// WiFiClient client;はグローバルで定義して参照させること。引数にすると接続が維持されないようだ。
void THETA_init()
{
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
  Serial.print("connecting to ");
  Serial.println(host);

  delay(5000);
  Serial.print("connecting to ");
  Serial.println(host);
  // Use WiFiClient class to create TCP connections
  if (!THETA_connect()) {
    Serial.println("connection failed");
    return;
  }
  THETA_initialized();
}

// WiFiClient client;はグローバルで定義して参照させること。引数にすると接続が維持されないようだ。
void CommandRequest(const char* const request)
{
  char buffer[1024] = {'\0'};

  Serial.println("----------command----------");
  margeString_POST_Request(buffer, sizeof(buffer), request);
  Serial.println(buffer);
  // This will send the request to the server
  client.println(String(buffer));
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > result_timeout) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    line.trim();
    Serial.println(line);
  }
  {
    Serial.println("----------再接続----------");
    client.stop();
    // delay(1000);
    if (!THETA_connect()) {
      Serial.println("connection failed");
      return;
    }
  }
  return;
}

//ESP-IDFサンプル
// param s : socket
// return true:成功
// return false:失敗
#if 0
bool initialize_requests(int s){
	int r;
	char recv_buf[64];
	char buffer[1024] = {'\0'};
	const char* const requests[] = {
		POST_REQUEST_BODY_setBluetoothDevice,
		POST_REQUEST_BODY_setOptions__bluetoothPower,
		POST_REQUEST_BODY_getOptions__bluetoothPower,
//		POST_REQUEST_BODY_takePicture,
		NULL
	};
	for(int no = 0 ; requests[no] != NULL ; no++){
		margeString_POST_Request(buffer, 1024, requests[no]);
		ESP_LOGI(WIFI_TAG, "%s", buffer);
		if(write(s, buffer, strlen(buffer)) < 0){
			ESP_LOGE(WIFI_TAG, "... socket send failed");
			close(s);
			vTaskDelay(4000 / portTICK_PERIOD_MS);
			return(false);
		}
		ESP_LOGI(WIFI_TAG, "... socket send success");
		struct timeval receiving_timeout;
		receiving_timeout.tv_sec = 5;
		receiving_timeout.tv_usec = 0;
		if(setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout, sizeof(receiving_timeout)) < 0){
			ESP_LOGE(WIFI_TAG, "... failed to set socket receiving timeout");
			close(s);
			vTaskDelay(4000 / portTICK_PERIOD_MS);
			return(false);
		}
		ESP_LOGI(WIFI_TAG, "... set socket receiving timeout success");
		/* Read HTTP response */
		do{
			bzero(recv_buf, sizeof(recv_buf));
			r = read(s, recv_buf, sizeof(recv_buf)-1);
			for(int i = 0; i < r; i++){
				putchar(recv_buf[i]);
			}
		}while(r > 0);
		ESP_LOGI(WIFI_TAG, "... done reading from socket. Last read return=%d\r\n", r);
	}
	return(true);
}
#endif

#endif

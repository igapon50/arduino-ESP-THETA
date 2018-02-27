#ifndef _thetav2_1_h
#define _thetav2_1_h

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

static const char *POST_REQUEST_BODY_takePicture = "{\r\n"
"	\"name\":\"camera.takePicture\"\r\n"
"}\r\n";

static const char *POST_REQUEST_BODY_closeSession = "{\r\n"
"	\"name\":\"camera.closeSession\",\r\n"
"	\"parameters\":{\r\n"
"		\"sessionId\": \"" SETTION_ID "\"\r\n"
"	}\r\n"
"}\r\n";

static const char *POST_REQUEST_BODY_clientVersion = "{\r\n"
"	\"name\":\"camera.setOptions\",\r\n"
"	\"parameters\":{\r\n"
"		\"sessionId\": \"" SETTION_ID "\",\r\n"
"		\"options\":{\r\n"
"			\"clientVersion\": 2\r\n"
"		}\r\n"
"	}\r\n"
"}\r\n";

static const char *POST_REQUEST_BODY_startSession = "{\r\n"
"	\"name\":\"camera.startSession\",\r\n"
"	\"parameters\":{}\r\n"
"}\r\n";

static const char *POST_REQUEST_BODY_getOptions__bluetoothPower = "{\r\n"
"	\"name\":\"camera.getOptions\",\r\n"
"	\"parameters\":{\r\n"
"		\"optionNames\":[\r\n"
"			\"_bluetoothPower\",\r\n"
"			\"_bluetoothPowerSupport\"\r\n"
"		]\r\n"
"	}\r\n"
"}\r\n";

static const char *POST_REQUEST_BODY_setOptions__bluetoothPower = "{\r\n"
"	\"name\":\"camera.setOptions\",\r\n"
"	\"parameters\":{\r\n"
"		\"options\":{\r\n"
"			\"_bluetoothPower\":\"ON\"\r\n"
"		}\r\n"
"	}\r\n"
"}\r\n";

static const char *POST_REQUEST_BODY_setBluetoothDevice = "{\r\n"
"	\"name\":\"camera._setBluetoothDevice\",\r\n"
"	\"parameters\":{\"uuid\":\""AUTH_UUID"\"}\r\n"
"}\r\n";

// POST http://192.168.1.1/osc/commands/execute
static const char *POST_REQUEST_BODY_setAccessPoint = "{\r\n"
"	\"name\":\"camera._setAccessPoint\",\r\n"
"	\"parameters\":{\r\n"
"		\"ssid\":\"" CL_SSID "\",\r\n"
"		\"security\":\"WPA/WPA2 PSK\",\r\n"
"		\"password\":\"" CL_PASSWORD "\"\r\n"
"	}\r\n"
"}\r\n";

// dststr 合成後の文字列
// dstlen dststrのバッファサイズ
// path FileUrl
// ip 192.168.1.1(固定)
// port 80(無指定)
// method GET,POST(固定)
void margeString_GET_Request(char *dststr, size_t dstlen, const char *path){
	static const char *REQUEST_HEADER_1 = " HTTP/1.1\r\n"
	"Connection: keep-alive\r\n"
	"Content-Type: application/json\r\n"
	"Accept: application/json\r\n"
	"Content-Length: ";
	static const char *REQUEST_HEADER_2 = "\r\nHost: "WEB_SERVER"\r\n"
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
void margeString_POST_Request(char *dststr, size_t dstlen, const char *body, const char *path = "/osc/commands/execute"){
	static const char *REQUEST_HEADER_1 = " HTTP/1.1\r\n"
	"Connection: keep-alive\r\n"
	"Content-Type: application/json\r\n"
	"Accept: application/json\r\n"
	"Content-Length: ";
	static const char *REQUEST_HEADER_2 = "\r\nHost: "WEB_SERVER"\r\n"
	"User-Agent: Apache-HttpClient/4.5.3 (Java/1.8.0_144)\r\n"
	"Authorization: Basic cmVjZXB0b3I6cmVjZXB0b3I=\r\n"
	"\r\n";

	//パラメータノーチェック
	size_t length = strlen(body);
	snprintf(dststr, dstlen, "POST %s%s%d%s%s", path, REQUEST_HEADER_1, length, REQUEST_HEADER_2, body);
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
	const char *requests[] = {
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


#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>


const char* ssid = "math";
const char* password = "Ali9137266252@1372";

const char* host = "192.168.1.7";
const int httpsPort = 80;
struct userStruct
{
	int id;
	String name;
	String family;
	bool isActive;
	String imei;
	String macAddress;
	String token;
	bool isData;
};
userStruct doorUers[10];
userStruct tempUser;
enum state
{
	user = 0
};
// Use web browser to view and copy
// SHA1 fingerprint of the certificate
//const char* fingerprint = "CF 05 98 89 CA FF 8E D8 5E 5C E0 C2 E4 F7 E6 C3 C7 50 DD 5C";
int  userState = -1;
WiFiServer server(80);
void setup() {
	Serial.begin(115200);
	delay(10);

	// prepare GPIO2
	pinMode(2, OUTPUT);
	digitalWrite(2, 0);

	// Connect to WiFi network
	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");

	// Start the server
	server.begin();
	Serial.println("Server started");

	// Print the IP address
	Serial.println(WiFi.localIP());
	/*for (int  i = 0; i < 10; i++)
	{
		SaveScheduleEEporomInFirstState(-1, "", "", false, "", "", "", false);
	}*/
}
void loop() {
	WiFiClient client = server.available();

	if (client) {

		Serial.println("Client connected.");

		while (client.connected()) {

			if (client.available()) {

				/*String command = client.readString();
				Serial.println(command);*/
				String line = client.readStringUntil('\n');
				Serial.println(line);
				/*if (line == "\r") {
				Serial.println("headers received");
				break;
				}*/
				Serial.println("user is :" + String(line.indexOf("user")));

				if (line.indexOf("user") != -1) {
					userState = 0;
				}
				if (line.indexOf("all") != -1) {
					userState = 1;
				}

				if (line.startsWith("{")) {
					Serial.println("esp8266/Arduino CI successfull!");
					//parser(line);
					switch (userState)
					{
					case 0:
						AddUser(line);
						break;
					case 1:
						getAllUser();
						break;
					default:
						break;
					}

					break;
				}






			}
			else
			{

			}
		}

		Serial.println("Client disconnected.");
		client.flush();
		String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
		s += "high";
		s += "</html>\n";

		// Send the response to the client
		client.print(s);
		delay(1);
		Serial.println("Client disonnected");

		client.stop();

		// Prepare the response



	}


}
void parser(String json) {
	Serial.println("json is" + json);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);

	// Test if parsing succeeds.
	if (!root.success()) {
		Serial.println("parseObject() failed");
		return;
	}
	const char* name = root["name"];
	const char* family = root["family"];
	const char* imei = root["imei"];
	const char* mac = root["mac"];
	bool isActive = root["isActive"];
	doorUers[0].family = family;
	doorUers[0].name = name;

	Serial.println("val is " + doorUers[0].family + doorUers[0].name);

}
void AddUser(String json) {
	int i = 0;
	readSchedulestruct(i);
	/*while (tempUser.isData && i <= 10)
	{
		i++;
		readSchedulestruct(i);

	}
*/
	Serial.println("fistrs" + String(tempUser.name));
	Serial.println("json is" + json);
	Serial.println("id is" + String(i));

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);

	// Test if parsing succeeds.
	if (!root.success()) {
		Serial.println("parseObject() failed");
		return;
	}
	const char* name = root["name"];
	const char* family = root["family"];
	const char* imei = root["imei"];
	const char* mac = root["mac"];
	bool isActive = root["isActive"];
	doorUers[0].family = family;
	doorUers[0].name = name;
	doorUers[0].imei = imei;
	doorUers[0].macAddress = mac;
	doorUers[0].isActive = isActive;
	SaveScheduleEEporomInFirstState(i, name, family, isActive, imei, mac, "", true);


}
void getAllUser() {
	for (int i = 0; i < 10; i++)
	{
		readSchedulestruct(i);
		Serial.println(tempUser.family);
	}
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["name"] =tempUser.name ;
	
	root.printTo(Serial);
}
void SaveScheduleEEporomInFirstState(int id, String name, String family, bool isActive, String imei, String mac, String token, bool isData) {
	Serial.println("val is:::: " + name + String(id));
	int eeAddress = 0;
	eeAddress += id * sizeof(userStruct); //Move address to the next byte after float 'f'.
	userStruct customVar = {
		id,name,family,isActive,imei,mac,token,isData
	};


	EEPROM.put(eeAddress, customVar);
	EEPROM.commit();
	Serial.print("Written custom data type! \n\nView the example sketch eeprom_get to see how you can retrieve the values!");
}
void readSchedulestruct(int id) {
	//MySchedule customVar;
	int eeAddress = id * sizeof(userStruct);
	EEPROM.get(eeAddress, tempUser);

	Serial.println("Read custom object from EEPROM: ");
	/*Serial.println(tempSchedule.day);
	Serial.println(tempSchedule.sHour);
	Serial.println(tempSchedule.sMin);
	Serial.println(tempSchedule.fHour);
	Serial.println(tempSchedule.fMin);*/
	//return TodaySchedule;

}
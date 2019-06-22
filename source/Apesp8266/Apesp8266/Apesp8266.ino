/*
* Copyright (c) 2015, Majenko Technologies
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* * Neither the name of Majenko Technologies nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

bool state = true;
struct MySchedule
{
	byte day;
	bool isActive;
	byte sHour;
	byte sMin;
	byte fHour;
	byte fMin;
};
String daysOfWeek[7] = {
	"Monday"
	,	"Tuesday"
	,	"Wednesday"
	,	"Thursday"
	,	"Friday",
	"Saturday",
	"Sunday",

};
/* Set these to your desired credentials. */
const char *ssid = "D-123";
const char *password = "953281ali";
const char* host = "192.168.1.7";
ESP8266WebServer wserver(80);
WiFiServer server(80);
MySchedule TodaySchedule;
MySchedule tempSchedule;//برای نگه دار مقادیر میانی
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
* connected to this access point to see it.
*/
void handleRoot() {
	wserver.send(200, "text/html", "{\"hi\":\"ali\"}");
	

}

void setup() {
	delay(1000);

	Serial.begin(115200);
	
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	wserver.on("/", handleRoot);
	wserver.begin();
	Serial.println("HTTP server started");
}

void loop() {
	wserver.handleClient();
	
	WiFiClient client = server.available();
	if (!client) {
		return;
	}

	// Wait until the client sends some data
	Serial.println("new client");
	while (!client.available()) {
		delay(1);
	}

	// Read the first line of the request
	String req = client.readStringUntil('\r');
	Serial.println(req);
	client.flush();
	while (client.available()) {
		String line = client.readString();
		Serial.print(line);
	}
}
void sendRequest() {
	WiFiClient client;

	Serial.print("connecting to ");
	Serial.println(host);
	if (!client.connect(host, 80)) {
		Serial.println("connection failed");
		//return;
	}


	String url = "/";
	Serial.print("requesting URL: ");
	Serial.println(url);

	client.print(String("GET ") + url + " HTTP/1.1\r\n" +
		"Host: " + host + "\r\n" +
		"User-Agent: BuildFailureDetectorESP8266\r\n" +
		"Connection: close\r\n\r\n");

	Serial.println("request sent");
	while (client.connected()) {
		String line = client.readStringUntil('\n');
		Serial.println(line);
		/*if (line == "\r") {
		Serial.println("headers received");
		break;
		}*/
		if (line.startsWith("{")) {
			Serial.println("esp8266/Arduino CI successfull!");
			parser(line);
		}
	}
	String line = client.readStringUntil('\n');
	if (line.startsWith("{")) {
		Serial.println("esp8266/Arduino CI successfull!");
		parser(line);
	}
	else {
		Serial.println("esp8266/Arduino CI has failed");
	}
	Serial.println("reply was:");
	Serial.println("==========");
	Serial.println(line);
	Serial.println("==========");
	Serial.println("closing connection");
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
	const char* sensor = root["image"];
	Serial.println("val is " + String(sensor));

}
void SaveScheduleEEporomInFirstState(int address, byte day, byte sHour, byte sMin, byte fHour, byte fMin, bool isActive) {
	int eeAddress = 0;
	eeAddress += address * sizeof(MySchedule); //Move address to the next byte after float 'f'.
	MySchedule customVar = {
		day,isActive,sHour,sMin,fHour,fMin
	};
	EEPROM.put(eeAddress, customVar);
	Serial.print("Written custom data type! \n\nView the example sketch eeprom_get to see how you can retrieve the values!");
}
void readSchedulestruct(int day) {
	//MySchedule customVar;
	int eeAddress = day * sizeof(MySchedule);
	EEPROM.get(eeAddress, tempSchedule);

	Serial.println("Read custom object from EEPROM: ");
	/*Serial.println(tempSchedule.day);
	Serial.println(tempSchedule.sHour);
	Serial.println(tempSchedule.sMin);
	Serial.println(tempSchedule.fHour);
	Serial.println(tempSchedule.fMin);*/
	//return TodaySchedule;

}
void readScheduleEEporom(int day) {
	//MySchedule customVar;
	int eeAddress = day * sizeof(MySchedule);
	EEPROM.get(eeAddress, TodaySchedule);
	Serial.println("Read custom object from EEPROM: ");
	/*Serial.println(TodaySchedule.day);
	Serial.println(TodaySchedule.sHour);
	Serial.println(TodaySchedule.sMin);
	Serial.println(TodaySchedule.fHour);
	Serial.println(TodaySchedule.fMin);*/
	//return TodaySchedule;

}
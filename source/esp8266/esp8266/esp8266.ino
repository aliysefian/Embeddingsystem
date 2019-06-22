/*
*  HTTP over TLS (HTTPS) example sketch
*
*  This example demonstrates how to use
*  WiFiClientSecure class to access HTTPS API.
*  We fetch and display the status of
*  esp8266/Arduino project continuous integration
*  build.
*
*  Created by Ivan Grokhotkov, 2015.
*  This example is in public domain.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>


const char* ssid = "math";
const char* password = "Ali9137266252@1372";

const char* host = "192.168.1.7";
const int httpsPort = 80;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
//const char* fingerprint = "CF 05 98 89 CA FF 8E D8 5E 5C E0 C2 E4 F7 E6 C3 C7 50 DD 5C";

void setup() {
	Serial.begin(115200);
	Serial.println();
	Serial.print("connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	// Use WiFiClientSecure class to create TLS connection
	WiFiClient client;
	
	Serial.print("connecting to ");
	Serial.println(host);
	if (!client.connect(host,80)) {
		Serial.println("connection failed");
		//return;
	}


	String url = "/shoping/public/api/slider/list";
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

void loop() {
}

void parser(String json) {
	Serial.println("json is"+json);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);

	// Test if parsing succeeds.
	if (!root.success()) {
		Serial.println("parseObject() failed");
		return;
	}
	const char* sensor = root["image"];
	Serial.println("val is "+String(sensor) );

}
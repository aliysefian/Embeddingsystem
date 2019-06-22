/*
 Name:		espSimple.ino
 Created:	9/7/2017 10:41:17 AM
 Author:	Ali
  برای دایی
*/
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#define DEBUG true

SoftwareSerial esp8266(2, 3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
							  // This means that you need to connect the TX line from the esp to the Arduino's pin 2
							  // and the RX line from the esp to the Arduino's pin 3
// the setup function runs once when you press reset or power the board
int pin0 = A0;
int pin1 = A1;
int pin2 = A3;
void setup() {
	Serial.begin(9600);
	esp8266.begin(9600); // your esp's baud rate might be different
						 /////sendData("AT+RST\r\n", 2000, DEBUG); // reset module
	sendData("AT+CWMODE=2\r\n", 1000, DEBUG); // configure as access point
	sendData("AT+CIFSR\r\n", 1000, DEBUG); // get ip address
	sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); // configure for multiple connections
	sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // turn on server on port 80
	Serial.println("ready");
	pinMode(pin0, OUTPUT);
	pinMode(pin1, OUTPUT);
	pinMode(pin2, OUTPUT);
	digitalWrite(pin2, HIGH);
	delay(1000);
	digitalWrite(pin2, LOW);

}
byte connectionId;
// the loop function runs over and over again until power down or reset
void loop() {
	while (esp8266.available())
	{
		/*char c = esp8266.read();
		Serial.write(c);*/

		String c = esp8266.readStringUntil('\n'); // read the next character.										  //if (c.startsWith("{"))
		Serial.println(c);
		/*if (c.indexOf("active") != -1) {
			stateRequest = 5;
		}
		else if (c.indexOf("getmember") != -1) {
			stateRequest = 4;
		}
		else if (c.indexOf("door") != -1) {
			stateRequest = 3;
		}
		else if (c.indexOf("user") != -1) {
			stateRequest = 1;
		}
		else if (c.indexOf("all") != -1) {
			stateRequest = 2;
		}
*/
		if (c.indexOf("+IPD,") != -1)
		{

			connectionId = splitStringIPD(c);
			Serial.println("sssp=" + String(connectionId));
			if (c.indexOf("relay/1/on") != -1) {
				digitalWrite(pin0, HIGH);

			}
			if (c.indexOf("relay/1/off") != -1) {
				digitalWrite(pin0, LOW);

			}
			if (c.indexOf("relay/2/on") != -1) {
				digitalWrite(pin1, HIGH);

			}
			if (c.indexOf("relay/2/off") != -1) {
				digitalWrite(pin1, LOW);

			}if (c.indexOf("relay/3/on") != -1) {
				digitalWrite(pin2, HIGH);

			}
			if (c.indexOf("relay/3/off") != -1) {
				digitalWrite(pin2, LOW);

			}if (c.indexOf("all/trigger") != -1) {
				digitalWrite(pin2, HIGH);
				digitalWrite(pin1, HIGH);
				digitalWrite(pin0, HIGH);

			}
			if (c.indexOf("all/off") != -1) {
				digitalWrite(pin2, LOW);
				digitalWrite(pin1,LOW);
				digitalWrite(pin0,LOW);

			}
			//Serial.println("oooooo=" + String(stateRequest));
			delay(1000);
			req(connectionId);

		}


	}
}
void sendData(String command, const int timeout, boolean debug)
{
	String response = "";

	esp8266.print(command); // send the read character to the esp8266

	long int time = millis();

	while ((time + timeout) > millis())
	{
		while (esp8266.available())
		{

			// The esp has data so display its output to the serial window 
			char c = esp8266.read(); // read the next character.
			response += c;
		}
	}

	if (debug)
	{
		Serial.print(response);
	}

	//return response;
}
byte splitStringIPD(String data) {
	int st = data.indexOf(",");
	int fi = data.lastIndexOf(",");
	String s = data.substring(st + 1, fi);
	Serial.println("yyyy=" + s);
	return s.toInt();
}
void req(int connectionId) {

	//delay(1000);
	String name = "fooo";
	//int connectionId = esp8266.read() - 48; // subtract 48 because the read() function returns 

	Serial.println("commmmmm=" + String(connectionId));										// the ASCII decimal value and 0 (the first decimal number) starts at 48

	String webpage = "HTTP/1.1 200 OK\r\n Content-Type: text/html; charset=utf-8\r\n\r\n ";

	String cipSend = "AT+CIPSEND=";
	cipSend += connectionId;
	cipSend += ",";
	cipSend += webpage.length();
	cipSend += "\r\n";

	sendData(cipSend, 1000, DEBUG);
	sendData(webpage, 1000, DEBUG);
	 webpage = "<!DOCTYPE html><html><head><title>IOT</title></head><body> <div>  <a href=\"http://192.168.4.1/relay/1/on \">    ";

	 cipSend = "AT+CIPSEND=";
	cipSend += connectionId;
	cipSend += ",";
	cipSend += webpage.length();
	cipSend += "\r\n";

	sendData(cipSend, 1000, DEBUG);
	sendData(webpage, 1000, DEBUG);
	webpage = "<button> relay 1 ON</button></a>  <a href=\"http://192.168.4.1/relay/1/off \"> <button> relay 1 OFF</button></a> </div>";

	 cipSend = "AT+CIPSEND=";
	cipSend += connectionId;
	cipSend += ",";
	cipSend += webpage.length();
	cipSend += "\r\n";
	sendData(cipSend, 1000, DEBUG);
	sendData(webpage, 1000, DEBUG);
	webpage = "<div><a href=\"http://192.168.4.1/relay/2/on \"> <button> relay 2 ON</button></a>  <a href=\"http://192.168.4.1/relay/2/off \">";

	 cipSend = "AT+CIPSEND=";
	cipSend += connectionId;
	cipSend += ",";
	cipSend += webpage.length();
	cipSend += "\r\n";
	sendData(cipSend, 1000, DEBUG);
	sendData(webpage, 1000, DEBUG);
	webpage = " <button> relay 2 OFF</button></a> </div><div> <a href=\"http://192.168.4.1/relay/3/on \"> <button> relay 3 ON</button></a>";

	 cipSend = "AT+CIPSEND=";
	cipSend += connectionId;
	cipSend += ",";
	cipSend += webpage.length();
	cipSend += "\r\n";
	sendData(cipSend, 1000, DEBUG);
	sendData(webpage, 1000, DEBUG);	
	webpage = "<a href=\"http://192.168.4.1/relay/3/off \"> <button> relay 3 OFF</button></a>";

	 cipSend = "AT+CIPSEND=";
	cipSend += connectionId;
	cipSend += ",";
	cipSend += webpage.length();
	cipSend += "\r\n";
	sendData(cipSend, 1000, DEBUG);
	sendData(webpage, 1000, DEBUG);
	webpage = "<a href=\"http://192.168.4.1/all/off \"> <button> All OFF</button></a><a href=\"http://192.168.4.1/all/trigger \"> <button> All triigger</button></a>";

	 cipSend = "AT+CIPSEND=";
	cipSend += connectionId;
	cipSend += ",";
	cipSend += webpage.length();
	cipSend += "\r\n";
	sendData(cipSend, 1000, DEBUG);
	sendData(webpage, 1000, DEBUG);
	
	webpage = "  </div></body></html>";
	 cipSend = "AT+CIPSEND=";
	cipSend += connectionId;
	cipSend += ",";
	cipSend += webpage.length();
	cipSend += "\r\n";

	sendData(cipSend, 1000, DEBUG);
	sendData(webpage, 1000, DEBUG);
	String closeCommand = "AT+CIPCLOSE=";
	closeCommand += connectionId; // append connection id
	closeCommand += "\r\n";

	sendData(closeCommand, 3000, DEBUG);


}
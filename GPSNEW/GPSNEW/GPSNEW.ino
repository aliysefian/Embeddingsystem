/*
 Name:		GPSNEW.ino
 Created:	9/4/2017 12:31:39 PM
 Author:	Ali
*/
#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial Serial1(8, 9);
String target_phone = "";
int pinGas = A0;
int pinPOsitionFind = A0;
#define RST 2
byte cnt = 0;
bool gpsFlag = false;
bool gprsFlag = true;
String postion[2];
String gpsTime = "";
String gpsImei = "867567021341311";
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	Serial1.begin(115200);
	gpsFlag = false;
	digitalWrite(RST, HIGH);
	delay(5000);
	digitalWrite(RST, LOW);
	delay(500);
	for (int i = 0; i < 2; i++) {
		sendData("AT+IPR=9600", 5000, DEBUG);
	}
	pinMode(pinGas, OUTPUT);
	pinMode(pinPOsitionFind, OUTPUT);
	digitalWrite(pinGas, LOW);
	//digitalWrite(pinGas,HIGH);
	//sendData("AT+IPR=9600", 5000, DEBUG);
	Serial1.begin(9600);
	//delay(2000);
	Serial.println("A7 Power ON!");

	sendData("AT+GPS=0", 3000, DEBUG);     //Close GPS
										   //sendData("AT+AGPS=1", 3000, DEBUG);     //Close GPS
	for (int i = 0; i < 2; i++) {                //Make sure the GPS has been turned on
		sendData("AT+GPSRD=1", 1000, DEBUG);
		Serial1.println("AT+GPS=1");
	}
	Serial.println("*********************************************************");
	Serial.println("**If don`t display 'GPS positioning....',please reboot.**");
	Serial.println("*********************************************************");

}

// the loop function runs over and over again until power down or reset
void loop() {
	sendData("AT+GPSRD=1", 1000, DEBUG);
	Serial1.println("AT+GPS=1");
	if (gpsFlag) {
		digitalWrite(pinPOsitionFind, HIGH);
	}else{
		digitalWrite(pinPOsitionFind, LOW);

	}
	getMesage();
}
void getMesage() {
	sendData("AT+CMGF=1\r\n", 2000, DEBUG);
	sendData("AT+CSQ\r\n", 10000, DEBUG);
	sendData("AT+GSN\r\n", 10000, DEBUG);
	if (gprsFlag && gpsFlag) {
		TCP_GPRS(postion[0], postion[1]);
	}
}
void sendData(String command, const int timeout, boolean debug)
{
	String response = "";
	String info = "";
	String val2 = "";
	byte cnt = 0;

	Serial1.println(command);
	long int time = millis();
	while ((time + timeout) > millis()) {
		while (Serial1.available()) {
			response = Serial1.readStringUntil('\n');
			if (response.startsWith("+GPSRD")) {
				configGPSData(response);
			}
			if (cnt == 1) {
				configCMTSMS(info, response);
				cnt = 0;
			}
			if (response.startsWith("+CMT")) {
				info = response;
				cnt++;
			}

			if (debug) {
				Serial.println(response);

			}


		}
	}

}
void configGPSData(String gprd) {
	String lo = getRealiablePosition(getSpliteValue(gprd, ',', 2), 2);
	String la = getRealiablePosition(getSpliteValue(gprd, ',', 4), 3);
	Serial.println("position=" + lo + "," + la);
	if (gpsFlag) {
		postion[0] = lo;
		postion[1] = la;
		gpsTime = getSpliteValue(gprd, ',', 1);
	}


}
void configCMTSMS(String info, String message) {
	Serial.println("info" + info);
	Serial.println("message" + message);
	Serial.println("number" + getPhoneNumberFomCMT(info) + "posiotion" + postion[0] + "," + postion[1]);
	message.toLowerCase();

	if (message.indexOf("position") != -1) {
		if (gpsFlag)
			SendTextMessage(postion[0] + "," + postion[1], getPhoneNumberFomCMT(info));
		Serial.println("number" + getPhoneNumberFomCMT(info) + "posiotion" + postion[0] + "," + postion[1]);

	}
	if (message.indexOf("gprson") != -1) {
		Serial.println("gprs oNNNNNNNNNNNNNNNNNNN");
		gprsFlag = true;

	}
	if (message.indexOf("gprsoff") != -1) {
		Serial.println("gprs oFFFFFFFFFFFFFFFF");

		gprsFlag = false;

	}
	if (message.indexOf("lockon") != -1 && message.indexOf("1372")!=-1) {
			digitalWrite(pinGas, HIGH);
	}
	if (message.indexOf("lockoff") != -1) {
		if (message.indexOf("1372") != -1) {
			digitalWrite(pinGas, LOW);
		}
	}
	/*if (message.indexOf("gprson") != -1) {
		stateGPSRWorking = true;
		Serial.println("gptssssssssssONNNNN");

		testgps();
	}
	if (message.indexOf("gprsoff") != -1) {
		Serial.println("gptssssssssssOFFFFFF");

		stateGPSRWorking = false;
		testgps();
	}
	if (message.indexOf("lockon") != -1) {
		if (message.indexOf("1372") != -1) {
			Serial.println("LOCKONNNNN");

			digitalWrite(pinGas, HIGH);
		}

	}
	if (message.indexOf("lockoff") != -1) {
		if (message.indexOf("1372") != -1) {
			Serial.println("LOCKOFFFF");

			digitalWrite(pinGas, LOW);
		}

	}*/
}
String getPhoneNumberFomCMT(String info) {
	String s = getSpliteValue(info, ',', 0);
	//target_phone.remove(0, 6);
	s.remove(0, 7);
	s.remove(s.length() - 1, 1);
	return s;
}
String getRealiablePosition(String longtute, byte substring) {
	String firstPartLang = longtute.substring(0, substring);
	longtute.remove(0, substring);
	double s = longtute.toFloat();
	s = s / 60;
	String val = String(s, 6);
	Serial.println("oooo" + val);
	if (s > 0) {
		gpsFlag = true;

	}
	else
	{
		gpsFlag = false;
	}
	if (val.charAt(0) == '0')
		val.remove(0, 1);
	longtute = firstPartLang + (val);
	if (longtute.charAt(0) == '0')
		longtute.remove(0, 1);
	Serial.println("tttt" + longtute);
	return longtute;
}
String getSpliteValue(String data, char separator, int index)
{
	int found = 0;
	int strIndex[] = { 0, -1 };
	int maxIndex = data.length() - 1;

	for (int i = 0; i <= maxIndex && found <= index; i++) {
		if (data.charAt(i) == separator || i == maxIndex) {
			found++;
			strIndex[0] = strIndex[1] + 1;
			strIndex[1] = (i == maxIndex) ? i + 1 : i;
		}
	}

	return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void SendTextMessage(String message, String number)
{
	sendData("AT+CMGF=1", 5000, DEBUG);            //Set the SMS in text mode
	delay(500);
	sendData("AT+CMGS=" + number, 2000, DEBUG);//send sms message to the cellphone , be careful need to add a country code before the cellphone number
	delay(100);
	sendData(message, 2000, DEBUG);                //the content of the message
	delay(200);
	Serial1.println((char)26);                  //the ASCII code of the ctrl+z is 26
	delay(100);
	sendData("", 1000, DEBUG);                     //Clear serial data
	delay(100);

	Serial.println("MESSASgesentgggg");
	Serial.println("yyyyyyyyyyyyyyyyyyy");
	delay(2000);
}
void TCP_GPRS(String lo, String la) {
	//sendData("AT+GPS=0", 3000, DEBUG);
	delay(2000);
	Serial.println("tcp sendddddd" + lo + "/" + la);
	sendData("AT+CREG?\r\n", 3000, DEBUG);
	sendData("AT+CGATT=1\r\n", 1000, DEBUG);
	sendData("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", 1000, DEBUG);
	sendData("AT+CGACT=1,1\r\n", 1000, DEBUG);
	sendData("AT+CIPSTART=\"TCP\",\"todozi.ir\",80\r\n", 3000, DEBUG);
	sendData("AT+CIFSR\r\n", 5000, DEBUG);
	sendData("AT+CIPSEND=200\r\n", 1000, DEBUG);
	sendData("GET http://todozi.ir/api/gps/GetGpsData?time="+gpsTime+"&l=" + lo + "&a=" + la +"&imei="+gpsImei+"\r\n", 100, DEBUG);
	delay(7000);
}
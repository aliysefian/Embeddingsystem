/*
* Created by Keen
* Modified by Keen
* Date: 15/07/2017
* Please note:
* 1.You need to change the "target_phone".
* 2.Lithium batteries must be used.
* 3.The GPS and GSM antenna must be used.
* Please open your "Serial Monitor" and you will get the debug information.
*/

#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial Serial1(8, 9);
char byteGPS = -1;
char linea[300] = "";
char comandoGPR[7] = "$GPRMC";
int cont = 0;
int bien = 0;
int conta = 0;
int indices[13];

int GPS_time = 30;                        // When the board gets the 30 times location information successfully and the location information will be send by sms.      
String target_phone = "+989137266252"; // Your phone number,be careful need to add a country code before the cellphone number

String GPS_position = "";
int GPS_position_count = 0;
int pinGas = A0;
byte stateWorking = 0; //1 for sms,2gprs on ,3gprs off,4lock0n ,lock off
bool stateGPSRWorking = false; //1 for sms,2gprs on ,3gprs off,4lock0n ,lock off
void setup()
{
	Serial.begin(115200);
	Serial1.begin(115200);
	/*pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(8, OUTPUT);
	digitalWrite(5, HIGH);
	digitalWrite(4, LOW);
	digitalWrite(8, LOW);
	delay(2000);
	digitalWrite(8, HIGH);
	delay(3000);
	digitalWrite(8, LOW);*/
	for (int i = 0; i < 2; i++) {
		sendData("AT+IPR=9600", 5000, DEBUG);
	}
	pinMode(pinGas, OUTPUT);
	//digitalWrite(pinGas,HIGH);
	sendData("AT+IPR=9600", 5000, DEBUG);
	Serial1.begin(9600);
	delay(2000);
	Serial.println("A7 Power ON!");
	//	sendData("AT+GPS=0", 3000, DEBUG);     //Close GPS
	sendData("AT+GPS=0", 3000, DEBUG);     //Close GPS
	//sendData("AT+AGPS=1", 3000, DEBUG);     //Close GPS
	for (int i = 0; i < 2; i++) {                //Make sure the GPS has been turned on
		sendData("AT+GPSRD=1", 1000, DEBUG);
		Serial1.println("AT+GPS=1");
	}
	//sendData("AT+GPS=1", 1000, DEBUG);
	//sendData("AT+GPSRD=1", 1000, DEBUG);
	Serial.println("*********************************************************");
	Serial.println("**If don`t display 'GPS positioning....',please reboot.**");
	Serial.println("*********************************************************");
}
String val = "";
long cnt = 0;
void loop()
{
	//	String s = "+CMT: \"+989137266252\"";
		//s.remove(0, 7);
		//s.remove(s.length()-1, 1);

		//Serial.println(s);
		//target_phone = s;
		//call();
		//cnt++;
	testgps();

	//	delay(5000);
	//TCP_GPRS("31.988809", "51.988809");
	//delay(5000);
	//call();
}

void testgps() {
	String getPosStrig = "";
	while (Serial1.available()) {
		val = Serial1.readStringUntil('\n');
		if (val.startsWith("+GPSRD")) {
			getPosStrig = val;
			/*Serial.println("0rrrrrrrr"+getSpliteValue(val, ',', 0));
			Serial.println("1rrrrrrrr"+getSpliteValue(val, ',', 1));
			Serial.println("2rrrrrrrr"+getSpliteValue(val, ',', 2));
			Serial.println("3rrrrrrrr"+getSpliteValue(val, ',', 3));
			Serial.println("4rrrrrrrr"+getSpliteValue(val, ',', 4));
			Serial.println("5rrrrrrrr"+getSpliteValue(val, ',', 5));
			Serial.println("6rrrrrrrr"+getSpliteValue(val, ',', 6));*/
			String longtute = getSpliteValue(val, ',', 2);

			String firstPartLang = longtute.substring(0, 2);
			longtute.remove(0, 2);
			double s = longtute.toFloat();
			s = s / 60;
			String val = String(s, 6);
			Serial.println("oooo" + val);

			//if (val.charAt(0) == '0')
			val.remove(0, 1);
			longtute = firstPartLang + (val);
			String lo = longtute;
			Serial.println("tttt" + longtute);

			longtute = getSpliteValue(getPosStrig, ',', 4);
			Serial.println("uuuuuuu" + longtute);
			firstPartLang = longtute.substring(0, 3);
			longtute.remove(0, 3);
			s = longtute.toFloat();
			s = s / 60;
			val = String(s, 6);
			Serial.println("oooo" + val);

			//if (val.charAt(0) == '0')
			val.remove(0, 1);
			longtute = firstPartLang + (val);
			Serial.println("wwww" + longtute);
			Serial.println(lo + "," + longtute);
			if (longtute.charAt(0) == '0')
				longtute.remove(0, 1);
			if (s > 0) {

				//	sendData("AT+GPS=0", 3000, DEBUG);     //Close GPS
				if (stateWorking == 1) {
					SendTextMessage(lo + "," + longtute);
					//call();
					delay(4000);


				}
				if (stateGPSRWorking) {
					TCP_GPRS(lo, longtute);
					delay(4000);

				}
				getMesage();
				/*switch (stateWorking)
				{
				case 1:

					break;
				case 2:

					break;
				default:
					break;
				}*/

				//	for (int i = 0; i<2; i++) {                //Make sure the GPS has been turned on
				//		sendData("AT+GPSRD=1", 1000, DEBUG);
				//		Serial1.println("AT+GPS=1");
				//	}
			}
			//SendTextMessage(lo + "," + longtute);

		}
		Serial.println(val);

		// Read a byte of the serial port

	}
}
void call() {
	Serial.println("uuuuuuuuuuuuuuuuuuuuuuu=" + target_phone);
	if (cnt == 0)
		sendData("ATD" + target_phone + "\r\n", 15000, DEBUG);            //Set the SMS in text mode
	//delay(1000);
	cnt++;

}

void SendTextMessage(String message)
{
	sendData("AT+CMGF=1", 5000, DEBUG);            //Set the SMS in text mode
	delay(500);
	sendData("AT+CMGS=" + target_phone, 2000, DEBUG);//send sms message to the cellphone , be careful need to add a country code before the cellphone number
	delay(100);
	sendData(message, 2000, DEBUG);                //the content of the message
	delay(200);
	Serial1.println((char)26);                  //the ASCII code of the ctrl+z is 26
	delay(100);
	sendData("", 1000, DEBUG);                     //Clear serial data
	delay(100);
	stateWorking = 0;
	Serial.println("MESSASgesentgggg");
	Serial.println("yyyyyyyyyyyyyyyyyyy");
	delay(2000);
}
void getMesage() {
	//sendData("AT+CMGF=0\r\n", 2000, DEBUG);
	//delay(100);
	sendData("AT+CMGF=1\r\n", 2000, DEBUG);
	//delay(100);

	sendData("AT+CSDH=1\r\n", 2000, DEBUG);
	//delay(100);

	//sendData("AT+CMGL= \"REC UNREAD\"\r\n", 5000, DEBUG);
	//sendData("AT+CMGL= \"REC READ\"\r\n", 5000, DEBUG);
	sendData("AT+CMGL=\"ALL\"\r\n", 10000, DEBUG);
	//delay(5000);

	sendData("AT+CSQ\r\n", 10000, DEBUG);
	//	delay(1000);
}

void sendData(String command, const int timeout, boolean debug)
{
	String response = "";
	String val = "";
	String val2 = "";
	byte cnt = 0;
	Serial1.println(command);
	long int time = millis();
	while ((time + timeout) > millis()) {
		while (Serial1.available()) {
			response = Serial1.readStringUntil('\n');
			if (debug) {
				Serial.println(response);
			}
			if (cnt == 1)
			{
				val2 = response;
				cnt = 0;
			}
			if (response.startsWith("+CMT")) {
				val = response;
				cnt++;
			}


		}
		if (val != "") {
			Serial.println("message" + val);
			Serial.println("messagewwww" + val2);

		}
	}
	if (val != "")
		stateSMSChooser(val, val2);

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
void TCP_GPRS(String lo, String la) {
	//sendData("AT+GPS=0", 3000, DEBUG);
	delay(2000);
	Serial.println(lo + "/" + la);

	/*sendData("AT+CMGF=1", 5000, DEBUG);
	sendData("AT+CGATT=1", 5000, DEBUG);
	sendData("AT+CGDCONT=1,\"IP\",\"mtnirancell\" ", 5000, DEBUG);
	sendData("AT+CSTT=\"mtnirancell\",\"\",\"\"", 5000, DEBUG);
	sendData("AT+CIICR", 5000, DEBUG);
	sendData("AT+CIFSR", 5000, DEBUG);
	sendData("AT+CIPHEAD=1", 5000, DEBUG);
	sendData("AT+CIPSTART=\"TCP\",\"microdroidprj.ir\",80", 5000, DEBUG);
	sendData("AT+CIPSEND", 5000, DEBUG);
	sendData("GET http://microdroidprj.ir/my_sct/sct.php?id=1", 5000, DEBUG);
*/
//sendData("AT+CREG?", 5000, DEBUG); //Query network registration
//delay(100);
//sendData("AT+CGATT=1", 5000, DEBUG);
//delay(100);
//sendData("AT+CGDCONT=1,\"IP\",\"CMNET\"", 2000, DEBUG);//setting PDP parameter 
//delay(100);
//sendData("AT+CGACT=1,1", 10000, DEBUG); //Activate PDP, open Internet service
//delay(100);
//sendData("AT+CIPSTART=\"TCP\",\"minachoob.ir?dell=168\",80", 10000, DEBUG);
//delay(100);
//sendData("AT+CIPSEND=5,\"12345\"", 2000, DEBUG); //Send string "12345" 
//delay(100);
//sendData("AT+CIPCLOSE", 2000, DEBUG);     //Close TCP
//delay(100);
/*
sendData("AT+CREG?",3000,DEBUG);
sendData("AT+CGATT=1",1000,DEBUG);
sendData("AT+CGDCONT=1,\"IP\",\"CMNET\"",1000,DEBUG);
sendData("AT+CGACT=1,1",1000,DEBUG);
sendData("AT+CIPSTART=\"TCP\",\"google.com\",80",3000,DEBUG);
sendData("AT+CIFSR", 5000, DEBUG);
sendData("AT+CIPSEND=80",1000,DEBUG);
sendData("GET http://google.com HTTP/1.1\r\n",100,DEBUG);*/
	sendData("AT+CREG?\r\n", 3000, DEBUG);
	sendData("AT+CGATT=1\r\n", 1000, DEBUG);
	sendData("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", 1000, DEBUG);
	sendData("AT+CGACT=1,1\r\n", 1000, DEBUG);
	sendData("AT+CIPSTART=\"TCP\",\"todozi.ir\",80\r\n", 3000, DEBUG);
	sendData("AT+CIFSR\r\n", 5000, DEBUG);
	sendData("AT+CIPSEND=200\r\n", 1000, DEBUG);
	sendData("GET http://todozi.ir/api/gps/GetGpsData?time=55&l=" + lo + "&a=" + la + "\r\n", 100, DEBUG);

	//sendData("AT+GPS=1", 1000, DEBUG);
	//sendData("AT+GPSRD=1", 1000, DEBUG);
	delay(7000);
}
void stateSMSChooser(String info, String message) {
	Serial.println("infooooooo" + info);
	String s = getSpliteValue(info, ',', 0);
	//target_phone.remove(0, 6);
	s.remove(0, 7);
	s.remove(s.length() - 1, 1);


	Serial.println("targetphone=" + target_phone);
	if (s != "")
		target_phone = s;
	if (message.indexOf("loc") != -1) {
		stateWorking = 1;
		Serial.println("loccccccccccccccc");
		testgps();
	}
	if (message.indexOf("gprson") != -1) {
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

	}

}
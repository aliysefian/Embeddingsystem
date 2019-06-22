/*
14CORE NRF24L01 SLAVE/RECIEVER
*/

#include  <SPI.h> //Include SPI Code Library which can be downloaded below
#include "nRF24L01.h" //Include NRF24L01 Code Library which can be downloaded below
#include "RF24.h" //Inlcude NRF24 Code Library which can be downloaded below

long msg[1];
RF24 radio(9, 10); // NRF24L01 Pin 
const uint64_t pipe = 0xE8E8F0F0E1LL; //Start Pipe Communication Address
const uint64_t pipeTransfer = 0xA8E8F0F0E1LL; //Start Pipe Communication Address
int Indicator = 3; //This the LED which is connected to Arduino Pin 3
int trans = 6;
int pin0 = A0;
int pin1 = A1;
int pin2 = A2;
int pin3 = A3;
int pin4 = A4;
int pin5 = A5;
int pin6 = 2;
int pin7 = 3;
int pinTransFail = 7;
int pinLoop = 4;

bool flag = true;
void setup(void) {
	Serial.begin(9600);
	radio.begin();
	//radio.openWritingPipe(pipeTransfer);
	radio.openReadingPipe(1, pipe);
	radio.startListening();
	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_250KBPS);
	// Here we are sending 1-byte payloads to test the call-response speed
	pinMode(Indicator, OUTPUT);

	pinMode(pin0, OUTPUT);
	pinMode(pin1, OUTPUT);
	pinMode(pin2, OUTPUT);
	pinMode(pin3, OUTPUT);
	pinMode(pin4, OUTPUT);
	pinMode(pin5, OUTPUT);
	pinMode(pin6, OUTPUT);
	pinMode(pin7, OUTPUT);
	pinMode(pinLoop, OUTPUT);
	pinMode(pinTransFail, OUTPUT);
}
int counter = 0;
unsigned long startTime = 0;
unsigned long startTimeForOff = 0;//ÎÇãæÔ ˜ääÏå 
unsigned long totalFalt = 0;//ÎÇãæÔ ˜ääÏå 
unsigned long listCounter[8] = { 0 };
unsigned long periodConter = 1000;

void loop() {

	if (flag) {
		digitalWrite(pinLoop, HIGH);
	}
	else
	{
		digitalWrite(pinLoop, LOW);

	}
	flag = !flag;
	///delay(1000);
	if (radio.available()) {
		digitalWrite(trans, HIGH);
	//	Serial.println("ok:  Radio Transmission");
		bool done = false;
		startTime = 0;
		digitalWrite(pinTransFail, LOW);
		totalFalt = 0;
		while (true) {

			startTime = 0;
			radio.read(msg, 3);
			//Serial.println(msg[0]);
			String val = String(msg[0]);
			Serial.println("askjhdajkdhalkdklah"+val);
			
			if (val.indexOf('1') != -1) {
				digitalWrite(pin0, HIGH);
				Serial.println("kkkkkk");
				listCounter[0] = 0;

			}
			else
			{
				if (listCounter[0] == 0)
					listCounter[0] = millis();
					Serial.println("no 1");

				if (CheckPeriod(listCounter[0], periodConter))
					digitalWrite(pin0, LOW);

			} if (val.indexOf('2') != -1) {
				listCounter[1] = 0;
				digitalWrite(pin1, HIGH);

			}
			else
			{
				if (listCounter[1] == 0)
					listCounter[1] = millis();
				//	Serial.println(listCounter[0]);

				if (CheckPeriod(listCounter[1], periodConter))
					digitalWrite(pin1, LOW);
			}
			if (val.indexOf('3') != -1) {
				listCounter[2] = 0;
				digitalWrite(pin2, HIGH);

			}
			else
			{
				if (listCounter[2] == 0)
					listCounter[2] = millis();
				//	Serial.println(listCounter[0]);

				if (CheckPeriod(listCounter[2], periodConter))
					digitalWrite(pin2, LOW);
			}

			if (val.indexOf('4') != -1) {
				listCounter[3] = 0;
				digitalWrite(pin3, HIGH);

			}
			else
			{
				if (listCounter[3] == 0)
					listCounter[3] = millis();
				//	Serial.println(listCounter[0]);

				if (CheckPeriod(listCounter[3], periodConter))
					digitalWrite(pin3, LOW);
			} if (val.indexOf('5') != -1) {
				listCounter[4] = 0;
				digitalWrite(pin4, HIGH);

			}
			else
			{
				if (listCounter[4] == 0)
					listCounter[4] = millis();
				//	Serial.println(listCounter[0]);

				if (CheckPeriod(listCounter[4], periodConter))
					digitalWrite(pin4, LOW);
			} if (val.indexOf('6') != -1) {
				listCounter[5] = 0;
				digitalWrite(pin5, HIGH);

			}
			else
			{
				if (listCounter[5] == 0)
					listCounter[5] = millis();
				//	Serial.println(listCounter[0]);

				if (CheckPeriod(listCounter[5], periodConter))
					digitalWrite(pin5, LOW);
			}if (val.indexOf('7') != -1) {
				listCounter[6] = 0;
				digitalWrite(pin6, HIGH);

			}
			else
			{
				if (listCounter[6] == 0)
					listCounter[6] = millis();
				//	Serial.println(listCounter[0]);

				if (CheckPeriod(listCounter[6], periodConter))
					digitalWrite(pin6, LOW);
			}if (val.indexOf('8') != -1) {
				listCounter[7] = 0;
				digitalWrite(pin7, HIGH);

			}
			else
			{
				if (listCounter[7] == 0)
					listCounter[7] = millis();
				//	Serial.println(listCounter[0]);

				if (CheckPeriod(listCounter[7], periodConter))
					digitalWrite(pin7, LOW);
			}
			if (msg[0] == 0 )
				break;
			/* if (msg[0] == 0 || msg[0] == 221) {
				if (startTime == 0)
					startTime = millis();
				else if (CheckPeriod(startTime, 10000)) {
					Serial.println("offfffff");
					startTime = 0;
					break;
				}
			}
			else if (msg[0] == 222 || msg[0] == 221) {
				digitalWrite(Indicator, LOW);
				digitalWrite(trans, HIGH); startTime = 0;
				delay(100);
			}*/
			//Serial.println("cnt is:" + String(counter));
			//Serial.println("time is :" + String(startTime));

		}
		//	delay(900);

	}
	else {
		if (totalFalt == 0)
			totalFalt = millis();
		if (CheckPeriod(totalFalt, 4000)) {
			Serial.println("Error: No Radio Transmission   no iooooooo");
			/*digitalWrite(Indicator, LOW);
			digitalWrite(trans, LOW);*/
			digitalWrite(pinTransFail, HIGH);
			digitalWrite(pin0, LOW);
			digitalWrite(pin1, LOW);
			digitalWrite(pin2, LOW);
			digitalWrite(pin3, LOW);
			digitalWrite(pin4, LOW);
			digitalWrite(pin5, LOW);
			digitalWrite(pin6, LOW);
			digitalWrite(pin7, LOW);

		}
		
		//delay(500);

	}
}
bool CheckPeriod(unsigned long time, unsigned long period) {


	float res = (millis() - time);
	//Serial.println("time is :" + String(res));
	if (time == 0) {
		return false;
	}
	if (millis() - time > period) {
		return true;
	}
	return false;
}
void SendMessageCheck() {
	msg[0] = 200; //Send the 111 to the reciever 
	radio.write(msg, 1);
	//return true;
	Serial.println("is :200");

}


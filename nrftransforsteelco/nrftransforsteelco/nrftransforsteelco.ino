#include "nRF24L01.h" //Include NRF24L01 Code Library which can be downloaded below
#include "RF24.h" //Inlcude NRF24 Code Library which can be downloaded below
long msg[1];
RF24 radio(9, 10); // The NRF24L01 Pin CE and Pin CSN
const uint64_t pipe = 0xE8E8F0F0E1LL; //Communication Pip Address
const uint64_t pipeTransfer = 0xA8E8F0F0E1LL; //Start Pipe Communication Address
int pin0 = A0;
int pin1 = A1;
int pin2 = A2;
int pin3 = A3;
int pin4 = A4;
int pin5 = A5;
int pin6 = 1;
int pin7 = 0;
int pinLoop = 4;
int pinTest = 6;
bool flag = true;
bool flagPinTest = false;
unsigned long time;
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600); //Start Serial Communication at baud rate 9600
						//EPROMClearfunc();
						//

	radio.begin();
	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_250KBPS);
	radio.openWritingPipe(pipe);
	//radio.openReadingPipe(1, pipeTransfer);
	pinMode(pinLoop, OUTPUT);
	//pinMode(pin0, INPUT);
	pinMode(pin0, INPUT);
	pinMode(pin1, INPUT);
	pinMode(pin2, INPUT);
	pinMode(pin3, INPUT);
	pinMode(pin4, INPUT);
	pinMode(pin5, INPUT);
	pinMode(pin6, INPUT);
	pinMode(pin7, INPUT);
	pinMode(pinTest, INPUT);
	Serial.println("Ready");


}
int initMode;
// the loop function runs over and over again until power down or reset
void loop() {
	if (flag) {
		digitalWrite(pinLoop, HIGH);
	}
	else
	{
		digitalWrite(pinLoop, LOW);

	}
	flag = !flag;
	String value = "";
	//msg[0] = 121; //Send the 111 to the reciever 
	//radio.write(msg, 1);
	if (digitalRead(pin0) == LOW) {
		//msg[0] = 111; //Send the 111 to the reciever 
		//radio.write(msg, 1);
		value += '1';
		//Serial.println("one is ");
		//delay(50);


	}
	if (digitalRead(pin1) == LOW) {
		//Serial.println("two is ");
		value += '2';

		//msg[0] = 112; //Send the 111 to the reciever 
		//radio.write(msg, 1);
	//	delay(40);
	}

	if (digitalRead(pin2) == LOW) {
		value += '3';
	}if (digitalRead(pin3) == LOW) {
		value += '4';
		//delay(5);
	}if (digitalRead(pin4) == LOW) {
		value += '5';
		//delay(5);
	}if (digitalRead(pin5) == LOW) {
		value += '6';
		//delay(5);
	}if (digitalRead(pin6) == LOW) {
		value += '7';
	}if (digitalRead(pin7) == LOW) {
		value += '8';
	}

	//int finalMode = digitalRead(pinTest);

	//if (finalMode == LOW) {
	//	//flagPinTest = !flagPinTest;
	//	time = 0;
	//	Serial.println("click");
	//	//EndFunc();
	//	if (time == 0) {
	//		time = millis();
	//		value += '9';
	//	}
	//	if (!CheckPeriod(time, 3000)) {
	//		//time = 0;
	//		value += '9';
	//	}if (CheckPeriod(time, 6000)) {
	//		time = 0;
	//	}
	//}
	//	initMode = finalMode;

	//}
	//if (flagPinTest) {
	//	if (time == 0) {
	//		time = millis();
	//		value += '9';
	//	}
	//	if (CheckPeriod(time, 3000)) {
	//		//time = 0;
	//		value += ' ';
	//	}if (CheckPeriod(time, 6000)) {
	//		time = 0;
	//	}

	//}
	//Serial.println(value);
	msg[0] = value.toInt();
	Serial.println(msg[0]);
	radio.write(msg, 4);
	//	delay(100);

}
bool CheckPeriod(unsigned long time, unsigned long period) {


	float res = (millis() - time);
	Serial.println("time is :" + String(res));
	if (time == 0) {
		return false;
	}
	if (millis() - time > period) {
		return true;
	}
	return false;
}

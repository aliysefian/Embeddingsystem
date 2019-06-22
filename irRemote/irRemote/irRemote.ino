#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

/*
* IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
* An IR detector/demodulator must be connected to the input RECV_PIN.
* Version 0.1 July, 2009
* Copyright 2009 Ken Shirriff
* http://arcfn.com
*/

#include <IRremote.h>

int RECV_PIN = 11;
byte check2;
IRrecv irrecv(RECV_PIN);
double timess = 0;
decode_results results;
int triggerWarning = 9;
int delaypin3 = 8;
int delaypin2 = 7;
int delaypin1 = 6;
int modepin3 = A5;
int modepin2 = A4;
int modepin1 = A3;
byte seeFlag = 0;
byte countToFire = 4;
unsigned long timeIgnore = 0;
void setup()
{
	Serial.begin(9600);
	irrecv.enableIRIn(); // Start the receiver
	pinMode(triggerWarning, OUTPUT);
	pinMode(delaypin1, INPUT);
	pinMode(delaypin2, INPUT);
	pinMode(delaypin3, INPUT);
	//modefireFunc();
}

void loop() {
	modefireFunc();
	if (irrecv.decode(&results)) {
		Serial.println(results.value, HEX);
		Serial.println(String(results.value, HEX));
		String re = String(results.value, HEX);
		timess = 0;
		if (re == "a90" && !CheckPriodForIgnore())
		{
			//Serial.println("iseeeeeeeee");
			digitalWrite(triggerWarning, HIGH);

		}
		else
		{
			Serial.println("WRONG CODE");
			WrongCodeForIgnore();

			if (CheckPriodForIgnore()) {
				digitalWrite(triggerWarning, LOW);
				delay(delayReadFunc());
			}


		}
		irrecv.resume(); // Receive the next value
	}
	else
	{
		if (timess == 0) {
			timess = millis();
			//Serial.println("nisssssssssssssssss");
		}

		if (timess != 0 && (millis() - timess) >= 2000) {
			digitalWrite(triggerWarning, LOW);
			Serial.println("nisssssssssssssssss");
			timess = 0;
		}
			//WrongCodeForIgnore();
		//Serial.println("nisssssssssssssssss");
		//if (CheckPriodForIgnore()) {
			//delay(delayReadFunc());

		//}



	}

	//delay(40);
	delayReadFunc();
	  delay(100);
	Serial.println("asdsajdlhaskjd:" + String(seeFlag));

}
int  delayReadFunc() {
	int pin1 = digitalRead(delaypin1);
	int pin2 = digitalRead(delaypin2);
	int pin3 = digitalRead(delaypin3);

	//String read = String(pin1) + String(pin2) + String(pin3);
	////Serial.print(String(read, BIN));
	int response = pin1 * 4 + pin2 * 2 + pin3;
	//Serial.println(response);
	return response * 1000;
}
void  modefireFunc() {
	int pin1 = digitalRead(modepin1);
	int pin2 = digitalRead(modepin2);
	int pin3 = digitalRead(modepin3);
	int response = pin1 * 4 + pin2 * 2 + pin3;
	Serial.println("dddd:" + String(response));
	Serial.println("dddd:" + String(countToFire));
	switch (response)
	{
	case 0:
		countToFire = 1; break;
	case 1:
		countToFire = 3; break;
	case 2:
		countToFire = 5; break;
	case 3:
		countToFire = 7; break;
	case 4:
		countToFire = 9; break;
	case 5:
		countToFire = 11; break;
	case 6:
		countToFire = 15; break;
	case 7:
		countToFire = 18;
	default:
		break;
	}


}
bool CheckPriodForIgnore() {


	int period = 20000;
	unsigned long mi = millis() - timeIgnore;

	if ((mi >= period  &&  seeFlag <= countToFire) || timeIgnore == 0)
	{
		seeFlag = 0;
		timeIgnore = 0;
	}
	Serial.println(String(seeFlag));
	if (seeFlag >= countToFire && timeIgnore != 0) {
		Serial.println("dozd");
		Serial.println(String(seeFlag));
		seeFlag = 0;
		timeIgnore = 0;
		return true;

	}
	return false;
}
void WrongCodeForIgnore() {
	seeFlag++;
	//delay(100);
	if (timeIgnore == 0)
		timeIgnore = millis();
}

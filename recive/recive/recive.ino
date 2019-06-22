/*
14CORE NRF24L01 SLAVE/RECIEVER
*/

#include  <SPI.h> //Include SPI Code Library which can be downloaded below
#include "nRF24L01.h" //Include NRF24L01 Code Library which can be downloaded below
#include "RF24.h" //Inlcude NRF24 Code Library which can be downloaded below

int msg[1];
RF24 radio(9, 10); // NRF24L01 Pin 
const uint64_t pipe = 0xE8E8F0F0E1LL; //Start Pipe Communication Address
const uint64_t pipeTransfer = 0xA8E8F0F0E1LL; //Start Pipe Communication Address
int Indicator = 3; //This the LED which is connected to Arduino Pin 3
int trans = 2;
void setup(void) {
	//Serial.begin(9600);
	Serial.end();
	radio.begin();
	radio.openWritingPipe(pipeTransfer);
	radio.openReadingPipe(1, pipe);
	radio.startListening();
	              // Here we are sending 1-byte payloads to test the call-response speed
	pinMode(Indicator, OUTPUT);
	pinMode(trans, OUTPUT);
}
int counter = 0;
unsigned long startTime = 0;
unsigned long startTimeForOff = 0;//Œ«„Ê‘ ò‰‰œÂ 
void loop() {
	delay(5);
	radio.stopListening();
	int cnt = 0;
	SendMessageCheck();
	delay(5);
	radio.startListening();
	/*radio.stopListening();
	int cnt = 0;
	SendMessageCheck();
	delay(5);

	radio.startListening();*/
	//radio.stopListening();
	//msg[0] = 200;
	//radio.writeAckPayload(pipeTransfer, msg, 1);

	/*while (cnt < 20) {
		cnt++;
		SendMessageCheck();
	}
	radio.startListening();
*/
	if (radio.available()) {
		digitalWrite(trans, HIGH);
		Serial.println("ok:  Radio Transmission");
		bool done = false;
		startTime = 0;
		while (true) {
			

			radio.read(msg, 1);
			Serial.println(msg[0]);
			if (msg[0] == 240) {
				startTime = 0;
			}
			else if (msg[0] == 111) { startTime = 0; digitalWrite(Indicator, HIGH); delay(500); }
			else if (msg[0] == 0 || msg[0] == 221) {
				if (startTime == 0)
					startTime = millis();
				else if (CheckPeriod(startTime, 10000)) {
					Serial.println("offfffff");
					startTime = 0;
					break;
				}
			}
			else if (msg[0]==222 || msg[0]==221) {
				digitalWrite(Indicator, LOW);
				digitalWrite(trans, HIGH); startTime = 0;
				delay(100);
			}
			//Serial.println("cnt is:" + String(counter));
			//Serial.println("time is :" + String(startTime));

		}
		
	}
	else  {

		Serial.println("Error: No Radio Transmission");
		digitalWrite(Indicator, LOW);
		digitalWrite(trans, LOW);
		delay(500);
		
	}
}
bool CheckPeriod(unsigned long time, unsigned long period) {
	delay(5);
	radio.stopListening();
	int cnt = 0;
	SendMessageCheck();
	delay(5);

	radio.startListening();
	float res = (millis() - time );
	Serial.println("time is :" + String(res));

	if (millis() - time >period ) {
				return true;
	}
	return false;
}
void SendMessageCheck() {
	msg[0] = 200; //Send the 111 to the reciever 
	radio.write(msg, 1);
	//return true;
	Serial.println("is :200" );

}


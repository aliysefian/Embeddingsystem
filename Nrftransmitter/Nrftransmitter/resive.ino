/*
14CORE NRF24L01 SLAVE/RECIEVER
*/

#include  <SPI.h> //Include SPI Code Library which can be downloaded below
#include "nRF24L01.h" //Include NRF24L01 Code Library which can be downloaded below
#include "RF24.h" //Inlcude NRF24 Code Library which can be downloaded below

int msg[1];
RF24 radio(9, 10); // NRF24L01 Pin 
const uint64_t pipe = 0xE8E8F0F0E1LL; //Start Pipe Communication Address
int Indicator = 3; //This the LED which is connected to Arduino Pin 3
int trans = 6;
void setup(void) {
	Serial.begin(9600);
	radio.begin();
	radio.openReadingPipe(1, pipe);
	radio.startListening();
	pinMode(Indicator, OUTPUT);
	pinMode(trans, OUTPUT);
}

void loop(void) {
	if (radio.available()) {
		digitalWrite(trans, HIGH);
		Serial.println("ok:  Radio Transmission");
		bool done = false;
		while (!done) {
			done = radio.read(msg, 1);
			Serial.println(msg[0]);
			if (msg[0] == 111) { digitalWrite(Indicator, HIGH); delay(1500); }
			else {
				digitalWrite(Indicator, LOW);
				digitalWrite(trans, HIGH);
				delay(1000);
			}
		}
	}
	else {
		Serial.println("Error: No Radio Transmission");
		digitalWrite(Indicator, LOW);
	}
}


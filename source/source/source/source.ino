/*
 Name:		source.ino
 Created:	6/2/2017 11:07:07 AM
 Author:	Ali
*/

// the setup function runs once when you press reset or power the board
int ledPin = 9;
int incPin = 10;
void setup() {
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
int initMode;
int value=0;
void loop() {
	Serial.println(String(value));

	int finalMode = digitalRead(incPin);
	if (finalMode != initMode) {
		delay(50);
		if (finalMode == LOW) {
			value += 5;
			if (value > 135) {
				value = 0;
			}

		}
		initMode = finalMode;

	}

	analogWrite(ledPin, value);
	// wait for 30 milliseconds to see the dimming effect
	delay(30);
}

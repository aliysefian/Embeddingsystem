#include <EEPROM.h>
#include <SoftwareSerial.h>

int bluetoothTx = 7;
int bluetoothRx = 8;
int addressEprom = 0;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
int blue=0;
int green=0;
int red=0;
void setup()
{
	//EPROMClearfunc();
	pinMode(3, OUTPUT); // blue pin of RGB LED
	pinMode(5, OUTPUT); // Green pin of RGB LED
	pinMode(6, OUTPUT); // Red pin of RGB LED

	digitalWrite(3, LOW);
	digitalWrite(5, LOW);
	digitalWrite(6, LOW);
	//save epp
	if (addressEprom == EEPROM.length()) {
		addressEprom = 0;

	}
	else
	{
		if (EPROMCheckIsEmptyfunc())
		{
			EEPROM.write(addressEprom, red);
			EEPROM.write(addressEprom + sizeof(int), green);
			EEPROM.write(addressEprom + (2 * sizeof(int)),blue);
			//EEPROM.write(addressEprom + (2 * sizeof(int)), lowtemp);
		}
		else
		{
			red = EEPROM.read(addressEprom);
			green = EEPROM.read(addressEprom + sizeof(int));
			blue = EEPROM.read(addressEprom + (2 * sizeof(int)));

		}
	}
	Serial.println("red is" + red);
	callColorBegin();
	//
	//Setup usb serial connection to computer
	Serial.begin(9600);

	//Setup Bluetooth serial connection to android
	bluetooth.begin(9600);
}

void loop()
{
	//red = EEPROM.read(addressEprom);
	///green = EEPROM.read(addressEprom + sizeof(int));
	//blue = EEPROM.read(addressEprom + (2 * sizeof(int)));
	Serial.println("red is" + String(EEPROM.read(addressEprom)));
	Serial.println("gree is" + String(EEPROM.read(addressEprom + sizeof(int))));
	Serial.println("b is" + String(EEPROM.read(addressEprom + (2 * sizeof(int)))));
	//Read from bluetooth and write to usb serial
	if (bluetooth.available() >= 2)
	{
		unsigned int color1 = bluetooth.read();
		unsigned int color2 = bluetooth.read();
		unsigned int color = (color2 * 256) + color1;
		Serial.println(color);

		if (color >= 1000 && color <1255) {
			blue = color;
			blue = map(blue, 1000, 1255, 0, 255);
			analogWrite(6, blue);
			Serial.println(blue);
			delay(10);

		}

		if (color >= 2000 && color <2255) {
			 green = color;
			green = map(green, 2000, 2255, 0, 255);
			analogWrite(5, green);
			Serial.println(green);
			delay(10);

		}

		if (color >= 3000 && color < 3255) {
			 red = color;
			red = map(red, 3000, 3255, 0, 255);
			analogWrite(3, red);
			Serial.println(red);
			delay(10);
		}
		saveEPROMRGB();

	}


}

void EPROMClearfunc() {
	for (int i = 0; i < EEPROM.length(); i++) {
		EEPROM.write(i, 0);
	}
}
void saveEPROMRGB() {
	Serial.println("eeeeeeeprom");
	//addressEprom = 0;
	EEPROM.update(addressEprom, red);
	EEPROM.update(addressEprom + sizeof(int), green);
	EEPROM.update(addressEprom + (2*sizeof(int)), blue);
}
bool EPROMCheckIsEmptyfunc() {
	for (int i = 0; i < EEPROM.length(); i++) {
		if (EEPROM.read(i) != 0)
			return false;

	}
	return true;
}
void callColorBegin() {
	analogWrite(3, red);
	analogWrite(6, blue);
	analogWrite(5, green);

}
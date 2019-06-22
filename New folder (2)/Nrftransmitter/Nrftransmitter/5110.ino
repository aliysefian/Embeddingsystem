
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LCD5110_Graph.h>

//      SCK  or CLK- Pin 4 arduino
//      MOSI or DIN - Pin 5 arduino
//      DC   - Pin 10 arduino
//      RST  - Pin 8 arduino
//      CS or CE  - Pin 7 arduino
//      VCC - 3.3 volt arduino
//      BL - 5 volt arduino
//      GND- ground of arduino
LCD5110 myGLCD(4, 5, 6, 8, 7);

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
//consts
// clock var 
unsigned long s = 0;
unsigned int sec = 0;
unsigned int hrs = 0;
unsigned minutes = 0;
int initialHours = 14;//variable to initiate hours
int initialMins = 12;//variable to initiate minutes
int initialSecs = 00;//variable to initiate seconds
int lightPin = A1;
//
int hightemp = 34;
float lowtemp = 20.5;
int setStatus = 2;
int setCounter = 0;
int sta = 1;
int finalState;
int lowState;
int initLowState;
int initHighState;
int highState;
int initaitState;
int secoundInitState;
int secoundfinalState;
unsigned long settingStartTime;

int controlMenuState = 0;
const int lowPin = 4;     // the number of the pushbutton pin
const int setPin = 5;     // the number of the pushbutton pin
const int highPin = 6;     // the number of the pushbutton pin
const int controlPin = A0;     // control when turn on 
const int selectionPin = 5;     // control hotting or cooling 
								//lcd nokia 5110
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];
extern uint8_t arduino_logo[];

extern uint8_t TinyFont[];
extern uint8_t The_End[];
extern uint8_t pacman1[];
extern uint8_t pacman2[];
extern uint8_t pacman3[];
extern uint8_t pill[];
const uint8_t off[] PROGMEM = {

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0x00, 0x00, 0xFC, 0xFC,
	0x00, 0x00, 0xE0, 0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0xF8, 0xFE, 0x1F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x03, 0x07, 0x3F, 0xFE, 0xF8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x1F, 0x7F, 0xF8, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0xE0, 0xF8, 0x7F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x03, 0x03, 0x07, 0x06, 0x0E, 0x06, 0x0E, 0x0E, 0x06, 0x0E, 0x06, 0x07, 0x03, 0x03
};
const uint8_t sun[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x82, 0x24, 0x10, 0x08,
	0x04, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x04, 0x08, 0x18, 0x24, 0x82, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x00, 0x0F,
	0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x20, 0x0F,
	0x00, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x02, 0x01, 0x00, 0x00, 0x01, 0x00, 0x02, 0x02, 0x7A, 0x02, 0x02, 0x00, 0x01, 0x00,
	0x00, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x20, 0x30, 0x78,
	0x40, 0x30, 0x28, 0x20, 0x30, 0x78, 0x30, 0x30, 0x28,
};
const uint8_t snow[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x90, 0x90, 0x60, 0x7E, 0x40,
	0x81, 0x81, 0x02, 0x06, 0xFF, 0x06, 0x02, 0x81, 0x81, 0x40, 0x7E, 0x60, 0x90, 0x90, 0x88, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x48, 0x48,
	0x30, 0xF0, 0x10, 0x08, 0x08, 0x05, 0x07, 0xFF, 0x07, 0x05, 0x08, 0x08, 0x10, 0xF0, 0x30, 0x48,
	0x48, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x04, 0x02, 0x03, 0x3F, 0x03, 0x02, 0x04, 0x00, 0x00,
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
bool lightMode = false;
int finalMode;
int initMode;
int modeHitOrOff = 0;//mode hit ,cool,off
int status = 0;
float y;
uint8_t* bm;
int pacy;
int lastLowButtonState = 0;
int buttonState = 0;
int addressEprom = 0;//eeprom save state
					 //
bool flagClearScreen = true;
float periodTempChange = .5;

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
void setup() {
	//EPROMClearfunc();
	Serial.begin(9600);
	myGLCD.InitLCD();
	myGLCD.setFont(SmallFont);
	//Serial.println("Dallas Temperature IC Control Library Demo");
	pinMode(lowPin, INPUT);
	pinMode(setPin, INPUT);
	pinMode(highPin, INPUT);
	pinMode(selectionPin, INPUT);
	pinMode(controlPin, OUTPUT);

	pinMode(A1, OUTPUT);
	analogWrite(A1, 1023);
	sensors.begin();
	addressEprom = sizeof(int) + addressEprom;

	if (addressEprom == EEPROM.length()) {
		addressEprom = 0;

	}
	else
	{
		if (EPROMCheckIsEmptyfunc())
		{
			EEPROM.write(addressEprom, modeHitOrOff);
			EEPROM.write(addressEprom + sizeof(int), lowtemp);
			//EEPROM.write(addressEprom + (2 * sizeof(int)), lowtemp);
		}
		else
		{
			modeHitOrOff = EEPROM.read(addressEprom);
			lowtemp = EEPROM.read(addressEprom + sizeof(int));

		}


	}
	myGLCD.setFont(BigNumbers);
	myGLCD.print(String(1, 1), 1, 20);


}
float test = 12.0;
void loop() {// call sensors.requestTemperatures() to issue a global temperature
			 // request to all devices on the bus


			 //Serial.println("eepromlen" + String(EEPROM.length()));
			 //delay(10000);
			 //lowPinCheckFunctionNew();

	
	sensors.requestTemperatures();
	float sensorValue = sensors.getTempCByIndex(0);
	//float sensorValue = 12;
	//int se = sensors.getTempCByIndex(0);
	int valueSence = sensorValue;
	Serial.print(String(valueSence));
	if (valueSence != -127) {
		Serial.print(" Requesting temperatures...");
		//sensors.requestTemperatures(); // Send the command to get temperatures
		Serial.print("DONE");
		Serial.print("Temperature for Device 1 is: ");
		Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"? 
		Serial.print("Temperature for Device 1 is: ");
		Serial.print(sensorValue);
		//myGLCD.clrRect(38,2,84,24);


		myGLCD.setFont(SmallFont);
		//myGLCD.print("Temp", 2, 2);
		myGLCD.print("Set", 5, 2);
		myGLCD.setFont(MediumNumbers);
		//myGLCD.print(String(lowtemp), 60, 2);
		myGLCD.print(String(lowtemp, 1), 28, 2);
		myGLCD.setFont(SmallFont);
		//myGLCD.print("Mode", 55, 2);
		Serial.println("aaaaa:" + String(sensorValue, 1));
		myGLCD.setFont(BigNumbers);
		myGLCD.print(String(sensorValue, 1), 1, 20);
		//myGLCD.print(String(test,1), 1, 20);

		//myGLCD.print("DEMO", CENTER, 20);
		//myGLCD.drawRoundRect(0, 0, 62, 25);
		//myGLCD.drawRoundRect(0, 25, 63, 48);


		myGLCD.update();
		//delay(1000);
		//test = test - .2;
		controlTempPeriod(sensorValue);
		long time = millis();
		while ((millis() - time < 5000) && flagClearScreen)
		{
			if (CheckMillsecToFinishSettingTime() == true) {
				Serial.println("finis");
				analogWrite(lightPin, 1023);
				settingStartTime = millis();
			}
			Serial.println(String(lowtemp));
			controlTempPeriod(sensorValue);

			lowPinCheckFunction();

			highPinCheckFunction();
		}
		if (!flagClearScreen) {
			flagClearScreen = true;
		}
		//lowPinCheckFunctionNew();
	}
	else
	{
		//myGLCD.clrScr();
		myGLCD.setFont(SmallFont);
		//myGLCD.print("Temp", 2, 2);
		myGLCD.print("Error Sensor", 4, 20);

		digitalWrite(controlPin, LOW);
		myGLCD.update();
		delay(1000);
	}


}

void controlTempPeriod(float temper) {
	float temp = lowtemp;
	int finalMode = digitalRead(selectionPin);
	if (finalMode != initMode) {
		delay(50);
		if (finalMode == LOW) {
			modeHitOrOff++;
			saveEPROMPeriodTempfunc();
			CheckLightfunc();

		}
		initMode = finalMode;

	}
	Serial.println("mode=" + String(modeHitOrOff));
	if (modeHitOrOff == 1 || modeHitOrOff == 0) {
		myGLCD.drawBitmap(56, 20, snow, 35, 35);
		myGLCD.update();
		/*myGLCD.setFont(TinyFont);
		myGLCD.print("hit", 70, 2);
		myGLCD.update();*/
		if (temper < (temp - 0.3)) {
			Serial.println("onnnnnnnnnnnnnnnnnnnnn" + String(temp));
			digitalWrite(controlPin, HIGH);
		}
		else if (temper >= (temp + .3)) {
			//delay(5000);


			digitalWrite(controlPin, LOW);

		}
	}
	else if (modeHitOrOff == 2) {
		/*myGLCD.setFont(TinyFont);
		myGLCD.print("cool", 70, 2);
		myGLCD.update();*/
		myGLCD.drawBitmap(56, 20, sun, 35, 35);
		myGLCD.update();
		if (temper > temp + .3) {
			digitalWrite(controlPin, HIGH);
		}
		else if (temper < temp - .3) {
			//delay(5000);

			digitalWrite(controlPin, LOW);

		}
	}
	else if (modeHitOrOff == 3)
	{
		myGLCD.drawBitmap(56, 20, off, 30, 30);
		myGLCD.update();
		digitalWrite(controlPin, LOW);

	}
	else
	{
		modeHitOrOff = 0;
	}

}

void lowPinCheckFunction() {
	//int initLowState;
	//int lowState;

	lowState = digitalRead(lowPin);
	//int initLowState;

	if (lowState != initLowState) {
		delay(10);
		if (lowState == LOW) {
			Serial.println("low is active");
			CheckLightfunc();
			//lowState=digitalRead(lowPin);

			float checklow = lowtemp - periodTempChange;
			if (lowtemp - 1 >= -40) {
				lowtemp = lowtemp - periodTempChange;
				saveEPROMPeriodTempfunc();
				//ClearScreen();

			}


		}
		initLowState = lowState;

	}

}

void ClearScreen() {
	flagClearScreen = false;
	myGLCD.clrScr();

}

void setErrorFunction() {

	// lcd.setCursor(9, 0);
	//lcd.print("Error");
	//delay(3000);
	//lcd.setCursor(9, 0);
	//lcd.print("     ");


}

void highPinCheckFunction() {

	highState = digitalRead(highPin);
	if (highState != initHighState) {
		delay(10);

		CheckLightfunc();

		if (highState == LOW && lowtemp + periodTempChange<120) {
			lowtemp = lowtemp + periodTempChange;
			saveEPROMPeriodTempfunc();
			/*myGLCD.setFont(BigNumbers);
			myGLCD.print(String(lowtemp), CENTER, 20);
			myGLCD.update();*/
		}
		initHighState = highState;
	}



}

void activeLowFunction() {
	//delay(500);
	Serial.println("low");
	// lcd.setCursor(0, 1);
	// lcd.print("   ");
	// lcd.setCursor(4, 1);
	// lcd.print(lowtemp);
	//delay(500);
}

void activehighFunction() {
	//	delay(500);
	Serial.println("high");
	//lcd.setCursor(9, 1);
	//lcd.print("    ");
	//lcd.setCursor(14, 1);
	//lcd.print(hightemp);
	//delay(500);
}

bool CheckMillsecToFinishSettingTime() {
	unsigned long time;
	time = millis();
	Serial.println("time know:" + String(time - settingStartTime));

	if (time - settingStartTime >= 3000) {
		return true;
	}
	return false;
}

//clock set
unsigned int seconds()
{
	s = 10 * 3600;
	s = s + (initialMins * 60);
	s = s + initialSecs;
	s = s + (millis() / 1000);
	return s;
}
//this method is for hours
int hours()
{
	hrs = seconds();
	hrs = hrs / 3600;
	hrs = hrs % 24;
	return (hrs + 1);
}
//this method is for minutes
int mins()
{
	minutes = seconds();
	minutes = minutes / 60;
	minutes = minutes % 60;
	return minutes;
}

int secs()
{
	sec = seconds();
	sec = sec % 60;
	return sec;
}
void EPROMClearfunc() {
	for (int i = 0; i < EEPROM.length(); i++) {
		EEPROM.write(i, 0);
	}
}
bool EPROMCheckIsEmptyfunc() {
	for (int i = 0; i < EEPROM.length(); i++) {
		if (EEPROM.read(i) != 0)
			return false;

	}
	return true;
}
void saveEPROMPeriodTempfunc() {
	Serial.println("eeeeeeeprom");
	//addressEprom = 0;
	EEPROM.update(addressEprom, modeHitOrOff);
	EEPROM.update(addressEprom + sizeof(int), lowtemp);
	for (int i = 38; i < 84; i++) {
		for (int j = 0; j < 20; j++)
		{
			myGLCD.clrPixel(i, j);
		}
	}
	myGLCD.setFont(MediumNumbers);
	myGLCD.print(String(lowtemp, 1), 28, 2);
	myGLCD.setFont(SmallFont);


	myGLCD.update();

}
void CheckLightfunc() {
	settingStartTime = millis();

	analogWrite(lightPin, 0);



}
//
//


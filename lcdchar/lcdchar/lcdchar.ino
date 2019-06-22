#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 7
int addressEprom = 0;
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
//consts
int hightemp = 30;
int lowtemp = 28;
int setStatus = 0;
int finalState;
int lowState;
int initLowState;
int initHighState;
int highState;
int initaitState;
int secoundInitState;
int secoundfinalState;
const int lowPin = 8;     // the number of the pushbutton pin
const int setPin = 9;     // the number of the pushbutton pin
const int highPin = 10;     // the number of the pushbutton pin
const int controlPin = 6;     // control when turn on 
const int selectionPin = 13;     // control hotting or cooling 
const int backlightPin = A6;     // backlight 
double timerOn=0;
double timerSt=0;
int statusSystem=0;
int finalMode;
int initMode;
int modeHitOrOff = 0;//mode hit ,cool,off

								 // Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
void setup() {
	//EPROMClearfunc();
	Serial.begin(9600);
	//Serial.println("Dallas Temperature IC Control Library Demo");
	pinMode(lowPin, INPUT);
	pinMode(setPin, INPUT);
	pinMode(highPin, INPUT);
	pinMode(selectionPin, INPUT);
	pinMode(controlPin, OUTPUT);
	pinMode(A6, OUTPUT);

	// Start up the library
	sensors.begin();
	// put your setup code here, to run once:
	lcd.begin(16, 2);
	// Print a message to the LCD.
	lcd.print("TEMP:");

	addressEprom = sizeof(int) + addressEprom;

	if (addressEprom == EEPROM.length()) {
		addressEprom = 0;

	}
	else
	{
		if (EPROMCheckIsEmptyfunc())
		{
			EEPROM.write(addressEprom, hightemp);
			EEPROM.write(addressEprom + sizeof(int), lowtemp);
			EEPROM.write(addressEprom + 2*sizeof(int), timerOn);
			//EEPROM.write(addressEprom + (2 * sizeof(int)), lowtemp);
		}
		else
		{
			hightemp = EEPROM.read(addressEprom);
			lowtemp = EEPROM.read(addressEprom + sizeof(int));
			timerOn=EEPROM.read(addressEprom + 2 * sizeof(int));


		}
		Serial.println(String(timerOn));

	}
}
void loop() {// call sensors.requestTemperatures() to issue a global temperature
			 // request to all devices on the bus
	Serial.println("lowwsss" + String(EEPROM.read(addressEprom + sizeof(int))));
	digitalWrite(A6, HIGH);
	Serial.print(" Requesting temperatures...");
	sensors.requestTemperatures(); // Send the command to get temperatures
	Serial.println("DONE");
	float sensorValue = sensors.getTempCByIndex(0);
	Serial.print("Temperature for Device 1 is: ");
	Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"? 
	Serial.print("Temperature for Device 1 is: ");
	Serial.print(sensorValue);
	lcd.setCursor(6, 0);
	// print the number of seconds since reset:
	lcd.print(sensors.getTempCByIndex(0),1);
	lcd.setCursor(0, 1);
	lcd.print("Setpoint:");
	lcd.setCursor(10, 1);
	lcd.print(lowtemp);
	lcd.setCursor(9, 1);
	//lcd.print("High:");
	//lcd.setCursor(14, 1);
	//lcd.print(hightemp);
	controlTempPeriod(sensorValue);
	CheckButtonFunction();

}
void controlTempPeriod(float temper) {
	float temp = lowtemp;
	int finalMode = digitalRead(lowPin);
	if (finalMode != initMode) {
		delay(10);
		if (finalMode == LOW) {
			modeHitOrOff++;

		}
		initMode = finalMode;

	}
	Serial.println("mode=" + String(modeHitOrOff));
	if (modeHitOrOff == 1 || modeHitOrOff == 0) {
		lcd.setCursor(12, 0);
		lcd.print("Heat");
		/*myGLCD.setFont(TinyFont);
		myGLCD.print("hit", 70, 2);
		myGLCD.update();*/

		if (temper < (temp - 0.4)) {
			Serial.println("onnnnnnnnnnnnnnnnnnnnn");
			if (statusSystem == 0) {
				timerSt = millis();

			}
			statusSystem = 1;

			digitalWrite(controlPin, HIGH);
		}
		else if (temper >= (temp + .5)) {
			if (statusSystem == 1) {
				timerSt = millis() - timerSt;
				timerOn = timerOn + timerSt;
			}
			statusSystem = 0;
			digitalWrite(controlPin, LOW);

		}
	}
	else if (modeHitOrOff == 2) {
		lcd.setCursor(12, 0);
		lcd.print("Cool");
		if (temper > temp + .5) {
			digitalWrite(controlPin, HIGH);
		}
		else if (temper < temp - .5) {
			//delay(5000);
			sensors.requestTemperatures();
			temper = sensors.getTempCByIndex(0);
			if (temper < lowtemp) {
				digitalWrite(controlPin, LOW);
			}
		}
	}
	else if (modeHitOrOff == 3)
	{
		lcd.setCursor(12, 0);
		lcd.print("OFF ");
		digitalWrite(controlPin, LOW);
	}
	else
	{
		modeHitOrOff = 1;
	}
	int input = digitalRead(selectionPin);


}
void CheckButtonFunction() {
	finalState = digitalRead(setPin);
	if (finalState != initaitState) {
		delay(10);
		if (finalState == LOW) {

			setStatus++;
			while (setStatus != 0)
			{
				//delay(10);


				if (setStatus == 1)
				{
					lowPinCheckFunction();
					highPinCheckFunction();
					activeLowFunction();
					saveEPROMPeriodTempfunc();
					//String myString = String(setStatus);
					//Serial.print(myString);
				}
				//else if (setStatus == 2)
				//{
				//	//Serial.print(setStatus);
				//	//setStatus++;
				//	lowPinCheckFunction();
				//	highPinCheckFunction();
				//	activehighFunction();
				//}
				else
				{
					setStatus = 0;
				}

				secoundfinalState = digitalRead(setPin);
				if (secoundfinalState != secoundInitState) {
					delay(10);
					if (secoundfinalState == LOW)
					{



						Serial.print("click is in while");
						setStatus++;
						String myString = String(setStatus);
						Serial.print(myString);


					}



				}
				secoundInitState = secoundfinalState;
				lcd.setCursor(0, 1);
				lcd.print("Setpoint:");
				lcd.setCursor(10, 1);
				lcd.print(lowtemp);
				lcd.setCursor(9, 1);
				/*lcd.print("High:");
				lcd.setCursor(14, 1);
				lcd.print(hightemp);*/
				Serial.println(setStatus);


			}
			Serial.print("while finished");
		}
		initaitState = finalState;
	}



}






void lowPinCheckFunction() {

	lowState = digitalRead(lowPin);

	if (lowState != initLowState) {
		delay(10);
		if (lowState == LOW) {

			//lowState=digitalRead(lowPin);
			if (setStatus == 1) {
				int checklow = lowtemp - 1;
				if (lowtemp - 1 != -50)
					lowtemp--;
				else
					setErrorFunction();
			}
			else if (setStatus == 2) {

				if ((hightemp - 1 != lowtemp + 1))
					hightemp--;
				else
					setErrorFunction();
			}

		}
		initLowState = lowState;

	}

}
void setErrorFunction() {

	lcd.setCursor(12, 0);
	lcd.print("Error");
	
	
	delay(3000);
	lcd.setCursor(9, 0);
	lcd.print("     ");


}

void highPinCheckFunction() {

	highState = digitalRead(highPin);
	if (highState != initHighState) {
		delay(10);
		if (highState == LOW) {

			//highState=digitalRead(highPin);
			if (setStatus == 1) {
				if (lowtemp + 1 < 120)
					lowtemp++;
				else {
					setErrorFunction();
				}
			}
			else if (setStatus == 2) {
				if (hightemp + 1 < 50)
					hightemp++;
				else
					setErrorFunction();
			}

		}
		initHighState = highState;
	}



}


void activeLowFunction() {
	delay(500);
	Serial.println("low");
	lcd.setCursor(0, 1);
	lcd.print("        ");
	lcd.setCursor(10, 1);
	lcd.print(lowtemp);
	delay(500);
}

void activehighFunction() {
	delay(500);
	Serial.println("high");
	lcd.setCursor(9, 1);
	lcd.print("    ");
	lcd.setCursor(14, 1);
	lcd.print(hightemp);
	delay(500);
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
	EEPROM.update(addressEprom, hightemp);
	EEPROM.update(addressEprom + sizeof(int), lowtemp);
}
void lowPinModeChange() {

	lowState = digitalRead(lowPin);

	if (lowState != initLowState) {
		delay(10);
		if (lowState == LOW) {

			

		}
		initLowState = lowState;

	}

}
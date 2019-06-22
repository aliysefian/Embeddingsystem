#include <OneWire.h>
#include <DallasTemperature.h>
#include <LCD5110_Graph.h>
#include <LiquidCrystal.h>
LCD5110 myGLCD(7, 6, 5, 3, 4);

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 11

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
//consts
int hightemp = 41;
int lowtemp = 26;
int setStatus = 0;
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
const int lowPin = 8;     // the number of the pushbutton pin
const int setPin = 9;     // the number of the pushbutton pin
const int highPin = 10;     // the number of the pushbutton pin
const int controlPin = 12;     // control when turn on 
const int selectionPin = 13;     // control hotting or cooling 
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

	0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xF0, 0xF8, 0xF8, 0xF8, 0x7C, 0x7C, 0x7C, 0xFC, 0xFE,
	0xFE, 0xFE, 0x7E, 0x3E, 0x3E, 0x3E, 0xFE, 0xFE, 0xFE, 0xFE, 0x7C, 0x7C, 0x7C, 0xF8, 0xF8, 0xF8,
	0xF0, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x83, 0x01, 0x00, 0x10, 0x7C, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFE, 0xFC, 0x38, 0x00, 0x01, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x00,
	0x00, 0x00, 0x01, 0x07, 0x0F, 0x0F, 0x1F, 0x3F, 0x3F, 0x7E, 0x7C, 0x7C, 0xF8, 0xF8, 0xF1, 0xF1,
	0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF1, 0xF1, 0xF8, 0xF8, 0x7C, 0x7C, 0x7E, 0x3F,
	0x3F, 0x1F, 0x1F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t sun[] PROGMEM = {


	0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x48, 0x58, 0xD0, 0xA0, 0xE0, 0xD8, 0x73, 0x67, 0x3C,
	0x38, 0x14, 0x1C, 0x10, 0x1F, 0x1F, 0x10, 0x1C, 0x1C, 0x10, 0x38, 0x2E, 0x33, 0x78, 0x68, 0xA0,
	0xB0, 0xD8, 0x48, 0x08, 0x80, 0x80, 0x80, 0x80, 0x00, 0x10, 0x18, 0x18, 0x10, 0x72, 0xB5, 0xB5,
	0xB5, 0xFF, 0xFF, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xFF, 0xBD, 0xD5, 0x55, 0x10, 0x18, 0x18,
	0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x12, 0x1A, 0x09, 0x0D, 0x17, 0x1B, 0xCE, 0x64, 0x3C,
	0x0C, 0x28, 0x38, 0x08, 0xF8, 0xF8, 0x08, 0x38, 0x28, 0x08, 0x38, 0xE4, 0xCC, 0x1E, 0x16, 0x07,
	0x0B, 0x1A, 0x12, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t snow[] PROGMEM = {

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x24, 0x2C, 0x3C, 0x38, 0x70, 0xF8, 0xDC,
	0x8E, 0xC6, 0xC0, 0x00, 0x00, 0xC0, 0xC6, 0x8E, 0xDC, 0xF8, 0x70, 0x38, 0x3C, 0x2C, 0x24, 0x20,
	0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x93, 0xFF, 0xFE, 0x7C, 0x10,
	0x10, 0x54, 0x7C, 0x7D, 0x7D, 0xBB, 0xBB, 0x93, 0x93, 0xFF, 0xFF, 0x7C, 0x7C, 0xFF, 0xFF, 0x93,
	0x93, 0xBB, 0xBB, 0x7D, 0x7D, 0x7C, 0x54, 0x10, 0x10, 0x7C, 0xFE, 0xFF, 0x93, 0x10, 0x10, 0x10,
	0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x08, 0x08, 0x08, 0x48, 0x68, 0x79, 0x39, 0x1D, 0x3F, 0x77,
	0xE3, 0xC7, 0x07, 0x00, 0x00, 0x07, 0xC7, 0xE3, 0x77, 0x3F, 0x1D, 0x39, 0x79, 0x68, 0x48, 0x08,
	0x08, 0x08, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

int finalMode;
int initMode;
int modeHitOrOff=0;//mode hit ,cool,off
int status = 0;
float y;
uint8_t* bm;
int pacy;

//


// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
void setup() {
	Serial.begin(9600);
	myGLCD.InitLCD();
	myGLCD.setFont(SmallFont);
	//Serial.println("Dallas Temperature IC Control Library Demo");
	pinMode(lowPin, INPUT);
	pinMode(setPin, INPUT);
	pinMode(highPin, INPUT);
	pinMode(selectionPin, INPUT);
	pinMode(controlPin, OUTPUT);

	// Start up the library
	sensors.begin();
	// put your setup code here, to run once:
	// lcd.begin(16, 2);
	// Print a message to the LCD.
	//lcd.print("TEMP:");

}

void loop() {// call sensors.requestTemperatures() to issue a global temperature
			 // request to all devices on the bus
	

	
	Serial.print(" Requesting temperatures...");
	sensors.requestTemperatures(); // Send the command to get temperatures
	Serial.println("DONE");
	int sensorValue = sensors.getTempCByIndex(0);
	Serial.print("Temperature for Device 1 is: ");
	Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"? 
	Serial.print("Temperature for Device 1 is: ");
	Serial.print(sensorValue); 
							   
							   
							   
							   
	//myGLCD.clrScr();
	myGLCD.setFont(SmallFont);
	myGLCD.print("L:", 2, 2);
	myGLCD.print(String(lowtemp),15, 2);
	myGLCD.print("H:", 55, 2);
	myGLCD.print(String(hightemp), 65, 2);
	
	
	myGLCD.setFont(BigNumbers);
	myGLCD.print(String(sensorValue), 2, 20);
	//myGLCD.print("DEMO", CENTER, 20);
	//myGLCD.drawRoundRect(0, 0, 62, 25);
	//myGLCD.drawRoundRect(0, 25, 63, 48);

	myGLCD.update();
	controlTempPeriod(sensorValue);
	//CheckButtonFunction();
	CheckSetForStartMenuFunction();

}
void controlTempPeriod(int temper) {
	int finalMode = digitalRead(selectionPin);
	if (finalMode != initMode) {
		delay(10);
		if (finalMode == LOW) {
			modeHitOrOff++;
			
		}
		initMode = finalMode;
		
	}
	Serial.println("mode=" + String(modeHitOrOff));
	if (modeHitOrOff == 1 || modeHitOrOff == 0) {
		myGLCD.drawBitmap(45, 20, sun, 40, 25);
		myGLCD.update();
		/*myGLCD.setFont(TinyFont);
		myGLCD.print("hit", 70, 2);
		myGLCD.update();*/
		if (temper < lowtemp) {
			digitalWrite(controlPin, HIGH);
		}
		else if (temper >= hightemp) {
			delay(5000);
			sensors.requestTemperatures();
			temper = sensors.getTempCByIndex(0);
			if (temper >= hightemp) {
				digitalWrite(controlPin, LOW);
			}
		}
	}
	else if (modeHitOrOff == 2) {
		/*myGLCD.setFont(TinyFont);
		myGLCD.print("cool", 70, 2);
		myGLCD.update();*/
		myGLCD.drawBitmap(45, 20, snow, 40, 25);
		myGLCD.update();
		if (temper > hightemp) {
			digitalWrite(controlPin, HIGH);
		}
		else if (temper < lowtemp) {
			delay(5000);
			sensors.requestTemperatures();
			temper = sensors.getTempCByIndex(0);
			if (temper < lowtemp) {
				digitalWrite(controlPin, LOW);
			}
		}
	}
	else if (modeHitOrOff == 3)
	{
		myGLCD.drawBitmap(45, 20, off, 40, 25);
		myGLCD.update();
	}
	else
	{
		modeHitOrOff = 0;
	}
	
}
void CheckButtonFunction() {
	finalState = digitalRead(setPin);
	if (finalState != initaitState) {
		delay(10);
		if (finalState == LOW) {

			setStatus++;
			while (setStatus != 0)
			{
				delay(100);
				ShowSetMenuCheckFunction();

				if (setStatus == 1)
				{
					lowPinCheckFunction();
					highPinCheckFunction();
					activeLowFunction();

					//String myString = String(setStatus);
					//Serial.print(myString);
				}
				else if (setStatus == 2)
				{
					//Serial.print(setStatus);
					//setStatus++;
					lowPinCheckFunction();
					highPinCheckFunction();
					activehighFunction();
				}
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
				// lcd.setCursor(0, 1);
				// lcd.print("Low:");
				// lcd.setCursor(4, 1);
				// lcd.print(lowtemp);
				// lcd.setCursor(9, 1);
				// lcd.print("High:");
				// lcd.setCursor(14, 1);
				// lcd.print(hightemp);
				Serial.println(setStatus);


			}
			Serial.print("while finished");
		}
		initaitState = finalState;
	}



}
void CheckSetForStartMenuFunction() {
	finalState = digitalRead(setPin);
	if (finalState != initaitState) {
		delay(10);

		if (finalState == LOW) {
			Serial.print("while fssssinished");
			setCounter++;
			settingStartTime = millis();
			Serial.println(String(setCounter));


			ShowSetMenuCheckFunction();

		}
		initaitState = finalState;
	}



}

void ShowSetMenuCheckFunction()
{
	sta = 1;
	int j = 1;
	int start = 0;
	Serial.print("start menu");
	myGLCD.clrScr();
	myGLCD.setFont(SmallFont);
	myGLCD.print("Set High Temp", CENTER, 2);
	myGLCD.print("Set Low Temp", CENTER, 15);
	myGLCD.print("Exit", CENTER, 30);

	myGLCD.drawRoundRect(0, 0, 83, 1 * 15 - 2);
	myGLCD.update();
	//j=2;
	int initaitState;
	while (sta != 0) {
		// Serial.print(" menu while");
		if (setCounter == 1) {//setcounter for menu
			lowState = digitalRead(lowPin);
			
			if (CheckMillsecToFinishSettingTime()) {
				setCounter = 0;
				sta = 0;
				break;
			}
			if (lowState != initLowState) {
				delay(10);
				if (lowState == LOW) {
					j++;
					if (j == 4)
						j = 1;
					switch (j)
					{
					case 1:
						myGLCD.clrRoundRect(0, 2 * 15 - 2, 83, 3 * 15 - 2);
						myGLCD.drawRoundRect(0, 0, 83, 1 * 15 - 2);
						myGLCD.update();
						break;
					case 2:
						myGLCD.clrRoundRect(0, 0, 83, 1 * 15 - 2);
						myGLCD.drawRoundRect(0, 1 * 15 - 2, 83, 2 * 15 - 2);
						myGLCD.update(); 
						break;
					case 3:
						myGLCD.clrRoundRect(0, 1 * 15 - 2, 83, 2 * 15 - 2);
						myGLCD.drawRoundRect(0, 2 * 15 - 2, 83, 3 * 15 - 2);
						myGLCD.update(); 
						break;
					
					}
				}
				initLowState = lowState;

				Serial.println("run edd:");

			}



			highState = digitalRead(highPin);
			if (highState != initHighState) {
				delay(10);
				if (highState == LOW) {
					j--;
					if (j == 0)
						j = 3;
					switch (j)
					{
					case 1:
						myGLCD.clrRoundRect(0, 1 * 15 - 2, 83, 2 * 15 - 2);
						myGLCD.drawRoundRect(0, 0, 83, 1 * 15 - 2);
						myGLCD.update();;
						break;
					case 2:
						myGLCD.clrRoundRect(0, 2 * 15 - 2, 83, 3 * 15 - 2);
						myGLCD.drawRoundRect(0, 1 * 15 - 2, 83, 2 * 15 - 2);
						myGLCD.update();;
						break;
					case 3:
						myGLCD.clrRoundRect(0, 0 * 15 - 2, 83, 1 * 15 - 2);
						myGLCD.drawRoundRect(0, 2 * 15 - 2, 83, 3 * 15 - 2);
						myGLCD.update();;
						break;
					
					}

					
				}
				initHighState = highState;

			}
			Serial.println("run down:" + String(j));

			finalState = digitalRead(setPin);
			if (finalState != initaitState) {
				delay(10);

				if (finalState == LOW) {
					Serial.println("check counter");
					setCounter++;
					Serial.println(String(setCounter));
				}
				initaitState = finalState;

			}

		}

		if (setCounter == 2) {
			myGLCD.clrScr();
			MenuSelectedFunction(j);

		}
	}

}
void MenuSelectedFunction(int sel)
{

	
	int initaitState;
	while (1) {
		if (CheckMillsecToFinishSettingTime()) {
			setCounter = 0;
			sta = 0;
			break;
		}
		Serial.println("set menu:");
		Serial.println(String(sel));
		
		
			setStatus = sel ;

		
		Serial.println("setStatus:");
		Serial.println(String(setStatus));
		if (setStatus == 1 || setStatus == 2)
		{
			Serial.println("set is ch");
			lowPinCheckFunction();
			highPinCheckFunction();
			activeLowFunction();

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
		finalState = digitalRead(setPin);
		if (finalState != initaitState) {
			delay(10);

			if (finalState == LOW) {
				setCounter++;
				Serial.println(String(setCounter));
				
				if (setCounter == 3) {
					setCounter = 0;
					myGLCD.clrScr();
					sta = 0;
					break;
				}
					
			}
			initaitState = finalState;

		}

	}
	Serial.print("while exit");


}
void lowPinCheckFunction() {
	//int initLowState;
	lowState = digitalRead(lowPin);
	if (setStatus == 2) {
		//myGLCD.clrScr();
		myGLCD.setFont(SmallFont);
		myGLCD.print("Low Temp", CENTER, 4);
		myGLCD.setFont(MediumNumbers);
		myGLCD.print(String(lowtemp), CENTER, 20);
		myGLCD.update();
	}
	else
	{
		//myGLCD.clrScr();
		myGLCD.setFont(SmallFont);
		myGLCD.print("High Temp", CENTER, 4);
		myGLCD.setFont(BigNumbers);
		myGLCD.print(String(hightemp), CENTER, 20);
		myGLCD.update();
	}
	
	if (lowState != initLowState) {
		delay(10);
		if (lowState == LOW) {
			Serial.println("low is active");
			//lowState=digitalRead(lowPin);
			if (setStatus == 2) {
				int checklow = lowtemp - 1;
				if (lowtemp - 1 != 0) {
					lowtemp--;
					myGLCD.setFont(BigNumbers);
					myGLCD.print(String(lowtemp), CENTER, 20);
					myGLCD.update();
				}
				else
					setErrorFunction();
			}
			else if (setStatus == 1) {

				if ((hightemp - 1 != lowtemp + 2)) {
					hightemp--;
					myGLCD.setFont(BigNumbers);
					myGLCD.print(String(hightemp), CENTER, 20);
					myGLCD.update();
				}
				else
					setErrorFunction();
			}

		}
		initLowState = lowState;

	}

}
void setErrorFunction() {

	// lcd.setCursor(9, 0);
	//lcd.print("Error");
	delay(3000);
	//lcd.setCursor(9, 0);
	//lcd.print("     ");


}
void highPinCheckFunction() {

	highState = digitalRead(highPin);
	if (setStatus == 2) {
		//myGLCD.clrScr();
		myGLCD.setFont(SmallFont);
		myGLCD.print("Low Temp", CENTER, 4);
		myGLCD.setFont(BigNumbers);
		myGLCD.print(String(lowtemp), CENTER, 20);
		myGLCD.update();
	}
	else
	{
		//myGLCD.clrScr();
		myGLCD.setFont(SmallFont);
		myGLCD.print("High Temp", CENTER, 4);
		myGLCD.setFont(BigNumbers);
		myGLCD.print(String(hightemp), CENTER, 20);
		myGLCD.update();
	}
	if (highState != initHighState) {
		delay(10);
		if (highState == LOW) {

			//highState=digitalRead(highPin);
			if (setStatus == 2) {
				if (lowtemp + 1 != hightemp - 2) {
					lowtemp++;
					myGLCD.setFont(BigNumbers);
					myGLCD.print(String(lowtemp), CENTER, 20);
					myGLCD.update();
				}
				else {
					setErrorFunction();
				}
			}
			else if (setStatus == 1) {
				if (hightemp + 1 < 120) {
					hightemp++;
					myGLCD.setFont(BigNumbers);
					myGLCD.print(String(hightemp), CENTER, 20);
					myGLCD.update();
				}
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
	// lcd.setCursor(0, 1);
	// lcd.print("   ");
	// lcd.setCursor(4, 1);
	// lcd.print(lowtemp);
	delay(500);
}

void activehighFunction() {
	delay(500);
	Serial.println("high");
	//lcd.setCursor(9, 1);
	//lcd.print("    ");
	//lcd.setCursor(14, 1);
	//lcd.print(hightemp);
	delay(500);
}

bool CheckMillsecToFinishSettingTime() {
	unsigned long time;
	time = millis();
	Serial.println("time know:" + String(time - settingStartTime));

	if (time - settingStartTime>=120000) {
		return true;
	}
	return false;
}

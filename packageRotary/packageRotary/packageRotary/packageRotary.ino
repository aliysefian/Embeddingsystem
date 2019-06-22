
#include <EEPROM.h>
#include <math.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LCD5110_Graph.h>
#include  <SPI.h> //Include SPI Code Library which can be downloaded below
#include "nRF24L01.h" //Include NRF24L01 Code Library which can be downloaded below
#include "RF24.h" //Inlcude NRF24 Code Library which can be downloaded below

//      SCK  or CLK- Pin 6 arduino
//      MOSI or DIN - Pin 7 arduino
//      DC   - Pin 5 arduino
//      RST  - Pin 4 arduino
//      CS or CE  - Pin 3 arduino
//      VCC - 3.3 volt arduino
//      BL - 5 volt arduino
//      GND- ground of arduino
LCD5110 myGLCD(6, 7, A5, 4, 3);
#define ONE_WIRE_BUS 8//todo:must be change

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
//for rotary
#define encoder0PinA  2
#define encoder0PinB  A2

volatile unsigned int encoder0Pos = 0;
///main variant
int lightPin = 5;
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
const int lowPin = A0;     // the number of the pushbutton pin‍‍پایین
const int setPin;     // the number of the pushbutton pin 
const int highPin = A2;     // the number of the pushbutton pin بالا
const int controlPin = A3;     // control when turn on کلید رله
const int selectionPin = A1;     // control hotting or cooling lمود اینس
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
const uint8_t con4[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xC0,
	0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
	0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x70, 0x78, 0x78, 0x78, 0x7C, 0x3C, 0x3C, 0x3C,
	0x1E, 0x1E, 0x1E, 0x1E, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x87, 0x87, 0x87, 0x83, 0x83,
	0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1,
	0xC1, 0xC1, 0xC1, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x83, 0x83, 0x87, 0x87,
	0x87, 0x07, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x1E, 0x1E, 0x1E, 0x1E, 0x3C, 0x3C, 0x3C, 0x7C,
	0x78, 0x78, 0x78, 0x70, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0C, 0x0E, 0x1E, 0x1E, 0x1E, 0x0F, 0x0F, 0x0F, 0x0F, 0x07,
	0x07, 0x87, 0x87, 0x83, 0x83, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC1, 0xC1, 0xC1, 0xE1, 0xE1, 0xE1,
	0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xC1, 0xC1, 0xC1, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x83,
	0x83, 0x87, 0x87, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x1E, 0x1E, 0x1E, 0x0E, 0x0C, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x81,
	0xC1, 0xC1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xC1, 0xC1, 0x81, 0x01, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x07, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07,
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x68, 0x70, 0x64, 0x60, 0x60, 0x98, 0x64, 0x60, 0xE4, 0x68, 0x64, 0xDC,
	0x30, 0x34, 0xB0, 0x0C, 0x10, 0x74, 0x00, 0x00, 0x34,

};
const uint8_t con1[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
	0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07,
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,

};
const uint8_t con2[] PROGMEM = {

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0,
	0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80,
	0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x81,
	0xC1, 0xC1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xC1, 0xC1, 0x81, 0x01, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x07, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07,
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x00,
};
const uint8_t con3[] PROGMEM = {

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80,
	0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
	0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0C, 0x0E, 0x1E, 0x1E, 0x1E, 0x0F, 0x0F, 0x0F, 0x0F, 0x07,
	0x07, 0x87, 0x87, 0x83, 0x83, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC1, 0xC1, 0xC1, 0xE1, 0xE1, 0xE1,
	0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xC1, 0xC1, 0xC1, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x83,
	0x83, 0x87, 0x87, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x1E, 0x1E, 0x1E, 0x0E, 0x0C, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x81,
	0xC1, 0xC1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xC1, 0xC1, 0x81, 0x01, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x07, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07,
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
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

///
int msg[1];
RF24 radio(9, 10); // The NRF24L01 Pin CE and Pin CSN
const uint64_t pipe = 0xE8E8F0F0E1LL; //Communication Pip Address
const uint64_t pipeTransfer = 0xA8E8F0F0E1LL; //Start Pipe Communication Address

int switchButton = 7; // Push button connected to Arduino pin 7

void setup(void) {
	pinMode(encoder0PinA, INPUT);
	digitalWrite(encoder0PinA, HIGH);       // turn on pull-up resistor
	pinMode(encoder0PinB, INPUT);
	digitalWrite(encoder0PinB, HIGH);       // turn on pull-up resistor

											//attachInterrupt(0, doEncoder_Expanded, CHANGE);  // encoder pin on interrupt 0 - pin 2

//	Serial.begin(9600); //Start Serial Communication at baud rate 9600
						//EPROMClearfunc();
						//
	Serial.end();
	radio.begin();
	radio.openWritingPipe(pipe);
	radio.openReadingPipe(1, pipeTransfer);

	//
	//
	myGLCD.InitLCD();
	myGLCD.setFont(SmallFont);
	//Serial.println("Dallas Temperature IC Control Library Demo");
	//pinMode(lowPin, INPUT);
	//pinMode(setPin, INPUT);
	//pinMode(highPin, INPUT);
	pinMode(selectionPin, INPUT_PULLUP);
	pinMode(controlPin, OUTPUT);

	//pinMode(A1, OUTPUT);
	//analogWrite(A1, 1023);
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
			int point = EEPROM.read(addressEprom + (2 * sizeof(int)));
			if (point) {
				lowtemp += .5;
			}

		}


	}

	myGLCD.setFont(BigNumbers);
	myGLCD.print(String(1, 1), 1, 20);

	attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2

													 //

} //Open Communication Pipe
bool state = false;
void loop() {// call sensors.requestTemperatures() to issue a global temperature
			 // request to all devices on the bus
	Serial.println("wwwwwwwwwwwwwwwww nrf");

	/*state = !state;
	if (state)
	digitalWrite(controlPin, HIGH);

	else
	{
	digitalWrite(controlPin, LOW);

	}*/
	//delay(300);
	msg[0] = 221; //Send the 111 to the reciever 
	radio.write(msg, 1);
	delay(5);

	radio.startListening();
	int co = 0;
	unsigned long startTime = 0;
	while (!radio.available()) {
		Serial.println("no nrf");
		if (startTime == 0)
			startTime = millis();
		if (CheckPeriod(startTime, 10000)) {
			Serial.println('stop');
			reciveError(true);
			startTime = 0;
			//break;
		}
	}
	reciveError(false);
	radio.read(msg, 1);
	if (msg[0] == 0) {
		co++;

	}
	else
	{
		co = 0;
	}

	if (co == 6) {
		Serial.println('stop');
		//reciveError();
		co = 0;
	}

	Serial.println(msg[0]);

	delay(5);
	radio.stopListening();
	//Serial.println("eepromlen" + String(EEPROM.length()));
	//delay(10000);
	//lowPinCheckFunctionNew();


	//delay(100);
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
		//Serial.println("aaaaa:" + String(sensorValue, 1));
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
		while ((millis() - time < 1000) && flagClearScreen)
		{
			if (CheckMillsecToFinishSettingTime() == true) {
				Serial.println("finis");
				analogWrite(lightPin, 1023);
				settingStartTime = millis();
			}
			//	Serial.println(String(lowtemp));
			controlTempPeriod(sensorValue);

			//lowPinCheckFunction();

			//highPinCheckFunction();
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

	msg[0] = 221; //Send the 111 to the reciever 
	radio.write(msg, 1);


}
void reciveError(bool exec) {

	//myGLCD.clrScr();
	//myGLCD.setFont(SmallFont);
	////myGLCD.print("Temp", 2, 2);
	//myGLCD.print("Error recive", 4, 20);

	////digitalWrite(controlPin, LOW);
	//myGLCD.update();
	//delay(500);
	ShowConnetingpic(exec);
}
void controlTempPeriod(float temper) {
	float temp = lowtemp;
	int finalMode = digitalRead(selectionPin);
	if (finalMode != initMode) {
		delay(50);
		if (finalMode == LOW) {
			modeHitOrOff++;
			modeHitOrOff = modeHitOrOff % 3;
			saveEPROMPeriodTempfunc();
			CheckLightfunc();

		}
		initMode = finalMode;

	}
	//	float s = round(1.2333333);
	//	Serial.println("yyyyyyyyyaaaaa" + String(temper - temp));

	if (modeHitOrOff == 0) {
		myGLCD.drawBitmap(56, 20, snow, 35, 35);
		myGLCD.update();
		/*myGLCD.setFont(TinyFont);
		myGLCD.print("hit", 70, 2);
		myGLCD.update();*/
		if (temper - temp <= -0.30) {
			digitalWrite(controlPin, HIGH);
			msg[0] = 111; //Send the 111 to the reciever 
			radio.write(msg, 1);
			//delay(10);
		}
		else if (temper - temp > 0.30) {
			//delay(5000);

			msg[0] = 222; //Send the 111 to the reciever 
			radio.write(msg, 1);
			digitalWrite(controlPin, LOW);

		}
		else {
			//Serial.println("whyyyyyyyyyyyyyyyy" + String(temp));

			msg[0] = 240; //Send the 111 to the reciever 
			radio.write(msg, 1);
			//digitalWrite(controlPin, LOW);
		}
	}
	else if (modeHitOrOff == 1) {
		/*myGLCD.setFont(TinyFont);
		myGLCD.print("cool", 70, 2);
		myGLCD.update();*/
		myGLCD.drawBitmap(56, 20, sun, 35, 35);
		myGLCD.update();
		if (temper > temp + .3) {
			msg[0] = 111; //Send the 111 to the reciever 
			radio.write(msg, 1);
			digitalWrite(controlPin, HIGH);
		}
		else if (temper < temp - .3) {
			//delay(5000);
			msg[0] = 222; //Send the 111 to the reciever 
			radio.write(msg, 1);
			digitalWrite(controlPin, LOW);

		}
		else {
			//Serial.println("whyyyyyyyyyyyyyyyy" + String(temp));

			msg[0] = 240; //Send the 111 to the reciever 
			radio.write(msg, 1);
			//digitalWrite(controlPin, LOW);
		}
	}
	else if (modeHitOrOff == 2)
	{
		myGLCD.drawBitmap(56, 20, off, 30, 30);
		myGLCD.update();
		digitalWrite(controlPin, LOW);
		msg[0] = 222; //Send the 111 to the reciever 
		radio.write(msg, 1);

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
			//Serial.println("low is active");
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

		if (highState == LOW && lowtemp + periodTempChange < 120) {
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
	//Serial.println("time know:" + String(time - settingStartTime));

	if (time - settingStartTime >= 3000) {
		return true;
	}
	return false;
}

//clock set

//this method is for hours

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
	//Serial.println("eeeeeeeprom");
	//addressEprom = 0;
	EEPROM.update(addressEprom, modeHitOrOff);

	int te = lowtemp;
	int point = 0;
	if (lowtemp - te > 0) {
		point = 1;//برای نیم ها 

	}
	EEPROM.update(addressEprom + sizeof(int), te);
	EEPROM.write(addressEprom + 2 * sizeof(int), point);
	for (int i = 38; i < 84; i++) {
		for (int j = 0; j < 20; j++)
		{
			myGLCD.clrPixel(i, j);
		}
	}
	//Serial.println("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	/*for (int i = 0; i < EEPROM.length(); i += sizeof(int)) {
		if (EEPROM.read(i) != 0)
			Serial.println(EEPROM.read(i));

	}
	Serial.println("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
*/

	myGLCD.setFont(MediumNumbers);
	myGLCD.print(String(lowtemp, 1), 28, 2);
	myGLCD.setFont(SmallFont);

	myGLCD.update();

}
void CheckLightfunc() {
	settingStartTime = millis();

	//analogWrite(lightPin, 0);



}
bool CheckPeriod(unsigned long time, unsigned long period) {

	float res = (millis() - time);
	Serial.println("time is :" + String(res));
	if (millis() - time > period) {
		/*Serial.println("time is :" + String(res));
		Serial.println("mill is :" + String(millis()));
		Serial.println("ti is :" + String((time)));
		Serial.println("time sum is :" + String((time + period)));*/
		return true;
	}
	return false;
}
void ShowConnetingpic(bool trigger) {
	int cnt = 0;
	while (trigger)
	{
		if (cnt == 10) {
			cnt == 0;
			break;

		}
		cnt++;
		int i = 100;
		myGLCD.drawBitmap(0, 0, con1, 84, 42);
		myGLCD.update();
		delay(i);
		myGLCD.drawBitmap(0, 0, con2, 84, 42);
		myGLCD.update();
		delay(i);
		myGLCD.drawBitmap(0, 0, con3, 84, 42);
		myGLCD.update();
		delay(i);
		myGLCD.drawBitmap(0, 0, con4, 84, 42);
		myGLCD.update();
		delay(i);
		myGLCD.clrScr();
	}
}
byte cntINT = 1;
void doEncoder() {
	/* If pinA and pinB are both high or both low, it is spinning
	forward. If they're different, it's going backward.

	For more information on speeding up this process, see
	[Reference/PortManipulation], specifically the PIND register.
	*/
	if (cntINT == 2) {
		if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
			encoder0Pos++;
			lowtemp = lowtemp + periodTempChange;
		}
		else {
			encoder0Pos--;
			lowtemp = lowtemp - periodTempChange;
		}
		saveEPROMPeriodTempfunc();
	}
	//Serial.println(encoder0Pos, DEC);
	
	if (cntINT >= 2)
	{
		cntINT = 0;
	}
	cntINT++;
}

void doEncoder_Expanded() {
	//noInterrupts();

	if (cntINT == 2) {

		if (digitalRead(encoder0PinA) == HIGH) {   // found a low-to-high on channel A
			if (digitalRead(encoder0PinB) == LOW) {  // check channel B to see which way
													 // encoder is turning
				encoder0Pos = encoder0Pos - 1;         // CCW
				lowtemp = lowtemp - periodTempChange;
			}
			else {
				encoder0Pos = encoder0Pos + 1;         // CW
				lowtemp = lowtemp + periodTempChange;
			}
		}
		else                                        // found a high-to-low on channel A
		{

			if (digitalRead(encoder0PinB) == LOW) {   // check channel B to see which way
													  // encoder is turning
				encoder0Pos = encoder0Pos - 1;          // CW
				lowtemp = lowtemp + periodTempChange;
			}
			else {
				encoder0Pos = encoder0Pos + 1;          // CCW
				lowtemp = lowtemp - periodTempChange;
			}

		}
		saveEPROMPeriodTempfunc();
		//interrupts();

	}
	if (cntINT >= 2)
	{
		cntINT = 0;
	}
	cntINT++;

	//Serial.println(encoder0Pos, DEC);          // debug - remember to comment out
	// before final program run
	// you don't want serial slowing down your program if not needed
}
volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;
void updateEncoder() {
	int MSB = digitalRead(encoder0PinA); //MSB = most significant bit
	int LSB = digitalRead(encoder0PinB); //LSB = least significant bit

	int encoded = (MSB << 1) | LSB;
	//converting the 2 pin value to single number 
	int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
	if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
		lowtemp = lowtemp + periodTempChange;
	}
	if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
		lowtemp = lowtemp - periodTempChange;
	}
	lastEncoded = encoded; //store this value for next time
	saveEPROMPeriodTempfunc();
}
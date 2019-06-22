// All the mcufriend.com UNO shields have the same pinout.
// i.e. control pins A0-A4.  Data D2-D9.  microSD D10-D13.
// Touchscreens are normally A1, A2, D7, D6 but the order varies
//
// This demo should work with most Adafruit TFT libraries
// If you are not using a shield,  use a full Adafruit constructor()
// e.g. Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif



uint16_t g_identifier;



void setup(void) {
	Serial.begin(9600);
	uint32_t when = millis();
	//    while (!Serial) ;   //hangs a Leonardo until you connect a Serial
	if (!Serial) delay(5000);           //allow some time for Leonardo
	Serial.println("Serial took " + String((millis() - when)) + "ms to start");
	static uint16_t identifier;
	//    tft.reset();                 //we can't read ID on 9341 until begin()
	g_identifier = tft.readID(); //
	Serial.print("ID = 0x");
	Serial.println(g_identifier, HEX);
	if (g_identifier == 0x00D3 || g_identifier == 0xD3D3) g_identifier = 0x9481; // write-only shield
	if (g_identifier == 0xFFFF) g_identifier = 0x9341; // serial
													   //    g_identifier = 0x9329;                             // force ID
	tft.begin(g_identifier);
}




void loop(void) {
	uint8_t aspect;
	uint16_t pixel;
	char *aspectname[] = {
		"PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV"
	};
	char *colorname[] = { "BLUE", "GREEN", "RED", "GRAY" };
	uint16_t colormask[] = { 0x001F, 0x07E0, 0xF800, 0xFFFF };
	uint16_t dx, rgb, n, wid, ht;
	tft.setRotation(0);
	//    for (uint8_t i = 0; i < 2; i++) showhanzi(0, 0, i), delay(1000);
	//runtests();
	testText();

	delay(2000);
	//tft.println("INVERT DISPLAY");
	//tft.invertDisplay(true);
	//delay(2000);
///	tft.invertDisplay(false);
}


// Standard Adafruit tests.  will adjust to screen size

unsigned long testFillScreen() {
	unsigned long start = micros();
	tft.fillScreen(BLACK);
	tft.fillScreen(RED);
	tft.fillScreen(GREEN);
	tft.fillScreen(BLUE);
	tft.fillScreen(BLACK);
	return micros() - start;
}

unsigned long testText() {
	unsigned long start;
	tft.fillScreen(BLACK);
	start = micros();
	
	tft.fillRect(0, 0, 140, 120, GREEN);
	tft.setCursor(2, 10);

	tft.setTextColor(BLACK);  tft.setTextSize(1);
	tft.println("Current Temp is:");
	tft.setCursor(10,30);

	tft.setTextColor(BLACK);  tft.setTextSize(10);
	tft.println("28");
	delay(20000);
	/*tft.setTextColor(YELLOW); tft.setTextSize(2);
	tft.println(123.45);
	tft.setTextColor(RED);    tft.setTextSize(3);
	tft.println(0xDEADBEEF, HEX);
	tft.println();
	tft.setTextColor(GREEN);
	tft.setTextSize(5);
	tft.println("Groop");
	tft.setTextSize(2);
	tft.println("I implore thee,");
	tft.setTextSize(1);
	tft.println("my foonting turlingdromes.");
	tft.println("And hooptiously drangle me");
	tft.println("with crinkly bindlewurdles,");
	tft.println("Or I will rend thee");
	tft.println("in the gobberwarts");
	tft.println("with my blurglecruncheon,");
	tft.println("see if I don't!");*/
	return micros() - start;
}

unsigned long testLines(uint16_t color) {
	unsigned long start, t;
	int           x1, y1, x2, y2,
		w = tft.width(),
		h = tft.height();

	tft.fillScreen(BLACK);

	x1 = y1 = 0;
	y2 = h - 1;
	start = micros();
	for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
	x2 = w - 1;
	for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
	t = micros() - start; // fillScreen doesn't count against timing

	tft.fillScreen(BLACK);

	x1 = w - 1;
	y1 = 0;
	y2 = h - 1;
	start = micros();
	for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
	t += micros() - start;

	tft.fillScreen(BLACK);

	x1 = 0;
	y1 = h - 1;
	y2 = 0;
	start = micros();
	for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
	x2 = w - 1;
	for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
	t += micros() - start;

	tft.fillScreen(BLACK);

	x1 = w - 1;
	y1 = h - 1;
	y2 = 0;
	start = micros();
	for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);

	return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
	unsigned long start;
	int           x, y, w = tft.width(), h = tft.height();

	tft.fillScreen(BLACK);
	start = micros();
	for (y = 0; y < h; y += 5) tft.drawFastHLine(0, y, w, color1);
	for (x = 0; x < w; x += 5) tft.drawFastVLine(x, 0, h, color2);

	return micros() - start;
}


unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
	unsigned long start, t = 0;
	int           n, i, i2,
		cx = tft.width() / 2 - 1,
		cy = tft.height() / 2 - 1;

	tft.fillScreen(BLACK);
	n = min(tft.width(), tft.height());
	for (i = n; i > 0; i -= 6) {
		i2 = i / 2;
		start = micros();
		tft.fillRect(cx - i2, cy - i2, i, i, color1);
		t += micros() - start;
		// Outlines are not included in timing results
		tft.drawRect(cx - i2, cy - i2, i, i, color2);
	}

	return t;
}


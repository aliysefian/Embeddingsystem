// ***************************************************************************
// ***************************************************************************
// **** NETATMO REMOTE DISPLAY ****
// ********************************
// **** 21 MAI 2016 ***************
// **** 30 OCT 2016 ***************
// ***************************************************************************
// ***************************************************************************

// *** OTHER FILES FOR THE PROJECT ***
#include "netatmo_defines.h"
#include "netatmo_misc.h"
#include "netatmo_internet.h"
#include "netatmo_display.h"
#include "netatmo_misc.h"

/*
ADAFRUIT Resistve Touch 2.8 screen | pins 9=TFT dc / 10=TFT CS / D11 / D12 / D13 / D3 (backlight) )
Ethernet shield pins 10, 11, 12, 13
*/

// *** INCLUDES ***
#include <SPI.h>
#include "Adafruit_GFX.h"		// graphic library
#include "Adafruit_ILI9341.h"	// screen
#include <Time.h>
#include <wire.h>

// ======== COLOR SCALES =======
/*int HC[7] = {0X2CFF, 0x2DFF, 0x2EFF, 0x2FFF, 0x2FFB, 0x2FF7, 0x2FF3}; // Humidity 7 colors Scale (green to blue)
int CO2C[17] = {0x7FEF, 0x8FEF, 0x9FEF, 0xAFEF, 0xBFEF, 0xCFEF, 0xDFEF, 0xEFEF, 0xFFEF, 0xFF6F, 0xFEEF, 0xFE6F, 0xFDEF, 0xFD6F, 0xFCEF, 0xFC6F, 0xFBEF}; // scale CO2e
int TC[33] = {0x7BFF, 0x7C7F, 0x7CFF, 0x7D7F, 0x7DFF, 0x7E7F, 0x7EFF, 0x7F7F, 0x7FFF, 0x7FFD, 0x7FFB, 0x7FF9, 0x7FF7, 0x7FF5, 0x7FF3, 0x7FF1, 0x7FEF, 0x8FEF, 0x9FEF, 0xAFEF, 0xBFEF, 0xCFEF, 0xDFEF, 0xEFEF, 0xFFEF, 0xFF6F, 0xFEEF, 0xFE6F, 0xFDEF, 0xFD6F, 0xFCEF, 0xFC6F, 0xFBEF}; // temp scale blue/cyan/green/yellow/orange/red + pressure
*/

// blue=0 - cyan=16 - green=32 - yellow=48 - red=64
int _Colors[65] = {0x7BFF, 0x7C3F, 0x7C7F, 0x7CBF, 0x7CFF, 0x7D3F, 0x7D7F, 0x7DBF, 0x7DFF, 0x7E3F, 0x7E7F, 0x7EBF, 0x7EFF,
	0x7F3F, 0x7F7F, 0x7FBF, 0x7FFF, 0x7FFE, 0x7FFD, 0x7FFC, 0x7FFB, 0x7FFA, 0x7FF9, 0x7FF8, 0x7FF7, 0x7FF6,
	0x7FF5, 0x7FF4, 0x7FF3, 0x7FF2, 0x7FF1, 0x7FF0, 0x7FEF, 0x87EF, 0x8FEF, 0x97EF, 0x9FEF, 0xA7EF, 0xAFEF,
	0xB7EF, 0xBFEF, 0xC7EF, 0xCFEF, 0xD7EF, 0xDFEF, 0xE7EF, 0xEFEF, 0xF7EF, 0xFFEF, 0xFFAF, 0xFF6F, 0xFF2F,
	0xFEEF, 0xFEAF, 0xFE6F, 0xFE2F, 0xFDEF, 0xFDAF, 0xFD6F, 0xFD2F, 0xFCEF, 0xFCAF, 0xFC6F, 0xFC2F, 0xFBEF
};


unsigned int Font_25x15 [15][25] {
	{2016,4080,8184,16380,15996,31806,30750,30750,61455,61455,61455,61455,61455,61455,61455,61455,61455,30750,30750,31806,15996,16380,8184,4080,2016}, //0
	{448,960,1984,4032,8128,16320,32704,32704,31680,29632,960,960,960,960,960,960,960,960,960,960,960,65535,65535,65535,65535}, //1
	{2016,8184,16380,32766,31806,63519,61455,61455,15,15,15,31,62,2046,8188,16376,32736,31744,63488,61440,61440,65535,65535,65535,65535}, //2
	{65535,65535,65535,65535,62,124,248,496,992,1016,1020,1022,62,31,15,15,15,61455,61455,63519,31806,32766,16380,8184,2016}, //3
	{480,480,480,480,480,61920,61920,61920,61920,61920,61920,61920,61920,61920,65535,65535,65535,65535,480,480,480,480,480,480,480}, //4
	{8190,8190,8190,8190,7680,15872,15872,15360,16368,16376,32764,32766,31806,31,15,15,15,63503,63503,63519,31806,32766,16380,8184,2016}, //5
	{1984,8176,16376,16380,31804,30720,63488,61440,63456,65528,65532,65534,64574,63519,61455,61455,61455,61455,61455,63519,31806,32766,16380,8184,2016}, //6
	{65535,65535,65535,65535,31,31,62,62,124,124,248,248,496,496,992,992,1984,1984,3968,3968,7936,7936,15872,15872,15872}, //7
	{960,4080,8184,8184,15996,15420,15420,15996,8184,8184,16380,32766,31806,63519,61455,61455,61455,61455,61455,63519,31806,32766,16380,8184,2016}, //8
	{2016,8184,16380,32766,31806,63519,61455,61455,61455,61455,61455,63519,31807,32767,16383,8191,2031,15,31,30,15422,16380,8188,4088,992}, //9
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28672,63488,63488,63488,28672}, // 10 = . (dot)
	{0,0,14336,31744,31744,31772,14396,124,248,496,992,1984,3968,7936,15872,31744,63488,61440,57456,248,248,248,112,0,0}, // 11 = % (percent)
	{0,0,0,0,0,0,0,0,0,0,0,65472,65472,65472,65472,0,0,0,0,0,0,0,0,0,0}, // 12 = - (minus)
	{30720,64512,52224,52224,64512,30720,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 13 = ° (degre)
	{49152,49152,49152,49152,49152,0,0,0,0,0,49152,49152,49152,49152,49152,0,0,0,0,0,49152,49152,49152,49152,49152}  // 14 = | (separator)
};

unsigned int Font_29x16 [11][29] {
	{16380,32766,65535,65535,65535,64575,63519,63519,63519,63519,63519,63519,63519,63519,63519,63519,63519,63519,63519,63519,63519,63519,63519,64575,65535,65535,65535,32766,16380}, //0
	{1984,4032,8128,16320,32704,65472,65472,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984}, //1
	{16380,32766,65535,65535,65535,64575,63519,31,31,31,31,63,16383,32767,65535,65534,65532,64512,63488,63488,63488,63488,63488,63488,65535,65535,65535,65535,65535}, //2
	{16380,32766,65535,65535,65535,64639,63551,63,63,63,63,127,1023,1022,1022,1022,1023,127,63,63,63,63,63551,64639,65535,65535,65535,32766,16380}, //3
	{63519,63519,63519,63519,63519,63519,63519,63519,63519,63519,63519,63519,65535,65535,65535,32767,16383,31,31,31,31,31,31,31,31,31,31,31,31}, //4
	{65535,65535,65535,65535,65535,63488,63488,63488,63488,63488,63488,63488,65535,65535,65535,65535,65535,63,31,31,31,31,63519,64575,65535,65535,65535,32766,16380}, //5
	{16380,32766,65535,65535,65535,64575,63519,63488,63488,63488,63488,64512,65532,65534,65535,65535,65535,64575,63519,63519,63519,63519,63519,64575,65535,65535,65535,32766,16380}, //6
	{65535,65535,65535,65535,65535,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31}, //7
	{16380,32766,65535,65535,65535,64575,63519,63519,63519,63519,63519,64575,65535,32766,32766,32766,65535,64575,63519,63519,63519,63519,63519,64575,65535,65535,65535,32766,16380}, //8
	{16380,32766,65535,65535,65535,64575,63519,63519,63519,63519,63519,64575,65535,65535,65535,32767,16383,31,31,31,31,31,63519,64575,65535,65535,65535,32766,16380}, //9
	{0,0,0,0,0,0,0,28672,63488,63488,63488,28672,0,0,0,0,0,28672,63488,63488,63488,28672,0,0,0,0,0,0,0} //:
};


//_________________________________________________________________________________________________
// _______ variable de la gestion wifi _______________
String _SSID = "wifissid";  // SSID du réseau Wi-Fi
String _PASS = "wifipassword"; // Mot de passe Wi-Fi
String host = "wwebsite.com";

String request;
int STEP = 0;
int RETURNCODE;
char c1;
char c2;
String _buf = "";
int _buf_length = 0;

unsigned long MILLIS_INTERNET = 99999999;
unsigned long MILLIS_TIME = 99999999;
unsigned long MILLIS_WEATHERMAP = 99999999;


time_t _CurrentTime;
time_t _PreviousTime=0;
time_t _WeatherTime=0;

int _CountDown = -1;
int _PreviousCountDown = -1;

boolean _FirstRun = true;

int _err_code;

String _http_header;


// ***************************************************
// ***************************************************
// Variables global to share Netamtmo values accross all modules (internet, display...)
// Actual and previous data used to refresh display only when necessary
// Value available in Text (read from the Internet flow / display) and Int for calculation.

// TEMPERATURE
// external Temp
int _Ext_Temp = -99;
int _Prev_Ext_Temp = -99;
String _Ext_Temp_Trend = "up";
String _Ext_Temp_Text;

// internal Temp (Living Room)
int _Int_Temp = -99;
int _Prev_Int_Temp = -99;
String _Int_Temp_Trend = "down";
String _Int_Temp_Text;

// internal Temp (bedroom)
int _Chbr_Temp = -99;
int _Prev_Chbr_Temp = -99;
String _Chbr_Temp_Trend = "down";
String _Chbr_Temp_Text;

// HUMIDITY
int _Ext_Humidity = 00;
int _Prev_Ext_Humidity = 00;
String _Ext_Humidity_Text;

int _Int_Humidity = 00;
int _Prev_Int_Humidity = 00;
String _Int_Humidity_Text;

int _Chbr_Humidity = 00;
int _Prev_Chbr_Humidity = 00;
String _Chbr_Humidity_Text;

// RAIN
float _Rain_Hour = 999;
float _Prev_Rain_Hour = 999;
String _Rain_Hour_Text;

float _Rain_Day = 999;
float _Prev_Rain_Day = 999;
String _Rain_Day_Text;

// CO2
int _Int_CO2 = 9999;
int _Prev_Int_CO2 = 9999;
String _Int_CO2_Text;

int _Chbr_CO2 = 9999;
int _Prev_Chbr_CO2 = 9999;
String _Chbr_CO2_Text;

// Pressure
int _Pressure = 9999;
int _Prev_Pressure = 9999;
String _Pressure_Text;
String _Pressure_Trend = "up";

// *****************************************************
// *****************************************************




//_________________________________________________________________________________________________

int _cursor = 0;
int _PressureHistory [48]; // to stroe the history of PRessure values read from Netatmo web site
int _TemperatureHistory [2][49]; // to store the min and max history of external temperature

//_________________________________________________________________________________________________
//---------initialisation des ecrans --------------
// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 TFTD = Adafruit_ILI9341(TFTD_CS, TFTD_DC);
Adafruit_ILI9341 TFTG = Adafruit_ILI9341(TFTG_CS, TFTG_DC);


// *****************************************************
// ************ all values for weather forecast ********
// *****************************************************
int _TodayTempMin;
int _TodayTempMax;
int _TomorrowTempMin;
int _TomorrowTempMax;
int _DayAfterTempMin;
int _DayAfterTempMax;

int _TodayWeather;
int _TomorrowWeather;
int _DayAfterWeather;

int _TodayCloud;
int _TomorrowCloud;
int _DayAfterCloud;

float _TodayRain;
float _TomorrowRain;
float _DayAfterRain;
// *****************************************************






//================================== SETUP ==================================
//================================== SETUP ==================================
//================================== SETUP ==================================
//================================== SETUP ==================================


void setup()
{
	// === INITIALISATOIN ===

	Serial.begin (115200);
	Serial1.begin(115200); // baudrate par défaut de l'ESP8266

	Serial.println ("===== SETUP STARTS =====");

	//--init LCD screens--
	TFTG.begin();
	TFTG.setRotation(3); //0 ou 1 ou 2 ou 3
	analogWrite (backlightTFTG, BKLITE_DAY); // set backlight to high (0-255)

	TFTD.begin();
	TFTD.setRotation(1); //0 ou 1 ou 2 ou 3
	analogWrite (backlightTFTD, BKLITE_DAY); // set backlight to high (0-255)

	TFTG.fillScreen(ILI9341_BLACK); // efface ecran
	TFTD.fillScreen(ILI9341_BLACK); // efface ecran

	// === SPLASH SCREEN ===

	ColorScaleShow ();
	_DrawGrid();
	_Bonjour ();
	
	TFTD.setTextSize(1);
	TFTD.setTextColor(0x0000);
	TFTD.setCursor(0, 203);
	TFTD.println ("*** NETATMO REMOTE DISPLAY DUAL SCREEN 2560 ***");
	TFTD.println ("Init : mai 2016 / updt : oct 2016");
	TFTD.println ("");
	
	STEP = 0;
	
	Serial.println(" ===== SETUP ENDS, LOOP STARTS =====");
}
// ==========================================================================





// ================================= LOOP ===================================
// ================================= LOOP ===================================
// ================================= LOOP ===================================
// ================================= LOOP ===================================

void loop()
{
	// ------------ EVERY 5 MINUTES --------------
	// ------------ CONNECT TO INTERNET AND RETRIEVE DATA -----------
	if ( millis() - MILLIS_INTERNET > 300000 )
	{
		_cursor=0;			// flush buffer
		_WifiConnect ();	// connects and gets data from internet

		if ( _buf_length  > 700 )
		{
			_GetTimeFromBuffer ();
			
			_DecodeData (); // instant data
			_DecodePressureFromBuffer(); // pressure history
			_DecodeTempHistoryFromBuffer (); // temp history

			// _DisplayToSerial ();

			if (_FirstRun)
			{
				TFTG.fillScreen(ILI9341_BLACK); // efface ecran
				TFTD.fillScreen(ILI9341_BLACK); // efface ecran
				_FirstRun = false;
			}
			
			_DisplayTemp();
			_DisplayHumidity();
			_DisplayCO2 ();
			_DisplayRain ();
			_DisplayPressure ();
			
			_DisplayPressureHistory ();
			_DisplayTemperatureHistory ();
		}

		MILLIS_INTERNET = millis ();
	}


	// ------------ EVERY 1 SECOND --------------
	// ------------ DISPLAY TIME ------------
	if (millis () - MILLIS_TIME > 1000) // every 1 sec
	{
		_DisplayTime ();
		_DisplayCountDown ();

		MILLIS_TIME = millis ();
	}


	// ------------ EVERY 1 HOUR --------------
	// ------------ DISPLAY TIME ------------
	if (millis () - MILLIS_WEATHERMAP > 3600000) // every 1 hour
	{
		_WifiConnect_WeatherMap();
		if ( _buf_length  > 700 )
		{
			_DecodeDataWeather();
			_DisplayWeather2 (X1, _TodayWeather, _TodayTempMin, _TodayTempMax, _TodayCloud, _TodayRain);
			_DisplayWeather2 (X2, _TomorrowWeather, _TomorrowTempMin, _TomorrowTempMax, _TomorrowCloud, _TomorrowRain);
			_DisplayWeather2 (X3, _DayAfterWeather, _DayAfterTempMin, _DayAfterTempMax, _DayAfterCloud, _DayAfterRain);
			
		}
		MILLIS_WEATHERMAP = millis ();
	}
	
}
// ================================================================================================



// -------------------------------------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// ------------ END OF STORY -----------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------


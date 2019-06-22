#include "netatmo_defines.h"

#include "Adafruit_ILI9341.h"	// screen
#include <Time.h>

#include "netatmo_external_vars.h"

//********************** NETATMO VALUES *************

int RainColorScale (int value);
int HumidityColorScale (int value);
int CO2ColorScale (int value);
int TempColorScale (int value);
int PressureColorScale (int value);

void ColorScaleShow (void);

void _Bonjour (void);
void _DisplayCountDown (void);
void _DisplayTime (void);
void _DrawGrid (void);

void _DisplayInternetStatus (int color);

void _DisplayWeather (void);
void _DisplayWeather2 (int offsetX, int Weather, int TempMin, int TempMax, int Cloud, float Rain);

void _DisplayTemp (void);
void _DisplayHumidity (void);
void _DispPercentsymbol (int x, int y);
void _DisplayCO2 (void);
void _DisplayPressure (void);

void _DisplayPressureHistory (void);
void _DisplayTemperatureHistory (void);
void _DisplayRain (void);
void _displayTrend (int xcoord, int ycoord, String trend);

void _WriteFont_25x15 (int display, int x_offset, int y_offset, int digit, int color);
void _PrintFont_25x15 (int display, int x_offset, int y_offset, String text, int color);
int _LengthFont_25x15 (String text);

void _WriteFont_29x16 (int display, int x_offset, int y_offset, int digit, int color);
void _PrintFont_29x16 (int display, int x_offset, int y_offset, String text, int color);

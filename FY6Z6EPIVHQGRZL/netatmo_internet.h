// netatmo_internet.h

#ifndef _NETATMO_INTERNET_h
#define _NETATMO_INTERNET_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "netatmo_defines.h"
#include "netatmo_internet.h"
#include "netatmo_misc.h"
#include "netatmo_display.h"
#include "netatmo_misc.h"

#endif

#include "Adafruit_ILI9341.h"	// screen
#include <Time.h>

#include "netatmo_external_vars.h"

void _WifiConnect (void);
void _GetTimeFromBuffer (void);
void _WifiConnect_WeatherMap (void);
void _ReadSerial1forData (void);

String _ReadBuffer (void);
String _ReadBufferPercent (void);

void _DecodeTempHistoryFromBuffer (void);
void _DecodePressureFromBuffer (void);
void _DecodeData (void);
void _DecodeDataWeather (void);

int _ReadSerial1 (void);

String RemoveDecimals (String Text);
String OnlyOneDecimal (String Text);
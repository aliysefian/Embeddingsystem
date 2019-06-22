// netatmo_misc.h

#include "arduino.h"
#include <Time.h>
#include "Adafruit_ILI9341.h"	// screen

#include "netatmo_external_vars.h"


time_t _StringToTimeT (String _TimeString);
void _SerialDisplayWeather (void);
void _DisplayToSerial (void);
void _DisplayToLCD (void);


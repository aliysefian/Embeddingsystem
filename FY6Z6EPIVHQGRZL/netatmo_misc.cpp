#include "netatmo_defines.h"
#include "netatmo_misc.h"
#include "Adafruit_ILI9341.h"	// screen

// -------------------------------------------------------------------

time_t _StringToTimeT (String _TimeString)
{
	time_t _ComputeTime = 1400000000;
	_ComputeTime = _ComputeTime + (_TimeString.substring(2,6)).toInt() * 10000;
	_ComputeTime = _ComputeTime + (_TimeString.substring(6,10)).toInt();
	return _ComputeTime;
}

// -------------------------------------------------------------------

void _SerialDisplayWeather (void)
{
	Serial.println ("======== weather map decode =========");

	Serial.print ("Time : ");
	Serial.print (year(_WeatherTime)); Serial.print("-");
	Serial.print (month(_WeatherTime)); Serial.print("-");
	Serial.print (day(_WeatherTime)); Serial.print("  ");
	Serial.print (hour(_WeatherTime)); Serial.print(":");
	Serial.println (minute(_WeatherTime));

	Serial.print ("Today Temp Min : "); Serial.println (_TodayTempMin);
	Serial.print ("Today Temp Max : "); Serial.println (_TodayTempMax);
	Serial.print ("Today Weather : "); Serial.println (_TodayWeather);
	Serial.print ("Today Cloud : "); Serial.println (_TodayCloud);
	Serial.print ("Today Rain : "); Serial.println (_TodayRain);
	
	Serial.print ("Tomorrow Temp Min : "); Serial.println (_TomorrowTempMin);
	Serial.print ("Tomorrow Temp Max : "); Serial.println (_TomorrowTempMax);
	Serial.print ("Tomorrow Weather : "); Serial.println (_TomorrowWeather);
	Serial.print ("Tomorrow Cloud : "); Serial.println (_TomorrowCloud);
	Serial.print ("Tomorrow Rain : "); Serial.println (_TomorrowRain);
	
	Serial.print ("Day After Tomorrow Temp Min : "); Serial.println (_DayAfterTempMin);
	Serial.print ("Day After Tomorrow Temp Max : "); Serial.println (_DayAfterTempMax);
	Serial.print ("Day After Tomorrow Weather : "); Serial.println (_DayAfterWeather);
	Serial.print ("Day After Tomorrow Cloud : "); Serial.println (_DayAfterCloud);
	Serial.print ("Day After Tomorrow Rain : "); Serial.println (_DayAfterRain);
	
	
	/*
	int _TomorrowTempMin;
	int _TomorrowTempMax;
	int _DayAfterTempMin;
	int _DayAfterTempMax;
	
	int _TomorrowWeather;
	int _DayAfterWeather;
	
	int _TomorrowCloud;
	int _DayAfterCloud;
	*/

	Serial.println ("---------------------------");

}

// -------------------------------------------------------------------
// commented for memory saving - left for debug just in case

void _DisplayToSerial (void)
{
	Serial.println ("--------------");

	Serial.print ("Ext temp : ");
	Serial.println (_Ext_Temp);

	Serial.print ("Ext temp trend : ");
	Serial.println (_Ext_Temp_Trend);

	Serial.print ("Ext Humidity : ");
	Serial.println (_Ext_Humidity);

	Serial.print ("Rain Hour : ");
	Serial.println (_Rain_Hour);

	Serial.print ("Rain Day : ");
	Serial.println (_Rain_Day);

	Serial.print ("Int temp : ");
	Serial.println (_Int_Temp);

	Serial.print ("Int temp trend : ");
	Serial.println (_Int_Temp_Trend);

	Serial.print ("Int Humidity : ");
	Serial.println (_Int_Humidity);

	Serial.print ("Pressure : ");
	Serial.println (_Pressure);

	Serial.print ("Pressure trend : ");
	Serial.println (_Pressure_Trend);

	Serial.print ("CO2 : ");
	Serial.println (_Int_CO2);

	Serial.print ("Chambre temp : ");
	Serial.println (_Chbr_Temp);

	Serial.print ("Chambre temp trend : ");
	Serial.println (_Chbr_Temp_Trend);

	Serial.print ("Chambre Humidity : ");
	Serial.println (_Chbr_Humidity);

	Serial.print ("Chambre CO2 : ");
	Serial.println (_Chbr_CO2);

	Serial.println ("--------------");
}

// -------------------------------------------------------------------

void _DisplayToLCD (void)
{
	TFTD.setTextSize(1);
	TFTD.setCursor(0, 0);
	TFTD.setTextColor (0xFFFF); //(white)
	TFTD.fillScreen(ILI9341_BLACK); // efface ecran


	TFTD.println ("--------------");

	TFTD.print ("Ext temp : ");
	TFTD.println (_Ext_Temp);

	TFTD.print ("Ext temp trend : ");
	TFTD.println (_Ext_Temp_Trend);

	TFTD.print ("Ext Humidity : ");
	TFTD.println (_Ext_Humidity);

	TFTD.println ("");

	TFTD.print ("Rain Hour : ");
	TFTD.println (_Rain_Hour);

	TFTD.print ("Rain Day : ");
	TFTD.println (_Rain_Day);

	TFTD.println ("");

	TFTD.print ("Pressure : ");
	TFTD.println (_Pressure);

	TFTD.print ("Pressure trend : ");
	TFTD.println (_Pressure_Trend);

	TFTD.println ("");

	TFTD.print ("Salon temp : ");
	TFTD.println (_Int_Temp);

	TFTD.print ("Salon temp trend : ");
	TFTD.println (_Int_Temp_Trend);

	TFTD.print ("Salon Humidity : ");
	TFTD.println (_Int_Humidity);

	TFTD.print ("Salon CO2 : ");
	TFTD.println (_Int_CO2);

	TFTD.println ("");

	TFTD.print ("Chambre temp : ");
	TFTD.println (_Chbr_Temp);

	TFTD.print ("Chambre temp trend : ");
	TFTD.println (_Chbr_Temp_Trend);

	TFTD.print ("Chambre Humidity : ");
	TFTD.println (_Chbr_Humidity);

	TFTD.print ("Chambre CO2 : ");
	TFTD.println (_Chbr_CO2);

	TFTD.println ("--------------");
}

// -------------------------------------------------------------------


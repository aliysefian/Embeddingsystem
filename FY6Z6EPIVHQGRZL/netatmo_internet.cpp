#include "netatmo_defines.h"
#include "netatmo_internet.h"

#include "netatmo_misc.h"
#include "netatmo_display.h"
#include "netatmo_misc.h"

#include "Adafruit_ILI9341.h"	// screen
#include <Time.h>

// ----------------------------------------
void _WifiConnect (void)
{
	Serial.println("============ START WIFI PROCEDURE ==============");

	// Echo off
	Serial.println("Echo off...");
	Serial1.println("ATE0");
	_ReadSerial1 ();

	// on va se connecter à un réseau existant, donc on passe en mode station
	Serial.println("Init to Wifi Client...");
	Serial1.println("AT+CWMODE_CUR=1");
	_ReadSerial1 ();

	// on se connecte au réseau
	Serial.println ("Connects to WiFi network...");
	Serial1.println ("AT+CWJAP_DEF=\"" + _SSID + "\",\"" + _PASS + "\"");
	if ( _ReadSerial1 () == 1 )
	{
		_DisplayInternetStatus (GREEN);
	}
	else
	{
		_DisplayInternetStatus (RED);
	}

	//      Serial1.println("AT+CIPSTA_CUR?");

	// mode "connexions unique"
	Serial.println ("Set to only one connection...");
	Serial1.println ("AT+CIPMUX=0");
	if ( _ReadSerial1 () == 1 )
	{
		_DisplayInternetStatus (GREEN);
	}
	else
	{
		_DisplayInternetStatus (RED);
	}

	// on se connecte à notre serveur en TCP + keepalive = 1000
	Serial.println ("TCP connection to website...");
	Serial1.println ("AT+CIPSTART=\"TCP\",\"www.yoursite.com\",80,1000");
	if ( _ReadSerial1 () == 1 )
	{
		_DisplayInternetStatus (GREEN);
	}
	else
	{
		_DisplayInternetStatus (RED);
	}

	// build GET request and SEND request info
	request  = "GET /netatmo201611a.php HTTP/1.1\r\n";
	request += "Host:" + host + "\r\n";
	request += "User-Agent: Arduino\r\n";
	request += "Accept: text/html\r\n";
	request += "Connection: close\r\n";
	// on donne la taille de la requête qu'on va envoyer, en ajoutant 2 car
	// println ajouter 2 caractères à la fin "\r" et "\n"
	Serial1.println("AT+CIPSEND=" + String(request.length() + 2));
	delay (100);
	Serial1.println(request);

	_ReadSerial1forData ();
	
	Serial.println (_buf_length);
	if ( _buf_length > 700 )
	{
		_DisplayInternetStatus (GREEN);
	}
	else
	{
		_DisplayInternetStatus (RED);
	}
	
	/*
	Serial.println ("~~~~ BUFFER ~~~~~");
	Serial.println (_buf);
	Serial.println ("~~~~ EOBUFF ~~~~~");
	*/
}
// ----------------------------------------

// ----------------------------------------
void _GetTimeFromBuffer (void)
{
	int _buf_cursor = 0;	// pointer used to parse the buffer
	int _monthInt;			// to store month as a INT
	int _dowInt;			// to store day of week as int
	int _DST=0;				// daylightsavingtime
	
	/*
	Serial.print ("===== TIME STATUS : ");
	if (timeStatus () == timeNotSet )
	{
	Serial.println ("Time not set !!! =====");
	}
	else if ( (timeStatus () == timeSet ) )
	{
	Serial.println ("Time is OK !!! =====");
	}
	*/
	
	// ======== search for 'DATE' within buffer
	while ( _buf_cursor + 4 < _buf_length )
	{
		if ( _buf.substring (_buf_cursor, _buf_cursor + 4) == "Date")
		{
			Serial.println ("=====> Date found in buffer <=====");
			break;
		}
		_buf_cursor++;
	}
	
	// ======== Store Dates values
	if ( _buf_cursor + 32 < _buf_length ) // check for complete data
	{
		String _dow = _buf.substring (_buf_cursor + 6, _buf_cursor + 9);
		String _day = _buf.substring (_buf_cursor + 11, _buf_cursor + 13);
		String _month = _buf.substring (_buf_cursor + 14, _buf_cursor + 17);
		String _year = _buf.substring (_buf_cursor + 18, _buf_cursor + 22);
		String _hour = _buf.substring (_buf_cursor + 23, _buf_cursor + 25);
		String _min = _buf.substring (_buf_cursor + 26, _buf_cursor + 28);
		String _sec = _buf.substring (_buf_cursor + 29, _buf_cursor + 31);
		
		/*
		// displays strings variable to check for proper string matching - use for debug
		Serial.println ("=====> STRING TIME IS : ");
		Serial.print("*"); Serial.print (_dow); Serial.println ("*");
		Serial.print("*"); Serial.print (_day); Serial.println ("*");
		Serial.print("*"); Serial.print (_month); Serial.println ("*");
		Serial.print("*"); Serial.print (_year); Serial.println ("*");
		Serial.print("*"); Serial.print (_hour); Serial.println ("*");
		Serial.print("*"); Serial.print (_min); Serial.println ("*");
		Serial.print("*"); Serial.print (_sec); Serial.println ("*");
		*/
		
		if (_dow == "Sun") _dowInt=1;
		if (_dow == "Mon") _dowInt=2;
		if (_dow == "Tue") _dowInt=3;
		if (_dow == "Wed") _dowInt=4;
		if (_dow == "Thu") _dowInt=5;
		if (_dow == "Fri") _dowInt=6;
		if (_dow == "Sat") _dowInt=7;
		
		if (_month == "Jan") _monthInt=1;
		if (_month == "Feb") _monthInt=2;
		if (_month == "Mar") _monthInt=3;
		if (_month == "Apr") _monthInt=4;
		if (_month == "May") _monthInt=5;
		if (_month == "Jun") _monthInt=6;
		if (_month == "Jul") _monthInt=7;
		if (_month == "Aug") _monthInt=8;
		if (_month == "Sep") _monthInt=9;
		if (_month == "Oct") _monthInt=10;
		if (_month == "Nov") _monthInt=11;
		if (_month == "Dec") _monthInt=12;
		
		// manage DST
		// in France , last sunday of march (+1 hour) and last sunday of october (-1 hour)
		if ( _monthInt < 4 ) _DST=1;																	// jan and feb and march -> winter
		if ( (_monthInt ==  4) && (_day.toInt() > 23) && ( (_day.toInt() - _dowInt) > 22) ) _DST=2;		// after last sun of march -> summer
		if ( _monthInt > 3 ) _DST=2;																	// after march -> summer
		if ( (_monthInt == 10) && (_day.toInt() > 24) && ( (_day.toInt() - _dowInt) > 23) ) _DST=1;		// after last sun of october -> winter
		if ( _monthInt > 10) _DST=1;																	// after oct -> winter
		
		/* used for debug DST calculation
		Serial.println ("------------ DST------------");
		Serial.println (_buf.substring (_buf_cursor + 6, _buf_cursor + 31));
		Serial.println (_monthInt);
		Serial.println (_day.toInt());
		Serial.println (_dowInt);
		Serial.println (_DST);
		Serial.println ("------------EoDST-----------");
		*/
		
		// Serial.println (_DST);
		
		setTime ( _hour.toInt(), _min.toInt(), _sec.toInt(), _day.toInt(), _monthInt, (byte)(_year.toInt()) );
		adjustTime (_DST * 3600); // add 1 or 2 hour(s) according DST
		_CurrentTime = now();

		/*
		// displays time stored and managed by arduino - use for debug
		Serial.println ("=====> ARDUINO TIME IS : ");
		Serial.print("*"); Serial.print ( hour(_CurrentTime) ); Serial.println ("*");
		Serial.print("*"); Serial.print ( minute(_CurrentTime) ); Serial.println ("*");
		Serial.print("*"); Serial.print ( second(_CurrentTime) ); Serial.println ("*");
		Serial.print("*"); Serial.print ( weekday(_CurrentTime) ); Serial.println ("*");
		Serial.print("*"); Serial.print ( day(_CurrentTime) ); Serial.println ("*");
		Serial.print("*"); Serial.print ( month(_CurrentTime) ); Serial.println ("*");
		Serial.print("*"); Serial.print ( year(_CurrentTime) ); Serial.println ("*");
		*/
	}
}


// ----------------------------------------
void _WifiConnect_WeatherMap (void)
{
	Serial.println("============ START WIFI PROCEDURE ==============");

	// Echo off
	Serial.println("Echo off...");
	Serial1.println("ATE0");
	_ReadSerial1 ();

	// on va se connecter à un réseau existant, donc on passe en mode station
	Serial.println("Init to Wifi Client...");
	Serial1.println("AT+CWMODE_CUR=1");
	_ReadSerial1 ();

	// on se connecte au réseau
	Serial.println ("Connects to WiFi network...");
	Serial1.println ("AT+CWJAP_DEF=\"" + _SSID + "\",\"" + _PASS + "\"");
	if ( _ReadSerial1 () == 1 )
	{
		_DisplayInternetStatus (GREEN);
	}
	else
	{
		_DisplayInternetStatus (RED);
	}

	//      Serial1.println("AT+CIPSTA_CUR?");

	// mode "connexions unique"
	Serial.println ("Set to only one connection...");
	Serial1.println ("AT+CIPMUX=0");
	if ( _ReadSerial1 () == 1 )
	{
		_DisplayInternetStatus (GREEN);
	}
	else
	{
		_DisplayInternetStatus (RED);
	}

	// on se connecte à notre serveur en TCP + keepalive = 1000
	Serial.println ("TCP connection to website...");
	Serial1.println ("AT+CIPSTART=\"TCP\",\"www.yoursite.com\",80,1000");
	if ( _ReadSerial1 () == 1 )
	{
		_DisplayInternetStatus (GREEN);
	}
	else
	{
		_DisplayInternetStatus (RED);
	}

	// build GET request and SEND request info
	request  = "GET /openweathermap.php HTTP/1.1\r\n";
	request += "Host:" + host + "\r\n";
	request += "User-Agent: Arduino\r\n";
	request += "Accept: text/html\r\n";
	request += "Connection: close\r\n";
	// on donne la taille de la requête qu'on va envoyer, en ajoutant 2 car
	// println ajouter 2 caractères à la fin "\r" et "\n"
	Serial1.println("AT+CIPSEND=" + String(request.length() + 2));
	delay (100);
	Serial1.println(request);

	_ReadSerial1forData ();
	
	Serial.println (_buf_length);
	if ( _buf_length > 700 )
	{
		_DisplayInternetStatus (GREEN);
	}
	else
	{
		_DisplayInternetStatus (RED);
	}
	
	/*
	Serial.println ("~~~~ BUFFER ~~~~~");
	Serial.println (_buf);
	Serial.println ("~~~~ EOBUFF ~~~~~");
	*/
}
// ----------------------------------------


// ---------------------------------------
// -- Read GET request
// -- Impossible to parse for content at the same time because we miss characters
// -- Parsing is done when full buffer was received
// -- A 10 sec timeframe allowed to receive all data
// -- It just fills a buffer which is then decoded by _ProcessData
// --
void _ReadSerial1forData (void)
{
	unsigned long MILLIS = millis ();
	_buf = "";
	_buf_length = 0;

	while (1)
	{
		//timeout = 10sec
		if ( millis() - MILLIS > 10000 )
		{
			Serial.println ("Timeout 10 secondes");
			break;
		}

		// check for data and store it into buffer
		if ( Serial1.available () )
		{
			c1 = Serial1.read();
			_buf += c1;
			_buf_length++;
		}
	}
}

//-------------------------------------------------------------------

String _ReadBuffer (void)
{
	/*
	Serial.print ("READBUFFER ROUTINE : ");
	Serial.print (_buf.substring (_cursor, _cursor + 10));
	Serial.print(" : ");
	*/
	String _local_buffer = "";
	
	_cursor++;
	
	while (_buf[_cursor] != '#')
	{
		_local_buffer += _buf[_cursor];
		_cursor++;
	}
	
	//Serial.println (_local_buffer);
	return _local_buffer;
}

//-------------------------------------------------------------------

String _ReadBufferPercent (void)
{
	/*
	Serial.print ("READBUFFER ROUTINE : ");
	Serial.print (_buf.substring (_cursor, _cursor + 10));
	Serial.print(" : ");
	*/
	String _local_buffer = "";
	
	_cursor++;
	
	while (_buf[_cursor] != '%')
	{
		_local_buffer += _buf[_cursor];
		_cursor++;
	}
	
	//Serial.println (_local_buffer);
	return _local_buffer;
}

//-------------------------------------------------------------------

void _DecodePressureFromBuffer (void)
{
	_cursor = 0;
	
	_ReadBufferPercent (); // goto first %
	for ( int i=0; i<48; i++)
	{
		_PressureHistory[i] = _ReadBufferPercent().toInt();
		/*		Serial.print ("->");
		Serial.print( i );
		Serial.print ("<-->");
		Serial.println (_PressureHistory[i]);*/
	}
}

//-------------------------------------------------------------------

void _DecodeTempHistoryFromBuffer (void)
{
	_TemperatureHistory [0][48] = -999; // init for max temp
	_TemperatureHistory [1][48] = 999; // init for min temp

	_ReadBufferPercent (); // gotonext % (second line /!\ _cursor is not reset to 0 !!!
	for (int j=0; j<2; j++) // j==0 -> max temp  || j==1 -> min temp
	{
		for ( int i=0; i<48; i++)
		{
			int value = _ReadBufferPercent().toInt();
			_TemperatureHistory[j][i]=value;
			if ((j==0) && (value > _TemperatureHistory[j][48]))  _TemperatureHistory[j][48] = value; // store max value
			if ((j==1) && (value < _TemperatureHistory[j][48]))  _TemperatureHistory[j][48] = value; // store min value
			/*	Serial.print ("->");
			Serial.print( i );
			Serial.print ("<-->");
			Serial.println (_TemperatureHistory[j][i]); */
		}
		_ReadBufferPercent (); // gotonext %
	}
	Serial.print ("Max Temp Value =");
	Serial.println (_TemperatureHistory[0][48]);
	Serial.print ("Min Temp Value =");
	Serial.println (_TemperatureHistory[1][48]);
}

//-------------------------------------------------------------------

void _DecodeData (void)
{
	String _buffer="";
	_cursor=0; // flush buffer
	
	// BUFFER SAMPLE
	// #EXT#21.9#up#74#PLUVIO#0#0.36246992782678#0#SALON#47#24.2#down#58#1020.7#up#600#CHAMBRE#25#down#55#462#

	_buffer = _ReadBuffer(); // drop HEADER
	
	_buffer = _ReadBuffer(); // drop #EXT

	// ext temp
	_buffer = _ReadBuffer();
	_Ext_Temp_Text = RemoveDecimals (_buffer);
	_Ext_Temp = _Ext_Temp_Text.toInt();
	_Ext_Temp_Text = _Ext_Temp_Text + "d";

	// ext temp trend
	_buffer = _ReadBuffer();
	_Ext_Temp_Trend = _buffer;

	// ext humidity
	_Ext_Humidity_Text = _ReadBuffer();
	_Ext_Humidity = _Ext_Humidity_Text.toInt();

	// drop #PLUVIO
	_buffer = _ReadBuffer();

	// rain inst
	_buffer = _ReadBuffer(); // drop value because not used

	// rain Day
	_buffer = _ReadBuffer();
	_Rain_Day_Text = OnlyOneDecimal(_buffer);
	_Rain_Day = _Rain_Day_Text.toFloat();

	// rain hour
	_buffer = _ReadBuffer();
	_Rain_Hour_Text = OnlyOneDecimal(_buffer);
	_Rain_Hour = _Rain_Hour_Text.toFloat();

	// drop #SALON
	_buffer = _ReadBuffer();

	// noise
	_buffer = _ReadBuffer(); // drop noise

	// int temp
	_buffer = _ReadBuffer();
	_Int_Temp_Text = RemoveDecimals (_buffer);
	_Int_Temp = _Int_Temp_Text.toInt();
	_Int_Temp_Text = _Int_Temp_Text + "d";

	// int temp trend
	_buffer = _ReadBuffer();
	_Int_Temp_Trend = _buffer;

	// Int humidity
	_Int_Humidity_Text = _ReadBuffer();
	_Int_Humidity = _Int_Humidity_Text.toInt();
	_Int_Humidity_Text = _Int_Humidity_Text + "%";

	// pressure
	_buffer = _ReadBuffer();
	_Pressure_Text = RemoveDecimals(_buffer);
	_Pressure = _Pressure_Text.toInt();

	// Pressure trend
	_buffer = _ReadBuffer();
	_Pressure_Trend = _buffer;

	// int CO2
	_Int_CO2_Text = _ReadBuffer();
	_Int_CO2 = _Int_CO2_Text.toInt();

	// drop #CHAMBRE
	_buffer = _ReadBuffer();

	// int temp
	_buffer = _ReadBuffer();
	_Chbr_Temp_Text = RemoveDecimals (_buffer);
	_Chbr_Temp = _Chbr_Temp_Text.toInt();
	_Chbr_Temp_Text = _Chbr_Temp_Text + "d";

	// int temp trend
	_buffer = _ReadBuffer();
	_Chbr_Temp_Trend = _buffer;

	// Chbr humidity
	_Chbr_Humidity_Text = _ReadBuffer();
	_Chbr_Humidity = _Chbr_Humidity_Text.toInt();
	_Chbr_Humidity_Text = _Chbr_Humidity_Text + "%";

	// int CO2
	_Chbr_CO2_Text = _ReadBuffer();
	_Chbr_CO2 = _Chbr_CO2_Text.toInt();

}

//-------------------------------------------------------------------
// Remove everything after (and including) the dot in a string that looks like a float // ex : 1024.5 becomes 1024
String RemoveDecimals (String Text)
{
	for (int i=0; i < Text.length(); i++)
	{
		if (Text[i]=='.') return Text.substring(0,i);
	}
	return Text;
}

//-------------------------------------------------------------------
// Remove everything after (and including) the dot in a string that looks like a float // ex : 1024.5 becomes 1024
String OnlyOneDecimal (String Text)
{
	for (int i=0; i < Text.length(); i++)
	{
		if (Text[i]=='.') return Text.substring(0,i+2);
	}
	return Text;
}

// -------------------------------------------------------------------

void _DecodeDataWeather (void)
{
	// we take the worst weather situation for each day

	String _buffer="";
	_cursor=0; // flush buffer
	
	// ************ TODAY **
	
	_buffer = _ReadBuffer(); // drop first #
	
	// first tag time
	_buffer = _ReadBuffer();
	_WeatherTime = _StringToTimeT (_buffer);
	// first temp
	_buffer = _ReadBuffer();
	_TodayTempMin = _buffer.toInt();
	_TodayTempMax = _buffer.toInt();
	//first weather
	_buffer = _ReadBuffer();
	_TodayWeather = _buffer.substring(0,2).toInt();
	//first cloud
	_buffer = _ReadBuffer();
	_TodayCloud = _buffer.toInt();
	//first rain
	_buffer = _ReadBuffer();
	_TodayRain = _buffer.toFloat();
	
	_SerialDisplayWeather();
	
	// Read all other for current day
	int j = (int) ( (21 - hour(_WeatherTime) ) / 3);
	for (int i=0; i<j; i++)
	{
		_buffer = _ReadBuffer(); // timetag
		_WeatherTime = _StringToTimeT (_buffer);
		
		_buffer = _ReadBuffer(); //temp
		int t = _buffer.toInt();
		if (t<_TodayTempMin) _TodayTempMin = t;
		if (t>_TodayTempMax) _TodayTempMax = t;
		
		_buffer = _ReadBuffer(); //weather
		int w = _buffer.substring(0,2).toInt();
		if (w>_TodayWeather) _TodayWeather = w;
		
		_buffer = _ReadBuffer(); //cloud
		int c = _buffer.toInt();
		if (c>_TodayCloud) _TodayCloud = c;

		_buffer = _ReadBuffer(); //rain
		float r = _buffer.toFloat();
		_TodayRain = _TodayRain + r;
		
		_SerialDisplayWeather();
	}

	// ************ TOMORROW **

	// first tag time
	_buffer = _ReadBuffer();
	_WeatherTime = _StringToTimeT (_buffer);
	// first temp
	_buffer = _ReadBuffer();
	_TomorrowTempMin = _buffer.toInt();
	_TomorrowTempMax = _buffer.toInt();
	//first weather
	_buffer = _ReadBuffer();
	_TomorrowWeather = _buffer.substring(0,2).toInt();
	//first cloud
	_buffer = _ReadBuffer();
	_TomorrowCloud = _buffer.toInt();
	//first rain
	_buffer = _ReadBuffer();
	_TomorrowRain = _buffer.toFloat();

	
	_SerialDisplayWeather();
	
	// Read all other for current day
	j = (int) ( (21 - hour(_WeatherTime) ) / 3);
	for (int i=0; i<j; i++)
	{
		_buffer = _ReadBuffer(); // timetag
		_WeatherTime = _StringToTimeT (_buffer);
		
		_buffer = _ReadBuffer(); //temp
		int t = _buffer.toInt();
		if (t<_TomorrowTempMin) _TomorrowTempMin = t;
		if (t>_TomorrowTempMax) _TomorrowTempMax = t;
		
		_buffer = _ReadBuffer(); //weather
		int w = _buffer.substring(0,2).toInt();
		if (w>_TomorrowWeather) _TomorrowWeather = w;
		
		_buffer = _ReadBuffer(); //cloud
		int c = _buffer.toInt();
		if (c>_TomorrowCloud) _TomorrowCloud = c;

		_buffer = _ReadBuffer(); //rain
		float r = _buffer.toFloat();
		_TomorrowRain = _TomorrowRain + r;
		
		
		_SerialDisplayWeather();
	}

	// ************ DAY AFTER TOMORROW **

	// first tag time
	_buffer = _ReadBuffer();
	_WeatherTime = _StringToTimeT (_buffer);
	// first temp
	_buffer = _ReadBuffer();
	_DayAfterTempMin = _buffer.toInt();
	_DayAfterTempMax = _buffer.toInt();
	//first weather
	_buffer = _ReadBuffer();
	_DayAfterWeather = _buffer.substring(0,2).toInt();
	//first cloud
	_buffer = _ReadBuffer();
	_DayAfterCloud = _buffer.toInt();
	//first rain
	_buffer = _ReadBuffer();
	_DayAfterRain = _buffer.toFloat();
	
	_SerialDisplayWeather();
	
	// Read all other for current day
	j = (int) ( (21 - hour(_WeatherTime) ) / 3);
	for (int i=0; i<j; i++)
	{
		_buffer = _ReadBuffer(); // timetag
		_WeatherTime = _StringToTimeT (_buffer);
		
		_buffer = _ReadBuffer(); //temp
		int t = _buffer.toInt();
		if (t<_DayAfterTempMin) _DayAfterTempMin = t;
		if (t>_DayAfterTempMax) _DayAfterTempMax = t;
		
		_buffer = _ReadBuffer(); //weather
		int w = _buffer.substring(0,2).toInt();
		if (w>_DayAfterWeather) _DayAfterWeather = w;
		
		_buffer = _ReadBuffer(); //cloud
		int c = _buffer.toInt();
		if (c>_DayAfterCloud) _DayAfterCloud = c;
		
		_buffer = _ReadBuffer(); //rain
		float r = _buffer.toFloat();
		_DayAfterRain = _DayAfterRain + r;
		
		_SerialDisplayWeather();
	}

}

// -------------------------------------------------------------------
// -- Simple Read Routine on ESP8266
// -- Used to check AT commands are properly processed
// -- It reads and wait for OK or FAIL or Timeout
// --
int _ReadSerial1 (void)
{
	unsigned long MILLIS = millis ();

	while (1)
	{
		if ( millis() - MILLIS > 10000 ) //timeout=10sec
		{
			Serial.println ("Timeout 10 secondes");
			return -1;
		}

		delay (25); // necessary, otherwise it loops too fast and doesn't work
		c1 = Serial1.read();
		// Serial.print ("***"); // uncomment for debug
		// Serial.println (c1);

		if ( (c1 == 'K') && (c2 == 'O') ) //OK
		{
			Serial.println ("==OK==");
			return 1;
		}
		else
		{
			c2 = c1;
		}

		if ( (c1 == 'I') && (c2 == 'L') ) //FAIL
		{
			Serial.println ("==FAIL==");
			return -1;
		}
		else
		{
			c2 = c1;
		}
	}
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
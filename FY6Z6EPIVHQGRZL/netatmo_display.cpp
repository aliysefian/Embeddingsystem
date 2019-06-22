// -------------------------------------------------------------------
// -------------------------------------------------------------------
// -- DISPLAY.cpp --> All display related functions ------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------

#include "netatmo_defines.h"
#include "netatmo_display.h"
#include "netatmo_external_vars.h"

#include "Adafruit_ILI9341.h"	// screen
#include <Time.h>

// -------------------------------------------------------------------
// -------------------------------------------------------------------
// --------------- SPLASH SCREEN -------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------

void ColorScaleShow (void)
{
	TFTG.fillScreen(ILI9341_BLACK); // efface ecran
	TFTD.fillScreen(ILI9341_BLACK); // efface ecran

	// === Temp 65 color scale ===
	for (int i = 0; i < 65; i++)
	{
		TFTG.fillRect ( i * 5, 0, 5, 240, _Colors[i] );
		TFTD.fillRect ( i * 5, 0, 5, 240, _Colors[i] );
	}
}

// -------------------------------------------------------------------

void _Bonjour (void)
{
	TFTG.setTextColor (0x0000);
	TFTD.setTextColor (0x0000);
	TFTG.setTextSize (12);
	TFTD.setTextSize (12);
	TFTG.setCursor (60, 80);
	TFTG.print ("BON");
	TFTD.setCursor (25, 80);
	TFTD.print ("JOUR");
}

// -------------------------------------------------------------------

void _DrawGrid (void)
{
	for (int x = 0; x < 320; x = x + 20)
	{
		TFTG.drawLine (x, 0, x, 239, 0x29A5);
		TFTD.drawLine (x, 0, x, 239, 0x29A5);
	}
	for (int y = 0; y < 240; y = y + 20)
	{
		TFTG.drawLine (0, y, 319, y, 0x29A5);
		TFTD.drawLine (0, y, 319, y, 0x29A5);
	}
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------
// --------------- LEFT SCREEN ---------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------

// 240 pix height
// 4 lines 228 pixels -> 57 pixels each
// 3 separators of 4 pixels
//  --
// 5 pix top margin
// 7 pix title size 1
// 10 pix middle margin
// 25 pix text new font
// 10 pix bottom margin
// ==> total = 57

// -------------------------------------------------------------------

void _displayTrend (int xcoord, int ycoord, String trend) //upper left corner of the display zone
{
	// no symbol if trend is STABLE
	if (trend == "up") // UP
	{
		TFTG.fillTriangle(xcoord+5, ycoord + 40, xcoord + 18, ycoord + 40, xcoord + 12, ycoord+30, 0x0000);
	}
	if (trend == "down") // DOWN
	{
		TFTG.fillTriangle(xcoord+5, ycoord+30, xcoord + 18, ycoord+30, xcoord + 12, ycoord + 40, 0x0000);
	}
}

// -------------------------------------------------------------------

void _DisplayTemp (void)
{
	int ycoord = 0;
	int height = 57;

	// ======OUTSIDE=====
	if (_Prev_Ext_Temp != _Ext_Temp)
	{
		//------background outside---
		TFTG.fillRect (X1, 0, 104, height, TempColorScale (_Ext_Temp));

		// ----- title
		TFTG.setTextSize (1);
		TFTG.setCursor (X1 + 5, 5);
		TFTG.print ("Temp. Exterieure");

		// ----- ext temp
		int textposx = X1 + XADJ + ( 104 - _LengthFont_25x15(_Ext_Temp_Text) ) / 2 ;
		_PrintFont_25x15(0, textposx, ycoord+22, _Ext_Temp_Text, 0x0000);

		// ---- trend
		_displayTrend (X1, ycoord, _Ext_Temp_Trend);
		
		_Prev_Ext_Temp = _Ext_Temp;
	}

	// ======INSIDE SALON=====
	if (_Prev_Int_Temp != _Int_Temp)
	{
		//------background outside---
		TFTG.fillRect (X2, 0, 104, height, TempColorScale (_Int_Temp));

		// ----- title
		TFTG.setTextSize (1);
		TFTG.setCursor (X2 + (int)((104-65)/2), 5);
		TFTG.print ("Temp. Salon"); //11x5+10=65 pixels

		// ----- ext temp
		int textposx = X2 + XADJ + ( 104 - _LengthFont_25x15(_Int_Temp_Text) ) / 2 ;
		_PrintFont_25x15(0, textposx, ycoord+22, _Int_Temp_Text, 0x0000);

		// ---- trend
		_displayTrend (X2, ycoord, _Int_Temp_Trend);
		
		_Prev_Int_Temp = _Int_Temp;
	}

	// ====== INSIDE CHAMBRE=====
	if (_Prev_Chbr_Temp != _Chbr_Temp)
	{
		//------background outside---
		TFTG.fillRect (X3, 0, 104, height, TempColorScale (_Chbr_Temp));

		// ----- title
		TFTG.setTextSize (1);
		TFTG.setCursor (X3 + (int)((104-77)/2), 5);
		TFTG.print ("Temp. Chambre"); //13x5+12=77 pixels

		// ----- ext temp
		int textposx = X3 + XADJ + ( 104 - _LengthFont_25x15(_Chbr_Temp_Text) ) / 2 ;
		_PrintFont_25x15(0, textposx, ycoord+22, _Chbr_Temp_Text, 0x0000);

		// ---- trend
		_displayTrend (X3, ycoord, _Chbr_Temp_Trend);
		
		_Prev_Chbr_Temp = _Chbr_Temp;
	}
}

// -------------------------------------------------------------------

void _DisplayHumidity (void)
{
	int ycoord = 57+4;
	int height = 57;

	// ======OUTSIDE=====
	if (_Prev_Ext_Humidity != _Ext_Humidity)
	{
		//------background outside---
		TFTG.fillRect (X1, ycoord, 104, height, HumidityColorScale (_Ext_Humidity));

		// ----- title
		TFTG.setTextSize (1);
		TFTG.setCursor (X1 + 5, ycoord+5);
		TFTG.print ("Humi. Exterieure");

		// ----- ext humi
		_Ext_Humidity_Text = _Ext_Humidity_Text + "%";
		int textposx = X1 + XADJ + ( 104 - _LengthFont_25x15(_Ext_Humidity_Text)) / 2 ;
		_PrintFont_25x15(0, textposx, ycoord+22, _Ext_Humidity_Text, 0x0000);

		_Prev_Ext_Humidity = _Ext_Humidity;
	}

	// ======INSIDE SALON=====
	if (_Prev_Int_Humidity != _Int_Humidity)
	{
		//------background outside---
		TFTG.fillRect (X2, ycoord, 104, height, HumidityColorScale (_Int_Humidity));

		// ----- title
		TFTG.setTextSize (1);
		TFTG.setCursor (X2 + (int)((104-65)/2), ycoord+5);
		TFTG.print ("Humi. Salon"); //11x5+10=65 pixels

		// ----- int humi
		int textposx = X2 + XADJ + ( 104 - _LengthFont_25x15 (_Int_Humidity_Text)) / 2 ;
		_PrintFont_25x15(0, textposx, ycoord+22, _Int_Humidity_Text, 0x0000);

		_Prev_Int_Humidity = _Int_Humidity;
	}

	// ====== INSIDE CHAMBRE=====
	if (_Prev_Chbr_Humidity != _Chbr_Humidity)
	{
		//------background outside---
		TFTG.fillRect (X3, ycoord, 104, height, HumidityColorScale (_Chbr_Humidity));

		// ----- title
		TFTG.setTextSize (1);
		TFTG.setCursor (X3 + (int)((104-77)/2), ycoord+5);
		TFTG.print ("Humi. Chambre"); //13x5+12=77 pixels

		// ----- chbr humi
		int textposx = X3 + XADJ + ( 104 - _LengthFont_25x15 (_Chbr_Humidity_Text)) / 2 ;
		_PrintFont_25x15(0, textposx, ycoord+22, _Chbr_Humidity_Text, 0x0000);

		_Prev_Chbr_Humidity = _Chbr_Humidity;
	}
}

// -------------------------------------------------------------------

void _DisplayRain (void)
{
	int ycoord = 57+4+57+4;
	int height = 57+4+57;

	if ( (_Prev_Rain_Hour != _Rain_Hour) || (_Prev_Rain_Day != _Rain_Day) )
	{
		// color
		TFTG.fillRect(0, ycoord, 104, height, RainColorScale ((_Rain_Day+(_Rain_Hour*5))/6));

		// unit
		TFTG.setTextSize (1);
		TFTG.setCursor (15, ycoord + 5);
		TFTG.print ("Pluie (mm/hr)");
		TFTG.setCursor (15, ycoord + 57+4+5);
		TFTG.print ("Pluie (mm/jr)");

		// value Rain/hour
		int textposx = X1 + ( 104 - _LengthFont_25x15(_Rain_Hour_Text)) / 2 ;
		_PrintFont_25x15 (0, textposx, ycoord+22, _Rain_Hour_Text, 0x0000);
		
		// value Rain/Day
		textposx = X1 + ( 104 - _LengthFont_25x15(_Rain_Day_Text)) / 2 ;
		_PrintFont_25x15 (0, textposx, ycoord+22+57+4, _Rain_Day_Text, 0x0000);
		
		_Prev_Rain_Hour = _Rain_Hour;
		_Prev_Rain_Day = _Rain_Day;
	}
}

// -------------------------------------------------------------------

void _DisplayCO2 (void)
{
	int ycoord = 57+4+57+4;
	int height = 57;

	// CO2 SALON
	if (_Prev_Int_CO2 != _Int_CO2)
	{
		// background
		TFTG.fillRect(X2, ycoord, 104, height, CO2ColorScale (_Int_CO2));
		
		// title
		TFTG.setTextSize (1);
		TFTG.setCursor (X2 + 10, ycoord + 5);
		TFTG.print ("CO2 Salon (ppm)");
		
		// value
		int textposx = X2 + ( 104 - _LengthFont_25x15(_Int_CO2_Text)) / 2 ;
		_PrintFont_25x15 (0, textposx, ycoord+22, _Int_CO2_Text, 0x0000);
		
		_Prev_Int_CO2 = _Int_CO2;
	}
	
	// CO2 CHAMBRE
	if (_Prev_Chbr_CO2 != _Chbr_CO2)
	{
		// background
		TFTG.fillRect(X3, ycoord, 104, height, CO2ColorScale (_Chbr_CO2));
		
		// title
		TFTG.setTextSize (1);
		TFTG.setCursor (X3 + 10, ycoord + 5);
		TFTG.print ("CO2 Chbre (ppm)");
		
		// value
		int textposx = X3 + ( 104 - _LengthFont_25x15(_Chbr_CO2_Text)) / 2 ;
		_PrintFont_25x15 (0, textposx, ycoord+22, _Chbr_CO2_Text, 0x0000);
	}
}

// -------------------------------------------------------------------

void _DisplayPressure (void)
{
	int ycoord = 57+4+57+4+57+4;
	int height = 57;

	if (_Prev_Pressure != _Pressure)
	{
		// color
		TFTG.fillRect(X2, ycoord, 319-X2, height, PressureColorScale (_Pressure));

		// title
		TFTG.setTextSize (1);
		TFTG.setCursor (X2 + 25, ycoord + 5);
		TFTG.print ("Pression Atmospherique (hPA)");

		// unit
		/*TFTG.setTextSize (2);
		TFTG.setCursor (260, ycoord + 22 + 11);
		TFTG.print ("hPA");*/

		// value
		int textposx = X2 + ( 212 - _LengthFont_25x15(_Pressure_Text)) / 2 ;
		_PrintFont_25x15(0, textposx, ycoord+22, _Pressure_Text, 0x0000);

		// trend
		_displayTrend (X2, ycoord, _Pressure_Trend);
		
		_Prev_Pressure = _Pressure;
	}
}



// -------------------------------------------------------------------
// -------------------------------------------------------------------
// --------------- RIGHT SCREEN --------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------

void _DisplayTime (void)
{
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

	if (minute(_CurrentTime) != minute(_PreviousTime) )
	{
		TFTD.fillRect(8, 0, 132, 45, 0x010);
		
		String _time="";
		if ( hour(_CurrentTime) < 10 )  _time="0";
		_time = _time + hour(_CurrentTime);
		_time = _time + ":";
		if ( minute(_CurrentTime) < 10 ) _time = _time + "0";
		_time = _time + minute(_CurrentTime);

		_PrintFont_29x16(1, 30, 8, _time, 0xFFFF);

		if (day(_CurrentTime) != day(_PreviousTime) )
		{
			TFTD.fillRect(140, 0, 172, 45, 0x010);
			String _date="";
			if ( day(_CurrentTime) < 10 )  _date="0";
			_date = _date + day(_CurrentTime);
			_date = _date + ".";
			if ( month(_CurrentTime) < 10 ) _date = _date + "0";
			_date = _date + month(_CurrentTime);
			_date = _date + ".";
			_date = _date + (year(_CurrentTime)-2000);
			
			_PrintFont_25x15(1, 160, 12, _date, 0xFFFF);
		}
		
		_PreviousTime = _CurrentTime;
		
		// Adjust backlight at night
		if ( (hour(_CurrentTime) >= 21) && (hour(_CurrentTime) < 01) )
		{
			analogWrite (backlightTFTD, BKLITE_NIGHT);
			analogWrite (backlightTFTG, BKLITE_NIGHT);
		}
		else if ( (hour(_CurrentTime) >= 01) && (hour(_CurrentTime) <= 5) )
		{
			analogWrite (backlightTFTD, 0);
			analogWrite (backlightTFTG, 0);
		}
		else
		{
			analogWrite (backlightTFTD, BKLITE_DAY);
			analogWrite (backlightTFTG, BKLITE_DAY);
		}
		
	}
}

// -------------------------------------------------------------------

void _DisplayPressureHistory (void)
{
	/*
	Pressure : 980 to 1040 (might be low as 970 but rarely, as well as rarely over 1030
	*/

	int xcoord = 35;
	int height = 121; // 60*2 + 1
	int ycoord = 239-height;

	// 6 pix width per measure
	// block of 8 values per 24 hrs
	// 1 pix separator between every 24 hrs period
	//values from 970 to 1030 => 61 different values
	
	TFTD.fillRect (xcoord, ycoord, 319-xcoord+1, height, 0x0);

	for (int i=0; i<6; i++) // vertical lines
	{
		TFTD.drawFastVLine (xcoord + i*48+i*2, ycoord, 120, 0x7BEF);
	}
	for (int j=0; j<7; j++) // horizontal lines
	{
		TFTD.drawFastHLine (xcoord, ycoord+j*20, 251, 0x7BEF);
	}

	for (int i=1; i<6; i++) // 5 most recent period of 24 hrs out of 6
	{
		for (int j=0; j<8; j++) // block of 8 values
		{
			int _PressureVal = _PressureHistory [j+i*8] - 980;
			if (_PressureVal < 0) _PressureVal = 0;
			if (_PressureVal > 60) _PressureVal = 60;
			TFTD.fillRect ( xcoord + j*6 + (i-1)*50 + 2, 239 - _PressureVal*2 , 5, _PressureVal*2, 0x1F);
		}
	}
	
	TFTD.setTextSize(1);
	TFTD.setTextColor(0xFFFF);
	TFTD.setCursor ( 5, ycoord + 18); TFTD.print ("1030");
	TFTD.setCursor ( 5, ycoord + 38); TFTD.print ("1020");
	TFTD.setCursor ( 5, ycoord + 58); TFTD.print ("1010");
	TFTD.setCursor ( 5, ycoord + 78); TFTD.print ("1000");
	TFTD.setCursor ( 5, ycoord + 98); TFTD.print (" 990");
}

// -------------------------------------------------------------------

void _DisplayTemperatureHistory (void)
{
	// superimpose to pressure history as curve
	// dynamic range to test (6 sectors to adjust (min = 2 de gper sector, could be 3 or 4...)
	
	// first range = 0 to 20

	int xcoord = 35;
	int height = 121; // 60*2 + 1
	int ycoord = 239-height;

	int _tmax = _TemperatureHistory[0][48];
	int _tmin = _TemperatureHistory[1][48];

	int interval = 2; // 2 degrees per horiz line -> 12 degrees total
	if ((_tmax-_tmin) > 10) interval = 3;
	if ((_tmax-_tmin) > 16) interval = 4;
	if ((_tmax-_tmin) > 22) interval = 5;

	int trefmin = _tmin - (6*interval - (_tmax-_tmin) ) / 2;
	float PixDeg = 120 / (float)(6 * interval);
	
	/*Serial.print ("Trefmin :");
	Serial.println (trefmin);
	Serial.print ("Interval :");
	Serial.println (interval);
	Serial.print ("PixDeg :");
	Serial.println (PixDeg);*/
	
	TFTD.setTextSize(1);
	TFTD.setTextColor(0xFFFF);
	TFTD.setCursor ( 290, ycoord + 18); TFTD.print (trefmin+5*interval);
	TFTD.setCursor ( 290, ycoord + 38); TFTD.print (trefmin+4*interval);
	TFTD.setCursor ( 290, ycoord + 58); TFTD.print (trefmin+3*interval);
	TFTD.setCursor ( 290, ycoord + 78); TFTD.print (trefmin+2*interval);
	TFTD.setCursor ( 290, ycoord + 98); TFTD.print (trefmin+1*interval);

	for (int i=1; i<6; i++) // 5 most recent period of 24 hrs out of 6
	{
		for (int j=0; j<8; j++) // block of 8 values
		{
			int index = j+i*8;
			
			float yPixMinTemp = PixDeg * (_TemperatureHistory[1][index] - trefmin);
			float yPixMaxTemp = PixDeg * (_TemperatureHistory[0][index] - trefmin);
			float NextyPixMinTemp = PixDeg * (_TemperatureHistory[1][index+1] - trefmin);
			float NextyPixMaxTemp = PixDeg * (_TemperatureHistory[0][index+1] - trefmin);
			
			/*Serial.print ("Index :");
			Serial.print (index);
			Serial.print (" temp ");
			Serial.print (_TemperatureHistory[0][index]);
			Serial.print (" Ypixels ");
			Serial.println (yPixMaxTemp);*/

			// min temp cyan vertical bar and bottom dash
			TFTD.drawFastVLine ( xcoord + j*6 + (i-1)*50 + 4, 238-yPixMaxTemp, yPixMaxTemp-yPixMinTemp+1, 0x07FF);
			TFTD.drawFastHLine ( xcoord + j*6 + (i-1)*50 + 3, 238-yPixMinTemp, 3, 0x07FF);

			// max temp square (red)
			TFTD.fillRect ( xcoord + j*6 + (i-1)*50 + 3, 238-yPixMaxTemp-1, 3, 3, 0xF800);

			if (index < 47) // to limit the index to 47 (table has 48 values, and we use index +1, so we do not want to exceed the table size...
			{
				int ni=i;
				int nj=j+1;
				if (nj==8) {ni=ni+1; nj=0;}
				TFTD.drawLine ( xcoord + j*6 + (i-1)*50 + 4, 238-yPixMaxTemp, xcoord + nj*6 + (ni-1)*50 + 4, 238-NextyPixMaxTemp, 0xF800);
			}
		}
	}
}

// -------------------------------------------------------------------

void _DisplayWeather2 (int offsetX, int Weather, int TempMin, int TempMax, int Cloud, float Rain)
{
	// full width
	// top = 50 , height = 66
	
	int _ColorWeather;
	int _TextColor;
	int offsetY = 50;
	
	if 	( Weather == 1 ) _ColorWeather = 0xFFE0; // sun = yellow
	if 	( Weather == 2 ) _ColorWeather = 0xFFEF; // few clouds = light yellow
	if 	( Weather == 3 ) _ColorWeather = 0xBDF7; // clouds = ligth grey
	if 	( Weather == 4 ) _ColorWeather = 0x7BEF; // lots clouds = dark grey
	if 	( Weather == 9 ) _ColorWeather = 0x7BFF; // lots rain = light blue
	if 	( Weather == 10 ) _ColorWeather = 0x7FFF; //  rain = light cyan
	if 	( Weather == 11 ) _ColorWeather = 0x79E0; //  thunderstorm = brown
	if 	( Weather == 13 ) _ColorWeather = 0xFFFF; //  snow = white
	if 	( Weather == 50 ) _ColorWeather = 0x0000; //  mist = black
	if  ( Weather == 50 )
	{
		TFTD.setTextColor (0xFFFF);
		_TextColor = 0xFFFF;
	}
	else
	{
		TFTD.setTextColor (0x0000);
		_TextColor = 0x0000;
	}
	
	TFTD.fillRect (offsetX, 50, 104, 66, _ColorWeather);
	
	//title
	TFTD.setTextSize (1);
	TFTD.setCursor (offsetX+13, offsetY+3);
	if 	( Weather == 1 )  TFTD.print ("  Ensoleille ");
	if 	( Weather == 2 )  TFTD.print (" Peu Couvert ");
	if 	( Weather == 3 )  TFTD.print ("   Couvert   ");
	if 	( Weather == 4 )  TFTD.print (" Tres Couvert");
	if 	( Weather == 9 )  TFTD.print ("Tres Pluvieux");
	if 	( Weather == 10 ) TFTD.print ("   Pluvieux  ");
	if 	( Weather == 11 ) TFTD.print ("    Orage    ");
	if 	( Weather == 13 ) TFTD.print ("    Neige    ");
	if 	( Weather == 50 ) TFTD.print ("   Tempete   ");
	
	//TEMP display
	String Disp="";
	Disp = Disp + TempMin;
	Disp = Disp + "s";
	Disp = Disp + TempMax;
	Disp = Disp + "d";
	int _DispLength = _LengthFont_25x15(Disp);
	_PrintFont_25x15 (1, offsetX+((104-_DispLength)/2), 65, Disp, 0x0000);
	Serial.print("METEO : "); Serial.print (Disp); Serial.print ("   :");Serial.println (_DispLength);
	
	//cloud
	TFTD.setTextSize (1);
	TFTD.setCursor (offsetX+10, offsetY+46);
	TFTD.print ("Nuages :  ");
	if ( (Cloud < 10) ) TFTD.print(" ");
	if ( (Cloud < 100) ) TFTD.print(" ");
	TFTD.print (Cloud);
	TFTD.print(" p");
	
	//rain
	TFTD.setCursor (offsetX+10, offsetY+56);
	TFTD.print ("Pluie  :  ");
	TFTD.print (Rain);
}

// -------------------------------------------------------------------

void _DisplayCountDown (void)
{
	_CountDown =   ( millis() - MILLIS_INTERNET ) / 33333; // every 0,5 minute, goes from 0 to 9
	if ( _CountDown != _PreviousCountDown )
	{
		if ( _CountDown == 0 ) // reset display
		{
			TFTD.fillRect (0, 0, 5, 45, 0x0000);
			TFTD.fillRect (0, _CountDown * 5, 5, 4, 0x7E0);
		}
		else
		{
			TFTD.fillRect (0, _CountDown * 5, 5, 4, 0x7E0);
		}
		
		_PreviousCountDown = _CountDown;
	}
	
}

// -------------------------------------------------------------------

void _DisplayInternetStatus (int color)
{
	TFTD.fillRect (314, 0, 5, 45, color);
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------
// --------------- COLOR SCALES --------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------

int RainColorScale (int value)
{
	// input : value 0 à 20 mm
	// output : de 32 (vert) à 0 (bleu)

	int index = constrain (map (value, 0, 10, 0, 32), 0, 32);
	return _Colors[32-index];
}

// -------------------------------------------------------------------

int HumidityColorScale (int value)
{
	// input : value 40 à 100
	// output : de 32 (vert) à 0 (bleu)

	int index = constrain (map (value, 40, 100, 0, 32), 0, 32);
	return _Colors[32 - index];
}

// -------------------------------------------------------------------

int CO2ColorScale (int value)
{
	// input : value 400 à 2500
	// output : de 32 (vert) à 64 (rouge)

	int index = constrain (map (value, 400, 2500, 32, 64), 32, 64);
	return _Colors[index];
}

// -------------------------------------------------------------------

int TempColorScale (int value)
{
	// input : value -5 à 35 degrés Celcius
	// output : de 0 (bleu) à 64 (rouge)

	int index = constrain (map (value, -5, 35, 0, 64), 0, 64);
	return _Colors[index];
}

// -------------------------------------------------------------------

int PressureColorScale (int value)
{
	// input : value 990 à 1035 hPa
	// output : de 32 (jaune) à 64 (rouge)

	int index = constrain (map (value, 990, 1035, 32, 64), 32, 64);
	return _Colors[index];
}


// -------------------------------------------------------------------
// -------------------------------------------------------------------
// --------------- CUSTOM FONT ---------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------

// display= 0 -> left screen / diaplay=1 -> right screen
// x and y is upper left corner of digit written on screen
// digit is digit to display within font definition

// -- calculates the length in pixels of a string
int _LengthFont_25x15 (String text)
{
	int pixels=0;
	for (int cursor=0; cursor < text.length(); cursor++)
	{
		switch (text[cursor]) {
			case '-':
			pixels += 10;
			break;
			case '%':
			pixels += 14;
			break;
			case '.':
			pixels += 5;
			break;
			case 'd':
			pixels += 6;
			break;
			case 's':
			pixels += 2;
			break;
			default :
			pixels += 16;
			break;
		}
		pixels += 4; // space between 2 chars
	}
	pixels -= 4; // remove the last space
	return pixels;
}

// -- displays ONE character --
void _WriteFont_25x15 (int display, int x_offset, int y_offset, int digit, int color)
{
	for (int y = 0; y < 25; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			int value = (boolean) bitRead (Font_25x15 [digit][y], 15-x);
			if ( (value) && (display==0) ) TFTG.drawPixel (x_offset + x, y_offset + y, color);
			if ( (value) && (display==1) ) TFTD.drawPixel (x_offset + x, y_offset + y, color);
		}
	}
}

// -- displays ONE character --
void _WriteFont_29x16 (int display, int x_offset, int y_offset, int digit, int color)
{
	for (int y = 0; y < 29; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			int value = (boolean) bitRead (Font_29x16 [digit][y], 15-x);
			if ( (value) && (display==0) ) TFTG.drawPixel (x_offset + x, y_offset + y, color);
			if ( (value) && (display==1) ) TFTD.drawPixel (x_offset + x, y_offset + y, color);
		}
	}
}

// -- displays a String (by calling WriteFont)
void _PrintFont_25x15 (int display, int x_offset, int y_offset, String text, int color)
{
	int xshift=0;
	char digit;
	for (int cursor=0; cursor < text.length(); cursor++)
	{
		digit = text[cursor];
		switch (digit) {
			case '0':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 0, color);
			xshift=xshift+20;
			break;
			case '1':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 1, color);
			xshift=xshift+20;
			break;
			case '2':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 2, color);
			xshift=xshift+20;
			break;
			case '3':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 3, color);
			xshift=xshift+20;
			break;
			case '4':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 4, color);
			xshift=xshift+20;
			break;
			case '5':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 5, color);
			xshift=xshift+20;
			break;
			case '6':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 6, color);
			xshift=xshift+20;
			break;
			case '7':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 7, color);
			xshift=xshift+20;
			break;
			case '8':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 8, color);
			xshift=xshift+20;
			break;
			case '9':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 9, color);
			xshift=xshift+20;
			break;
			case '.':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 10, color);
			xshift=xshift+9;
			break;
			case '%':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 11, color);
			xshift=xshift+18;
			break;
			case '-':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 12, color);
			xshift=xshift+14;
			break;
			case 'd':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 13, color);
			xshift=xshift+10;
			break;
			case 's':
			_WriteFont_25x15 (display, x_offset+xshift, y_offset, 14, color);
			xshift=xshift+6;
			break;
		}
	}
}

// -- displays a String (by calling WriteFont)
void _PrintFont_29x16 (int display, int x_offset, int y_offset, String text, int color)
{
	int xshift=0;
	char digit;
	for (int cursor=0; cursor < text.length(); cursor++)
	{
		digit = text[cursor];
		switch (digit) {
			case '0':
			_WriteFont_29x16 (display, x_offset+xshift, y_offset, 0, color);
			xshift=xshift+20;
			break;
			case '1':
			_WriteFont_29x16 (display, x_offset+xshift, y_offset, 1, color);
			xshift=xshift+14;
			break;
			case '2':
			_WriteFont_29x16 (display, x_offset+xshift, y_offset, 2, color);
			xshift=xshift+20;
			break;
			case '3':
			_WriteFont_29x16 (display, x_offset+xshift, y_offset, 3, color);
			xshift=xshift+20;
			break;
			case '4':
			_WriteFont_29x16 (display, x_offset+xshift, y_offset, 4, color);
			xshift=xshift+20;
			break;
			case '5':
			_WriteFont_29x16 (display, x_offset+xshift, y_offset, 5, color);
			xshift=xshift+20;
			break;
			case '6':
			_WriteFont_29x16 (display, x_offset+xshift, y_offset, 6, color);
			xshift=xshift+20;
			break;
			case '7':
			_WriteFont_29x16 (display, x_offset+xshift, y_offset, 7, color);
			xshift=xshift+20;
			break;
			case '8':
			_WriteFont_29x16 (display, x_offset+xshift, y_offset, 8, color);
			xshift=xshift+20;
			break;
			case '9':
			_WriteFont_29x16 (display, x_offset+xshift, y_offset, 9, color);
			xshift=xshift+20;
			break;
			case ':':
			_WriteFont_29x16 (display, x_offset+xshift, y_offset, 10, color);
			xshift=xshift+9;
			break;
		}
	}
}

// -------------------------------------------------------------------
// -------------------------------------------------------------------
// --------------- END OF FILE  --------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------





#ifndef NETATMO_EXTERNAL_VARS_H_
#define NETATMO_EXTERNAL_VARS_H_
#endif /* NETATMO_EXTERNAL_VARS_H_ */


//********************** NETATMO VALUES *************
extern int _Prev_Ext_Temp;
extern int _Ext_Temp;
extern String _Ext_Temp_Text;
extern String _Ext_Temp_Trend;

extern int _Int_Temp;
extern int _Prev_Int_Temp;
extern String _Int_Temp_Trend;
extern String _Int_Temp_Text;

extern int _Chbr_Temp;
extern int _Prev_Chbr_Temp;
extern String _Chbr_Temp_Trend;
extern String _Chbr_Temp_Text;

extern int _Ext_Humidity;
extern int _Prev_Ext_Humidity;
extern String _Ext_Humidity_Text;

extern int _Int_Humidity;
extern int _Prev_Int_Humidity;
extern String _Int_Humidity_Text;

extern int _Chbr_Humidity;
extern int _Prev_Chbr_Humidity;
extern String _Chbr_Humidity_Text;

extern float _Rain_Hour;
extern float _Prev_Rain_Hour;
extern String _Rain_Hour_Text;

extern float _Rain_Day;
extern float _Prev_Rain_Day;
extern String _Rain_Day_Text;

extern int _Int_CO2;
extern int _Prev_Int_CO2;
extern String _Int_CO2_Text;

extern int _Chbr_CO2;
extern int _Prev_Chbr_CO2;
extern String _Chbr_CO2_Text;

extern int _Pressure;
extern int _Prev_Pressure;
extern String _Pressure_Text;
extern String _Pressure_Trend;

extern int _TemperatureHistory [2][49];

extern Adafruit_ILI9341 TFTD;
extern Adafruit_ILI9341 TFTG;

extern int _TodayTempMin;
extern int _TodayTempMax;
extern int _TomorrowTempMin;
extern int _TomorrowTempMax;
extern int _DayAfterTempMin;
extern int _DayAfterTempMax;

extern int _TodayWeather;
extern int _TomorrowWeather;
extern int _DayAfterWeather;

extern int _TodayCloud;
extern int _TomorrowCloud;
extern int _DayAfterCloud;

extern float _TodayRain;
extern float _TomorrowRain;
extern float _DayAfterRain;

extern char c1;
extern char c2;

extern String _SSID;
extern String _PASS;
extern String host;
extern String request;
extern String _buf;
extern int _buf_length;
extern int _cursor;

extern time_t _WeatherTime;

extern int _color;
extern int _Colors[65];

extern unsigned int Font_25x15 [15][25];
extern unsigned int Font_29x16 [11][29];

extern int _CountDown;
extern int _PreviousCountDown;
extern unsigned long MILLIS_INTERNET;
extern time_t _CurrentTime;
extern time_t _PreviousTime;

extern int _PressureHistory [48]; // to store the history of PRessure values read from Netatmo web site

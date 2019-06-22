<?php

$url = "http://api.openweathermap.org/data/2.5/forecast/city?id=yourid&APPID=yourappid&units=metric&lang=fr";
$request = file_get_contents($url);
$json_weather_data = json_decode($request,true);

// var_dump ($json_weather_data);

echo ("#");

for ($i=0; $i < 24; $i++)
  {
echo $json_weather_data["list"][$i]["dt"]; // timestamp
echo ("#");
echo $json_weather_data["list"][$i]["main"]["temp"]; // temp
echo ("#");
echo $json_weather_data["list"][$i]["weather"][0]["icon"]; // meteo
echo ("#");
echo $json_weather_data["list"][$i]["clouds"]["all"]; // nuage
echo ("#");
echo $json_weather_data["list"][$i]["rain"]["3h"]; // pluie
echo ("#");

//echo ("<br>");
  }



?>
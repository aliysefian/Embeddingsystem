<?php

$password="yourpassword";
$username="youremail";
$app_id = "yourappid";
$app_secret ="yourappsecret";

$token_url = "https://api.netatmo.net/oauth2/token";
$postdata = http_build_query(
        array(
            'grant_type' => "password",
            'client_id' => $app_id,
            'client_secret' => $app_secret,
            'username' => $username,
            'password' => $password
    )
);

$opts = array('http' =>
    array(
        'method'  => 'POST',
        'header'  => 'Content-type: application/x-www-form-urlencoded',
        'content' => $postdata
    )
);

$context  = stream_context_create($opts);
$response = file_get_contents($token_url, false, $context);

$params = null;
$params = json_decode($response, true);

//var_dump ($params);

// =========== GET REAL TIME DATA =================

$station_url = "https://api.netatmo.net/api/getstationsdata?access_token=" . $params['access_token'];
$requete = file_get_contents($station_url);
$json_stations_data = json_decode($requete,true);
//echo ("## GET STATIONS DATA ##");
//var_dump ($json_stations_data);
//echo ("##########");
//echo $json_stations_data["body"]["user"]["mail"];

// MODULE EXTERIEUR //
echo ("#EXT#");
echo $json_stations_data["body"]["devices"][0]["modules"][0]["dashboard_data"]["Temperature"]; 		//temp ext
echo ("#");
echo $json_stations_data["body"]["devices"][0]["modules"][0]["dashboard_data"]["temp_trend"]; 		//temp ext trend
echo ("#");
echo $json_stations_data["body"]["devices"][0]["modules"][0]["dashboard_data"]["Humidity"]; 		//Humidity

// PLUVIOMETRE //
echo ("#PLUVIO#");
echo $json_stations_data["body"]["devices"][0]["modules"][1]["dashboard_data"]["Rain"]; 			//pluie
echo ("#");
echo $json_stations_data["body"]["devices"][0]["modules"][1]["dashboard_data"]["sum_rain_24"]; 		//pluie 24 h
echo ("#");
echo $json_stations_data["body"]["devices"][0]["modules"][1]["dashboard_data"]["sum_rain_1"]; 		//pluie 1 h

// MODULE INTERIEUR PRINCIPAL - SALON //
echo ("#SALON#");
echo $json_stations_data["body"]["devices"][0]["dashboard_data"]["Noise"];		 					//bruit
echo ("#");
echo $json_stations_data["body"]["devices"][0]["dashboard_data"]["Temperature"]; 					//temp
echo ("#");
echo $json_stations_data["body"]["devices"][0]["dashboard_data"]["temp_trend"]; 					//temp trend
echo ("#");
echo $json_stations_data["body"]["devices"][0]["dashboard_data"]["Humidity"]; 						//humidity
echo ("#");
echo $json_stations_data["body"]["devices"][0]["dashboard_data"]["Pressure"]; 						//pressure 
echo ("#");
echo $json_stations_data["body"]["devices"][0]["dashboard_data"]["pressure_trend"]; 				//pressure trend
echo ("#");
echo $json_stations_data["body"]["devices"][0]["dashboard_data"]["CO2"]; 							//CO2

// MODULE INTERIEUR SECONDAIRE - CHAMBRE //
echo ("#CHAMBRE#");
echo $json_stations_data["body"]["devices"][0]["modules"][2]["dashboard_data"]["Temperature"]; 					//temp
echo ("#");
echo $json_stations_data["body"]["devices"][0]["modules"][2]["dashboard_data"]["temp_trend"]; 					//temp trend
echo ("#");
echo $json_stations_data["body"]["devices"][0]["modules"][2]["dashboard_data"]["Humidity"]; 						//humidity
echo ("#");
echo $json_stations_data["body"]["devices"][0]["modules"][2]["dashboard_data"]["CO2"]; 							//CO2
echo ("#");


// ============ GET PRESSURE HISTORY ==============

$param2 = "&device_id=yourmacadress&scale=3hours&type=pressure&limit=48&real_time=true&date_begin=";
$param3 = time ()-518400;

$station_url = "https://api.netatmo.com/api/getmeasure?access_token=" . $params['access_token'] . $param2 . $param3;
//echo $station_url;
$requete = file_get_contents($station_url);
$json_stations_data = json_decode($requete,true);
//echo ("## GET MEASURE DATA ##");
//var_dump ($json_stations_data);
//echo ("##########");
//echo $json_stations_data["body"]["user"]["mail"];
echo ("<br>");
// LES 48 DERNIERES VALEURS DE PRESSION (1 MESURE / 3 heures, sur 6 jours => 48 mesures)
for ($i=0; $i < 48; $i++)
  {
    echo ("%"); 
    echo $json_stations_data["body"][0]["value"][$i][0];
  }
echo ("%"); 


// ============== GET MAX TEMP HISTORY =============

$param2 = "&device_id=yourmacadress&module_id=yourothermacadress&scale=3hours&type=max_temp&limit=48&real_time=true&date_begin=";
$param3 = time ()-518400;

$station_url = "https://api.netatmo.com/api/getmeasure?access_token=" . $params['access_token'] . $param2 . $param3;
//echo $station_url;
$requete = file_get_contents($station_url);
$json_stations_data = json_decode($requete,true);
//echo ("## GET MEASURE DATA ##");
//var_dump ($json_stations_data);
//echo ("##########");
//echo $json_stations_data["body"]["user"]["mail"];
echo ("<br>");
// LES 48 DERNIERES VALEURS DE PRESSION (1 MESURE / 3 heures, sur 6 jours => 48 mesures)
for ($i=0; $i < 48; $i++)
  {
    echo ("%"); 
    echo $json_stations_data["body"][0]["value"][$i][0];
  }
echo ("%"); 


// ============== GET MIN TEMP HISTORY =============

$param2 = "&device_id=yourmacadress&module_id=yourothermacadress&scale=3hours&type=min_temp&limit=48&real_time=true&date_begin=";
$param3 = time ()-518400;

$station_url = "https://api.netatmo.com/api/getmeasure?access_token=" . $params['access_token'] . $param2 . $param3;
//echo $station_url;
$requete = file_get_contents($station_url);
$json_stations_data = json_decode($requete,true);
//echo ("## GET MEASURE DATA ##");
//var_dump ($json_stations_data);
//echo ("##########");
//echo $json_stations_data["body"]["user"]["mail"];
echo ("<br>");
// LES 48 DERNIERES VALEURS DE PRESSION (1 MESURE / 3 heures, sur 6 jours => 48 mesures)
for ($i=0; $i < 48; $i++)
  {
    echo ("%"); 
    echo $json_stations_data["body"][0]["value"][$i][0];
  }
echo ("%"); 



?>
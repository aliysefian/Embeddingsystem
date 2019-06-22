
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
const char* ssid = "math";
const char* password = "Ali9137266252@1372";

const char* host = "ali1391ali1391.000webhostapp.com";

void setup() {
	Serial.begin(115200);
	delay(10);

	// We start by connecting to a WiFi network

	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
	delay(5000);
	++value;

	Serial.print("connecting to ");
	Serial.println(host);

	// Use WiFiClient class to create TCP connections
	WiFiClient client;
	const int httpPort = 80;
	if (!client.connect(host, httpPort)) {
		Serial.println("connection failed");
		return;
	}

	// We now create a URI for the request
	String url = "/ali.php?a=1&b=211112rrr";

	Serial.print("Requesting URL: ");
	Serial.println(url);

	// This will send the request to the server
	client.print(String("GET ") + url + " HTTP/1.1\r\n" +
		"Host: " + host + "\r\n" +
		"Connection: close\r\n\r\n");
	delay(10);
	StaticJsonBuffer<200> jsonBuffer;

	// Read all the lines of the reply from server and print them to Serial
	Serial.println("Respond:");
	String line = "";
	String value = "";
	int i = 0;
	while (client.available()) {

		String line = client.readStringUntil('\r');
		line.indexOf('')
		Serial.println();
		Serial.println("list in :"+String(i));
		if (i == 8)
			value = line;
		Serial.println(line);
		i++;

	}
	Serial.println("ffffff:");
	Serial.println(value);
	Serial.println("wwwwwwwwwwwwww");
	char jsonww[400];
	value.toCharArray(jsonww, 400);
	char json[] =
	"{\"ReferenceNo\":\"KWPOG0QoXU\",\"Message\":\"SUCCESS\",\"Status\":1,\"ResponseStatus\":200}";*/
	//Serial.print(line);
	JsonObject& root = jsonBuffer.parseObject(value);

	//if (!root.success())
	//{
	//	//Serial.println("parseObject() failed");
	//	continue;
	//}
	Serial.println("wwwwww ");
	//
	// Step 3: Retrieve the values
	//
	const char* sensor = root["ReferenceNo"];
	/*long        time = root["Message"];
	double      latitude = root["Status"];
	double      longitude = root["ResponseStatus"];*/
	Serial.println("value:::::: ");
	Serial.println(String(sensor));
	Serial.println();
	Serial.println("closing connection");
}
#include <SPI.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <Xively.h>

char ssid[] = "YourNetwork"; //  your network SSID (name) 
char pass[] = "password";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Your Xively key to let you upload data
char xivelyKey[] = "YOUR_API_KEY";

// Analog pin which we're monitoring (0 and 1 are used by the Ethernet shield)
int sensorPin = 2;

// Define the strings for our datastream IDs
char sensorId[] = "sensor_reading";
char bufferId[] = "info_message";
String stringId("random_string");
const int bufferSize = 140;
char bufferValue[bufferSize]; // enough space to store the string we're going to send
XivelyDatastream datastreams[] = {
  XivelyDatastream(sensorId, strlen(sensorId), DATASTREAM_FLOAT),
  XivelyDatastream(bufferId, strlen(bufferId), DATASTREAM_BUFFER, bufferValue, bufferSize),
  XivelyDatastream(stringId, DATASTREAM_STRING)
};
// Finally, wrap the datastreams into a feed
XivelyFeed feed(15552, datastreams, 3 /* number of datastreams */);

WiFiClient client;
XivelyClient xivelyclient(client);

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.println("Starting multiple datastream upload to Xively...");
  Serial.println();

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  datastreams[0].setFloat(sensorValue);

  Serial.print("Read sensor value ");
  Serial.println(datastreams[0].getFloat());

  datastreams[1].setBuffer("a message to upload");
  Serial.print("Setting buffer value to:\n    ");
  Serial.println(datastreams[1].getBuffer());

  // Pick a random number to send up in a string
  String stringValue(random(100));
  stringValue += " is a random number";
  datastreams[2].setString(stringValue);
  Serial.print("Setting string value to:\n    ");
  Serial.println(datastreams[2].getString());

  Serial.println("Uploading it to Xively");
  int ret = xivelyclient.put(feed, xivelyKey);
  Serial.print("xivelyclient.put returned ");
  Serial.println(ret);

  Serial.println();
  delay(15000);
}

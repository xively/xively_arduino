#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

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

EthernetClient client;
XivelyClient xivelyclient(client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.println("Starting multiple datastream upload to Xively...");
  Serial.println();

  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
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


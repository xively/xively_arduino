#include <SPI.h>
#include <GSM.h>
#include <HttpClient.h>
#include <Xively.h>

// Your phone SIM PIN Number
#define PINNUMBER ""

// Your phone APN data
#define GPRS_APN       "eseye.com" // replace your GPRS APN
#define GPRS_LOGIN     ""    // replace with your GPRS login
#define GPRS_PASSWORD  "" // replace with your GPRS password

// Your Xively key to let you upload data
char xivelyKey[] = "YOUR_XIVELY_API_KEY";

// Analog pin which we're monitoring 
int sensorPin = 2;

// Define the strings for our datastream IDs
// Replace these with strings that make sense for what you're recording
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
XivelyFeed feed(YOUR_FEED_ID, datastreams, 3 /* number of datastreams */);

// initialize the library instance
GSM gsmAccess;     // include a 'true' parameter to enable debugging
GPRS gprs;
GSMClient client;
XivelyClient xivelyclient(client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("Starting multiple datastream upload to Xively...");
  Serial.println();

  // connection state
  boolean notConnected = true;
  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while(notConnected)
  {
    if((gsmAccess.begin(PINNUMBER)==GSM_READY) &
      (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY))
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
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


#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Your Xively key to let you upload data
char xivelyKey[] = "YOUR_XIVELY_API_KEY";

// Define the string for our datastream ID
char temperatureId[] = "temperature";

XivelyDatastream datastreams[] = {
  XivelyDatastream(temperatureId, strlen(temperatureId), DATASTREAM_FLOAT),
};
// Finally, wrap the datastreams into a feed
XivelyFeed feed(15552, datastreams, 1 /* number of datastreams */);

EthernetClient client;
XivelyClient xivelyclient(client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.println("Reading from Xively example");
  Serial.println();

  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
}

void loop() {
  int ret = xivelyclient.get(feed, xivelyKey);
  Serial.print("xivelyclient.get returned ");
  Serial.println(ret);

  if (ret > 0)
  {
    Serial.println("Datastream is...");
    Serial.println(feed[0]);

    Serial.print("Temperature is: ");
    Serial.println(feed[0].getFloat());
  }

  Serial.println();
  delay(15000UL);
}


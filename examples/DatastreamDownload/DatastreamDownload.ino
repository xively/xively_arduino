#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Pachube.h>

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Your Pachube key to let you upload data
char pachubeKey[] = "AYC0gGBGd5sFG5ae9gushjbRM_QzywpfMkzwUlAH6V8";

// Define the string for our datastream ID
char temperatureId[] = "temperature";

Datastream datastreams[] = {
  Datastream(temperatureId, strlen(temperatureId), DATASTREAM_FLOAT),
};
// Finally, wrap the datastreams into a feed
PachubeFeed feed(15552, datastreams, 2 /* number of datastreams */);

EthernetClient client;
PachubeClient pachubeclient(client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.println("Reading from Pachube example");
  Serial.println();

  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
}

void loop() {
  int ret = pachubeclient.get(feed, pachubeKey);
  Serial.print("pachubeclient.get returned ");
  Serial.println(ret);

  Serial.println("Datastream is...");
  Serial.println(feed[0]);

  Serial.print("Temperature is: ");
  Serial.println(feed[0].getFloat());

  Serial.println();
  delay(15000UL);
}


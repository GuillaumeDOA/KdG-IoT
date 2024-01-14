#include "Arduino.h"
#include "WiFiS3.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "Guillaume's tellie";       // your network SSID (name)
char pass[] = "Guillaume12"; // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
// IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "https://script.google.com/macros/s/AKfycbzpolrHRgbMzY2GmgDRXfAzVL4bJFS9vaol7FUbyNlyLbdMHz8QkqKYDBe9Pd6tGM-2GQ/exec"; // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

/* -------------------------------------------------------------------------- */
void setup()
{
  /* -------------------------------------------------------------------------- */
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect('https://script.google.com/macros/s/AKfycbzpolrHRgbMzY2GmgDRXfAzVL4bJFS9vaol7FUbyNlyLbdMHz8QkqKYDBe9Pd6tGM-2GQ/exec', 443))
  {
    // send HTTP header
    client.println("POST / HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Connection: close");
    client.println(); // end HTTP header

    // send HTTP body
    client.println("?pm2=10&pm10=50");
  } else {
    Serial.println('Connection failed');
  }
}

/* just wrap the received data up to 80 columns in the serial print*/
/* -------------------------------------------------------------------------- */
void read_response()
{
  /* -------------------------------------------------------------------------- */
  uint32_t received_data_num = 0;
  while (client.available())
  {
    /* actual data reception */
    char c = client.read();
    /* print data to serial port */
    Serial.print(c);
    /* wrap data to 80 columns*/
    received_data_num++;
    if (received_data_num % 80 == 0)
    {
      Serial.println();
    }
  }
}

/* -------------------------------------------------------------------------- */
void loop()
{
  /* -------------------------------------------------------------------------- */
  read_response();

  // if the server's disconnected, stop the client:
  if (!client.connected())
  {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true)
      ;
  }
}


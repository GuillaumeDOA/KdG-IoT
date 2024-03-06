#include <Arduino.h>

#include "WiFi.h"
#include "ESPAsyncWebServer.h"

const char* BUTTON_PARAM = "pump";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>Dashboard Plantbewatering</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
        html {
            font-family: Arial;
            display: inline-block;
            text-align: center;
        }
        h2 {
            font-size: 3.0rem;
        }
        p {
            font-size: 1.5rem;
        }
        body {
            max-width: 600px;
            margin: 0px auto;
            padding-bottom: 25px;
        }
    </style>
</head>
<body>
  <h2>Dashboard plantbewatering</h2>
  <p>
     Percentage bodemvochtigheid: %BODEMVOCHTIGHEID%
  </p>
  <form>
     %BUTTONPLACEHOLDER%
  </form>

<script>
  function clickButton(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/update?pump=true", true);
    xhr.send();
  }
</script>
</body>
</html>
)rawliteral";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setupWifiAccessPoint() {
  // Set Wifi mode to both standard connection and AP
  WiFi.mode(WIFI_AP_STA);

  // Make sure the password is at least 8 characters long
  Serial.println("Setting up soft access point...");
  WiFi.softAP("wifi_guillaume", "0471990383");

  // Display IP address to use for webserver
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

String geefBodemvochtigheid() {
  // Zet hier code die het vochtigheidspercentage van je gecalibreerde sensor teruggeeft
  return "50";
}

// Replaces placeholders with buttons or value section in your web page
String replacePlaceholdersInHtml(const String& var){
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "<input type=\"button\" value=\"Plant bewateren\" onclick=\"clickButton(this)\" >";
    return buttons;
  } else if (var == "BODEMVOCHTIGHEID") {
    return geefBodemvochtigheid();
  }
  return String();
}

void bewaterPlant() {
  // Zet hier code die je plant bewatert
  Serial.println("Plant bewateren...");
  digitalWrite(D13, HIGH);
  // Don't use delay, just for demo purpose
  delay(2000); 
  digitalWrite(D13, LOW);
}

void setup(){
  Serial.begin(9600);

  // Set pinmode for the builtin LED
  pinMode(D13, OUTPUT);
  
  // Setup Wifi access point
  setupWifiAccessPoint();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, replacePlaceholdersInHtml);
  });

  // Send a GET request to <ESP_IP>/update?output=<pinNumber>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam(BUTTON_PARAM)) {
      Serial.println("Button Pressed");
      bewaterPlant();
    } else {
      Serial.println("No pinNumber parameter found.");
    }
    
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
}
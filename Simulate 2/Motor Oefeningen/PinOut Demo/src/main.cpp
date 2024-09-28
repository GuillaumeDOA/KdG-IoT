#include <Arduino.h>
#include <ezButton.h>
#include <RGBLed.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define LED_SHOOT D7
#define LED_AMMO D9
#define LED_SEMIAUTO D6

// WiFi credentials
const char *ssid = "IoT";
const char *password = "KdGIoT70!";

// MQTT broker
const char *mqtt_server = "10.6.120.8";

ezButton SHOOT(D12);
ezButton RELOAD(16);
ezButton SWITCH(17);
ezButton FIRE_MODE(D13);

RGBLed led(D2, D3, D5, RGBLed::COMMON_ANODE);

WiFiClient espClient;
PubSubClient client(espClient);

int ar_AMMO = 30;
int shotgun_AMMO = 8;

const unsigned long reloadDuration = 2000;
const unsigned long switchDuration = 1000;

bool arActive = true;
bool semiAuto = false;

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client"))
    {
      Serial.println("connected");
      // Subscribe to response topics
      client.subscribe("geweer/response");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);

  // Handle the message based on the topic
  if (String(topic) == "geweer/response")
  {
    if (messageTemp == "shoot")
    {
      Serial.println("Shoot action confirmed");
    }
    else if (messageTemp == "reload")
    {
      Serial.println("Reload action confirmed");
    }
    else if (messageTemp == "Switch")
    {
      Serial.println("Profile switch action confirmed");
    }
  }
}

void setup()
{
  pinMode(LED_SHOOT, OUTPUT);
  pinMode(LED_SEMIAUTO, OUTPUT);
  pinMode(LED_AMMO, OUTPUT);
  Serial.begin(9600);
  FIRE_MODE.setDebounceTime(100);
  SHOOT.setDebounceTime(50);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void Reload()
{
  Serial.println("RELOAD");
  client.publish("geweer/reload", "reload");
  led.setColor(RGBLed::YELLOW);
  delay(reloadDuration);
  if (arActive)
    ar_AMMO = 30;
  else
    shotgun_AMMO = 8;
}

void Switch()
{
  Serial.println("SWITCH");
  client.publish("Switch/profiles", "Switch");
  delay(switchDuration);
  arActive = !arActive;
}

void Fire()
{
  if (arActive)
  {
    if (ar_AMMO == 0)
    {
      digitalWrite(LED_SHOOT, LOW);
      return;
    }
  }
  else
  {
    if (shotgun_AMMO == 0)
    {
      digitalWrite(LED_SHOOT, LOW);
      return;
    }
  }

  digitalWrite(LED_SHOOT, HIGH);
  Serial.println("SHOOT");
  client.publish("geweer/shoot", "shoot");
  if (arActive)
  {
    if (semiAuto)
      delay(150);
    ar_AMMO -= 1;
  }
  else
  {
    shotgun_AMMO -= 1;
    delay(250);
  }
  Serial.printf("AR Ammo: %d, Shotgun Ammo: %d\n", ar_AMMO, shotgun_AMMO);
}

void loop()
{
  SHOOT.loop();
  RELOAD.loop();
  SWITCH.loop();
  FIRE_MODE.loop();
  client.loop();

  if (!client.connected())
  {
    reconnect();
  }

  if (arActive)
  {
    led.setColor(RGBLed::RED);

    if (ar_AMMO < 10)
      digitalWrite(LED_AMMO, HIGH);
    else
      digitalWrite(LED_AMMO, LOW);
  }
  else
  {
    led.setColor(RGBLed::GREEN);

    if (shotgun_AMMO < 3)
      digitalWrite(LED_AMMO, HIGH);
    else
      digitalWrite(LED_AMMO, LOW);
  }

  if (!SHOOT.getState())
  {
    Fire();
  }
  else
  {
    digitalWrite(LED_SHOOT, LOW);
  }

  if (SWITCH.isReleased())
  {
    Switch();
  }

  if (RELOAD.isReleased())
  {
    Reload();
  }

  if (FIRE_MODE.isReleased())
  {
    Serial.println("SEMI AUTO");
    semiAuto = !semiAuto;
    digitalWrite(LED_SEMIAUTO, semiAuto);
  }
}

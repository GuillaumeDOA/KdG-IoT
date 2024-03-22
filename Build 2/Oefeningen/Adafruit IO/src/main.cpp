#include "config.h"
#include <ezButton.h>

#define SEND_INTERVAL_MILLIS 5000
#define LED D13
#define POT A0

unsigned long lastSentTime = 0;

ezButton button(D13);

AdafruitIO_Feed *sensorFeed = io.feed("adafruit-io-demo.sensor");
AdafruitIO_Feed *ledFeed = io.feed("adafruit-io-demo.led");

void handleMessage(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");

  digitalWrite(LED, data->toPinLevel());
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(POT, INPUT);

  Serial.begin(115200);
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");
  io.connect();

  ledFeed->onMessage(handleMessage);

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());

  //ledFeed->get();
}

void sendMessage() {
  int val = analogRead(POT);
  Serial.print("sending -> ");
  Serial.println(val);
  sensorFeed->save(val);
}

void loop() {
  button.loop();
  io.run();
  if ((millis() - lastSentTime) > SEND_INTERVAL_MILLIS) {
    sendMessage();
    lastSentTime = millis();
  }
}
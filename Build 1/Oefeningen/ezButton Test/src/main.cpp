#include <Arduino.h>
#include <ezButton.h>

ezButton button1(2);
ezButton button2(3);

void setup()
{
  Serial.begin(9600);
  button1.setDebounceTime(100);
  button2.setDebounceTime(100);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  button1.loop(),
  button2.loop();

  if(button1.isPressed())
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
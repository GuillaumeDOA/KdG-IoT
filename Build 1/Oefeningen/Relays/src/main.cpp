#include <Arduino.h>
#include <ezButton.h>

#define relay 3
ezButton button(5);

void setup()
{
  pinMode(relay, OUTPUT);
  button.setDebounceTime(100);
}

void loop()
{
  // button.loop();
  // if (button.isPressed())
  // {
  //   digitalWrite(relay, !digitalRead(relay));
  // }
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(relay, !digitalRead(relay));
    delay(10);
  }
}
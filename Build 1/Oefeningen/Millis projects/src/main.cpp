#include <Arduino.h>
#include <ezButton.h>

#define LED1 6
#define LED2 7

long startMillisRight;
long startMillisLeft;
long currentMillis;
ezButton left(3);
ezButton right(2);

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  left.setDebounceTime(50);
  right.setDebounceTime(50);
}

void loop()
{
  left.loop();
  right.loop();

  if (right.isPressed())
  {
    digitalWrite(LED2, HIGH);
    startMillisRight = millis();
  }

  if (left.isPressed())
  {
    digitalWrite(LED1, HIGH);
    startMillisLeft = millis();
  }

  if (millis() - startMillisRight >= 5000 && digitalRead(LED2))
  {
    digitalWrite(LED2, LOW);
  }

  if (millis() - startMillisLeft >= 5000 && digitalRead(LED1))
  {
    digitalWrite(LED1, LOW);
  }
}
#include <Arduino.h>

#define RED 3
#define GREEN 5
#define BLUE 6
#define BUTTON 2
#define POT A0

long startMillis;
long currentMillis;
const long period = 500;
int seconds = 0;

int colorCount = 0;

void setup()
{
  startMillis = millis();
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(POT, INPUT);
  Serial.begin(9600);
  analogWrite(RED, 255);
  analogWrite(GREEN, 255);
  analogWrite(BLUE, 255);
}

void loop()
{
  currentMillis = millis();

  long val = map(analogRead(POT), 0, 1023, 0, 255);

  if (currentMillis - startMillis >= period)
  {
    startMillis += period;
    Serial.print("Current Value: ");
    Serial.println(val);
  }

  if (digitalRead(BUTTON) == LOW)
  {
    Serial.println("Button pressed");
    delay(300);
    if (colorCount == 0)
    {
      Serial.println("Value voor rood: " + val);
      analogWrite(RED, 255 - val);
      colorCount++;
    }
    else if (colorCount == 1)
    {
      Serial.println("Value voor groen: " + val);
      analogWrite(GREEN, 255 - val);
      colorCount++;
    }
    else if (colorCount == 2)
    {
      Serial.println("Value voor blauw: " + val);
      analogWrite(BLUE, 255 - val);
      colorCount++;
    }
    else
    {
      Serial.println("Led Reset!");
      analogWrite(RED, 255);
      analogWrite(GREEN, 255);
      analogWrite(BLUE, 255);
      colorCount = 0;
    }
  }
}
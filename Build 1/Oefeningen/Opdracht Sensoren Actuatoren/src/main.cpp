#include <Arduino.h>

#define RED 3
#define GREEN 5
#define BLUE 6
#define BUZZER 9
#define LDR A0

long startMillis;
long currentMillis;
long blinkDelay = 1000;
bool redState = false;

void setup()
{
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LDR, INPUT);
  Serial.begin(9600);
  analogWrite(RED, 255);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 255);
  startMillis = millis();
}

void RGB(int red, int green, int blue)
{
  analogWrite(RED, 255 - red);
  analogWrite(GREEN, 255 - green);
  analogWrite(BLUE, 255 - blue);
}

void loop()
{
  currentMillis = millis();
  int sensorVal = analogRead(LDR); // 280 - 730

  if (sensorVal <= 450)
  {
    if (currentMillis - startMillis >= blinkDelay)
    {
      tone(BUZZER, 500, 500);
      if (redState)
      {
        RGB(0, 0, 0);
        redState = false;
      }
      else
      {
        RGB(255, 0, 0);
        redState = true;
      }
      startMillis += blinkDelay;
    }
  }
  else
  {
    RGB(0, 255, 0);
  }
}

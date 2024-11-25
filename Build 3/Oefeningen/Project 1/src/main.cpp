/*  Voor deze opdracht heb ik gebruik gemaakt van ChatGPT voor het aanmaken van de html pagina
Bronnen:
- OpenAI (2022)- ChatGPT - https://chat.openai.com
 */
#include <Arduino.h>
#include <ezButton.h>


#define RED D6
#define GREEN D5
#define TEMPSENSOR A1

ezButton button(D2);

unsigned long startTime;

void setup()
{
  Serial.begin(115200);

  pinMode(TEMPSENSOR, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  button.setDebounceTime(100);

  startTime = millis();
}

float ReadTempSensor()
{
  int analogValue = analogRead(TEMPSENSOR);
  float voltage = (analogValue / 4095.0) * 3.3 * 1.1;
  float temp = voltage * 100.0 + 5.0;

  return temp;
}

void WriteTemp()
{
  int temp = ReadTempSensor();
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println("°C");

  if (temp >= 25)
  {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
  }
  else
  {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
  }
}

void loop()
{
  button.loop();

  if (button.isReleased())
  {
    WriteTemp();
  }

  if (millis() - startTime >= 5000)
  {
    WriteTemp();
    startTime = millis();
  }
}

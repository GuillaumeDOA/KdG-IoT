#include <Arduino.h>

// Define LM35 pin
const int lm35Pin = A2;

void setup()
{
  pinMode(lm35Pin, INPUT);
  Serial.begin(9600);
}

void loop()
{
  // Read the analog value from LM35
  int sensorValue = analogRead(lm35Pin);

  // Convert analog value to voltage (0-3.3V)
  float voltage = sensorValue * (3.3 / 4095.0);

  // Convert voltage to temperature in Celsius
  float temperatureC = voltage * 100.0;

  // Print temperature to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  // Delay for a while
  delay(1000); // Adjust delay as needed
}
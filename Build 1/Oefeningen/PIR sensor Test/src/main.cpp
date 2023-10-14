#include <Arduino.h>

int sensor = 9;  // the pin that the sensor is atteched to
int state = LOW; // by default, no motion detected
int val = 0;     // variable to store the sensor status (value)

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);   // initalize LED as an output
  pinMode(sensor, INPUT); // initialize sensor as an input
  Serial.begin(9600);     // initialize serial
}

void loop()
{
  val = digitalRead(sensor); // read sensor value
  if (val == HIGH)
  {                          // check if the sensor is HIGH
    digitalWrite(LED_BUILTIN, HIGH); // turn LED_BUILTIN ON
    delay(100);              // delay 100 milliseconds

    if (state == LOW)
    {
      Serial.println("Motion detected!");
      state = HIGH; // update variable state to HIGH
    }
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW); // turn LED_BUILTIN OFF
    delay(200);             // delay 200 milliseconds

    if (state == HIGH)
    {
      Serial.println("Motion stopped!");
      state = LOW; // update variable state to LOW
    }
  }
}
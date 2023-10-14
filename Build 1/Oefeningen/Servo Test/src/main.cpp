#include <Arduino.h>
#include <Servo.h>

// #define POT  A0

Servo myservo; // create servo object to control a servo
int pos = 0;   // variable to store the servo position

void setup()
{
  // pinMode(POT, INPUT);
  myservo.attach(9); // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}

void loop()
{
  // int reading = analogRead(POT);
  // long val = map(reading, 0, 1023, 0, 180);
  // Serial.println(val);
  for (size_t i = 0; i < 180; i++)
  {
    myservo.write(i);
    delay(10);
  }

  for (size_t i = 180; i > 0; i--)
  {
    myservo.write(i);
    delay(10);
  }
}

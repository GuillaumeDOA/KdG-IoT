#include <Arduino.h>
#include <Braccio.h>
#include <Servo.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

#define Button 14

void setup()
{
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  Braccio.begin(false);
}

void loop()
{
  for (int i = 0; i < 180; i++)
  {
    Braccio.ServoMovement(0, i, 45, 60, 60, 170, map(i, 0, 180, 90, 73));
  }

  for (int j = 180; j > 1; j--)
  {
    Braccio.ServoMovement(0, j, 45, 60, 60, 170, map(j, 0, 180, 90, 73));
  }
}

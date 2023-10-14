#include <Arduino.h>
#include <ezButton.h>

ezButton leftButton(8);
ezButton rightButton(9);

int rightLeds[] = {2, 3, 4};
int leftLeds[] = {5, 6, 7};
int leftLedCount = 0;
int rightLedCount = 0;

void setup()
{
  Serial.begin(9600);
  for (size_t i = 0; i < 3; i++)
  {
    pinMode(rightLeds[i], OUTPUT);
    pinMode(leftLeds[i], OUTPUT);
  }
  leftButton.setDebounceTime(50);
  rightButton.setDebounceTime(50);
}


void lightUpLeds(){
  for (int i = 0; i < leftLedCount; i++)
  {
    digitalWrite(leftLeds[i], HIGH);
  }

  for (int i = 0; i < rightLedCount; i++){
    digitalWrite(rightLeds[i], HIGH);
  }
  
}

void loop()
{
  rightButton.loop();
  leftButton.loop();

  if (rightButton.isPressed())
  {
    if (rightLedCount != 3)
      rightLedCount++;
  }

  if (leftButton.isPressed())
  {
    if (leftLedCount != 3)
      leftLedCount++;
  }



}
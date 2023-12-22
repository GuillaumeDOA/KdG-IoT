#include <Arduino.h>
#include <RTC.h>

int previousSecond = 0;

void setup()
{
  Serial.begin(9600);
  RTC.begin();
  RTCTime startTime(30, Month::JUNE, 2023, 13, 37, 00, DayOfWeek::WEDNESDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(startTime);
}

void loop()
{
  RTCTime currentTime;
  RTC.getTime(currentTime);

  if(currentTime.getSeconds() == previousSecond)
    return;
  previousSecond = currentTime.getSeconds();
  // Print Time (DD/mm/YY)
  Serial.print(currentTime.getDayOfMonth());
  Serial.print("/");
  Serial.print(Month2int(currentTime.getMonth()));
  Serial.print("/");
  Serial.print(currentTime.getYear());
  Serial.print(" - ");

  // Print time (HH/MM/SS)
  Serial.print(currentTime.getHour());
  Serial.print(":");
  Serial.print(currentTime.getMinutes());
  Serial.print(":");
  Serial.println(currentTime.getSeconds());
}
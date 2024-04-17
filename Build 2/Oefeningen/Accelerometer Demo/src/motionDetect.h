#include <Arduino.h>
#include <DFRobot_LIS2DW12.h>
#include <Wire.h>

#define LED D3

unsigned long ledTime, tDelay = 250, motionStart;
bool motion = false;

DFRobot_LIS2DW12_I2C acce(&Wire, 0x19);

void setup(void)
{
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
  while (!acce.begin())
  {
    Serial.println("Communication failed, check the connection and I2C address setting when using I2C communication.");
    delay(1000);
  }
  Serial.print("chip id : ");
  Serial.println(acce.getID(), HEX);

  // Software reset
  acce.softReset();
  acce.setRange(DFRobot_LIS2DW12::e2_g);
  acce.setFilterPath(DFRobot_LIS2DW12::eLPF);
  acce.setFilterBandwidth(DFRobot_LIS2DW12::eRateDiv_4);
  acce.setWakeUpDur(/*dur = */ 2);
  acce.setWakeUpThreshold(/*threshold = */ 0.4);
  acce.setPowerMode(DFRobot_LIS2DW12::eContLowPwrLowNoise1_12bit);
  acce.setActMode(DFRobot_LIS2DW12::eDetectAct);
  acce.setInt1Event(DFRobot_LIS2DW12::eWakeUp);
  acce.setDataRate(DFRobot_LIS2DW12::eRate_200hz);
  delay(100);
  ledTime = millis();
}

void loop(void)
{
  // Motion detected
  if (acce.actDetected())
  {
    motion = true;
    motionStart = millis();
    Serial.println("Activity Detected!");
  }

  if (motion && millis() - motionStart < 2000)
  {
    if (millis() - ledTime >= tDelay)
    {
      digitalWrite(LED, !digitalRead(LED));
      ledTime = millis();
    }
  }
  else
  {
    motion = false;
    digitalWrite(LED, LOW);
  }
}
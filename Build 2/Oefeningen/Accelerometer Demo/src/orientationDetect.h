#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_LIS2DW12.h>

DFRobot_LIS2DW12_I2C acce(&Wire,0x19);

int lastOrientation = 0; //No event happened

#define RED D3
#define GREEN D5

void setup(void){
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  
  Serial.begin(9600);
  while(!acce.begin()){
     Serial.println("Communication failed, check the connection and I2C address setting when using I2C communication.");
     delay(1000);
  }
  Serial.print("chip id : ");
  Serial.println(acce.getID(),HEX);
  //Chip soft reset
  acce.softReset();
  
  acce.setRange(DFRobot_LIS2DW12::e2_g);

  acce.setPowerMode(DFRobot_LIS2DW12::eContLowPwrLowNoise1_12bit);
  
  acce.setDataRate(DFRobot_LIS2DW12::eRate_200hz);

  acce.set6DThreshold(DFRobot_LIS2DW12::eDegrees60);
 
  acce.setInt1Event(DFRobot_LIS2DW12::e6D);

  delay(1000);
}

void loop(void){
    //check Changes detected in six directions
    if(acce.oriChangeDetected()){
        DFRobot_LIS2DW12::eOrient_t orientation = acce.getOrientation();
        if(lastOrientation != orientation){
            if(orientation == DFRobot_LIS2DW12::eZDown){
             Serial.println("Z is down now");
             digitalWrite(GREEN, LOW);
             digitalWrite(RED, HIGH);
            }
            if(orientation == DFRobot_LIS2DW12::eZUp){
             Serial.println("Z is up now");
             digitalWrite(GREEN, HIGH);
             digitalWrite(RED, LOW);
            }
            lastOrientation = orientation;
        }
    }
}
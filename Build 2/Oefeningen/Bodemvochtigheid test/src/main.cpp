#include <Arduino.h>



/*
Capacitive Sensor min (lucht): 3070
Capacitive Sensor max (water): 1425
    droog: 2700 - 2350          2631
    vochtig: 2350 - 1700        2100
    nat: 1700 - 1450            1535

Moisture Sensor min (lucht): 0
Moisture Sensor max (water): 2620
    droog: 1600 - 2100          2100
    vochtig: 2100 - 2400        2400
    nat: 2400 - 2600            2630
*/


const int RESISTANCE_MIN = 1600;
const int RESISTANCE_MAX = 2600;

const int CAPACITANCE_MIN = 2700;
const int CAPACITANCE_MAX = 1450;


const int CAPACITIVE = A1;
const int MOISTURE = A0;

unsigned long startTime;

int calculateMoisturePercentage(int sensorValue, int sensorMinValue, int sensorMaxValue) {
    return map(sensorValue, sensorMinValue, sensorMaxValue, 0, 100);
}

void outputSerialSoilValues(String soilSensorType, int rawValue, int percentage) {
  Serial.print(soilSensorType + "soil sensor value: ");
  Serial.print(rawValue);
  Serial.print(" - Soil moisture percentage: ");
  Serial.print(percentage);
  Serial.println("%");
}

void setup() {
  Serial.begin(9600);
}  

void loop() {
  int resSensorValue = analogRead(MOISTURE);
  int resMoisturePercentage = calculateMoisturePercentage(resSensorValue, RESISTANCE_MIN, RESISTANCE_MAX); 

  //int capSensorValue = analogRead(CAPACITIVE);
  //int capMoisturePercentage = calculateMoisturePercentage(capSensorValue, CAPACITANCE_MIN, CAPACITANCE_MAX);
  
  outputSerialSoilValues("Moisture type ", resSensorValue, resMoisturePercentage);
  //outputSerialSoilValues("Capacitance type ", capSensorValue, capMoisturePercentage);
  Serial.println("--------");

  delay(5000);
}

#include <Arduino.h>

#define LED1 8
#define LED2 9
#define LED3 10

#define POT A0

void setup(){
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(POT, INPUT);
  Serial.begin(9600);
}

void loop(){
  int reading = analogRead(POT);
  long val = map(reading ,0 ,1023 ,0 ,30);
  Serial.println(val);
  // 1024/3 = 341
  if(val >= 0 && val < 10){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  } else if (val >= 10 && val < 20){
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
  } else if(val >= 20 && val <= 30){
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
  }


}
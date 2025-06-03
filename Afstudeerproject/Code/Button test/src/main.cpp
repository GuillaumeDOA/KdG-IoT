#include <Arduino.h>

#define EXIT_BUTTON 14

void setup() {
  Serial.begin(115200);
  pinMode(EXIT_BUTTON, INPUT_PULLUP); // Configure the button pin
}

void loop() {
  int buttonState = digitalRead(EXIT_BUTTON); // Read the button state
  Serial.println(buttonState); // Print the state (HIGH or LOW)
  delay(100); // Add a small delay
}
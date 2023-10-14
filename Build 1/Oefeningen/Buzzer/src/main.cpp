#include <Arduino.h>

#define RED 4
#define GREEN 5
#define BLUE 6

void setup()
{
    pinMode(8, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    analogWrite(RED, 255);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 255);
}

void loop()
{
    analogWrite(RED, 0);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 255);
    tone(8, 440, 500);
    delay(1000);

    analogWrite(RED, 255);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 255);
    tone(8, 440, 500);
    delay(1000);

    analogWrite(RED, 255);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 0);
    tone(8, 440, 500);
    delay(1000);


}
#include <Arduino.h>

#define led1 2
#define led2 3
#define button1 4
#define button2 5

long startTimeLed1;
long startTimeLed2;
int period = 5000; // 5 seconden

void setup()
{
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(button1, INPUT);
    pinMode(button2, INPUT);
}

void loop()
{

    if (millis() - startTimeLed1 >= 5000 && digitalRead(led1))
    {
        digitalWrite(led1, LOW);
    }

    if (millis() - startTimeLed2 >= 5000 && digitalRead(led2))
    {
        digitalWrite(led2, LOW);
    }


    if (digitalRead(button1))
    {
        startTimeLed1 = millis(); // 5000
        digitalWrite(led1, HIGH);
    }

    if (digitalRead(button2))
    {
        startTimeLed2 = millis(); // 5000
        digitalWrite(led1, HIGH);
    }
}
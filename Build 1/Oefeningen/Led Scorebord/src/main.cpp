#include <Arduino.h>

#define PLAYER1 12
#define PLAYER2 13
#define RESET 11

int scoreP1 = 0;
int scoreP2 = 0;

void setup()
{
  pinMode(PLAYER1, INPUT_PULLUP);
  pinMode(PLAYER2, INPUT_PULLUP);
  pinMode(RESET, INPUT_PULLUP);
  for (size_t i = 2; i < 8; i++)
  {
    pinMode(i, OUTPUT);
  }
}

void LightUpP1()
{
  for (int i = 2; i < scoreP1 + 2; i++)
  {
    digitalWrite(i, HIGH);
  }
}

void LightUpP2()
{
  for (int i = 5; i < scoreP2 + 5; i++)
  {
    digitalWrite(i, HIGH);
  }
}

void LedDance(int player)
{
  switch (player)
  {
  case 1:
    for (size_t i = 0; i < 5; i++)
    {
      for (int i = 2; i < scoreP1 + 2; i++)
      {
        digitalWrite(i, HIGH);
      }
      delay(500);
      for (int i = 2; i < scoreP1 + 2; i++)
      {
        digitalWrite(i, LOW);
      }
      delay(500);
    }

    break;
  case 2:
    for (size_t i = 0; i < 5; i++)
    {
      for (int i = 5; i < scoreP2 + 5; i++)
      {
        digitalWrite(i, HIGH);
      }
      delay(500);
      for (int i = 5; i < scoreP2 + 5; i++)
      {
        digitalWrite(i, LOW);
      }
      delay(500);
    }
  default:
    break;
  }
}

void loop()
{
  if (digitalRead(PLAYER1) == LOW) // Increment Red LEDs
  {
    delay(300);
    if (scoreP2 != 3)
      scoreP1++;
  }

  if (digitalRead(PLAYER2) == LOW) // Inrement Green LEDs
  {
    delay(300);
    if (scoreP1 != 3)
      scoreP2++;
  }

  if (digitalRead(RESET) == LOW) // Reset Board
  {
    scoreP1 = 0;
    scoreP2 = 0;
    for (size_t i = 2; i < 8; i++)
    {
      digitalWrite(i, LOW);
    }
  }

  if (scoreP1 == 3)
    LedDance(1);
  if (scoreP2 == 3)
    LedDance(2);

  LightUpP1();
  LightUpP2();
}
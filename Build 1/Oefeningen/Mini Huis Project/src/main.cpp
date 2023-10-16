#include <Arduino.h>

// Include necessary libraries
#include <Servo.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <ezButton.h>

// Define pins
#define SERVO 10
#define RGB_RED 3
#define RGB_GREEN 5
#define RGB_BLUE 6
#define BUTTON_WHITE A0
#define BUTTON_RED A1
#define BUTTON_YELLOW A2
#define DHTPIN 2
#define MOTIONSENSOR 9

// Initialise OLED display
U8G2_SSD1306_128X64_NONAME_F_SW_I2C Display(U8G2_R0, /* clock=*/12, /* data=*/11, /* reset=*/U8X8_PIN_NONE); // Software I2C

// Initialise Temp sensor
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Initialise Servo
Servo Door;

// Initialise ezButtons
ezButton redButton(BUTTON_RED);
ezButton whiteButton(BUTTON_WHITE);
ezButton yellowButton(BUTTON_YELLOW);

// Initialise variables
float Temp;
float Humidity;
float lastTemp;
float lastHumidity;
float currentMillis;
float startMillis;
int DHTDelay = 2000;
int motionState = LOW;   // by default, no motion detected
int motionVal = 0;       // variable to store the sensor status (value)
bool RGB_Status = false; // TRue = ON; False = OFF

void setup()
{
  Door.attach(SERVO); // attach servo

  Display.begin(); // Start OLED display
  Display.setFont(u8g2_font_luBIS08_tf);
  Display.clearBuffer();

  pinMode(LED_BUILTIN, OUTPUT); // Set built in LED as OUTPUT

  pinMode(MOTIONSENSOR, INPUT);

  pinMode(DHTPIN, INPUT);
  dht.begin(); // Start Temp and Humid sensor

  redButton.setDebounceTime(100);
  yellowButton.setDebounceTime(100);
  whiteButton.setDebounceTime(50);

  analogWrite(RGB_RED, 255); // Turn off led on startup
  analogWrite(RGB_GREEN, 255);
  analogWrite(RGB_BLUE, 255);
  startMillis = millis();
}

void ReadTemp()
{
  currentMillis = millis();
  Temp = dht.readTemperature();
  Humidity = dht.readHumidity();

  if (currentMillis - startMillis >= DHTDelay)
  {
    startMillis += DHTDelay;
    Display.clearBuffer();

    if (isnan(Humidity) || isnan(Temp))
    {
      Display.setCursor(0, 10);
      Display.print("Failed to read from DHT");
      Display.sendBuffer();
      return;
    }

    Display.setCursor(0, 10);
    Display.print("Temp: ");
    Display.print(Temp);
    Display.setCursor(0, 30);
    Display.print("Humidity: ");
    Display.print(Humidity);
    Display.sendBuffer();
  }
}

void ReadMotion()
{
  motionVal = digitalRead(MOTIONSENSOR); // read sensor motionValue
  if (motionVal == HIGH)
  { // check if the sensor is HIGH
    if (motionState == LOW)
    {
      motionState = HIGH; // update variable state to HIGH
      Door.write(180);
    }
  }
  else
  {
    if (motionState == HIGH)
    {
      motionState = LOW; // update variable state to LOW
      Door.write(0);
    }
  }
}

void ChangeRGB()
{
  if (RGB_Status)
  {
    analogWrite(RGB_RED, 255);
    analogWrite(RGB_GREEN, 255);
    analogWrite(RGB_BLUE, 255);
  }

  if (whiteButton.isPressed()) // RGB Value for white = 255 255 255
  {
    analogWrite(RGB_RED, 0);
    analogWrite(RGB_GREEN, 0);
    analogWrite(RGB_BLUE, 0);
    RGB_Status = true;
  }

  if (yellowButton.isPressed()) // RGB Value for yellow = 255 255 0
  {
    analogWrite(RGB_RED, 0);
    analogWrite(RGB_GREEN, 0);
    analogWrite(RGB_BLUE, 255);
    RGB_Status = true;
  }
  if (redButton.isPressed()) // RGB Value for red = 255 0 0
  {
    analogWrite(RGB_RED, 0);
    analogWrite(RGB_GREEN, 255);
    analogWrite(RGB_BLUE, 255);
    RGB_Status = true;
  }
}

void loop()
{
  redButton.loop();
  whiteButton.loop();
  yellowButton.loop();

  ReadTemp();
  ReadMotion();
  ChangeRGB();
}

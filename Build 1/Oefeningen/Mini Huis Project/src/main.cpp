#include <Arduino.h>

// Include necessary libraries
#include <Servo.h>
#include <DHT.h>
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
#define MOTIONSENSOR 13
#define OLED_CLOCK 19 // YELLOW WIRE
#define OLED_DATA 18  // WHITE WIRE
#define BUZZER 4
#define DOORBELL_BUTTON A3

// Initialise OLED display
U8G2_SSD1306_128X64_NONAME_F_SW_I2C Display(U8G2_R0, OLED_CLOCK, OLED_DATA, /* reset=*/U8X8_PIN_NONE);

// Initialise Temp sensor
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Initialise Servo
Servo Door;

// Initialise ezButtons
ezButton redButton(BUTTON_RED);
ezButton whiteButton(BUTTON_WHITE);
ezButton yellowButton(BUTTON_YELLOW);
ezButton doorBell(DOORBELL_BUTTON);

// Initialise variables
float Temp;
float Humidity;
unsigned long startTimeTemp;
unsigned int DHTDelay = 2000;
int motionState = LOW;   // Default = no motion detected
int motionVal = 0;       // Variable to store the sensor status (value)
bool RGB_Status = false; // True = ON; False = OFF

void setup()
{
  Door.attach(SERVO); // attach servo

  Display.begin();                       // Start OLED display
  Display.setFont(u8g2_font_luBIS08_tf); // Set font

  // Set correct pinmodes
  pinMode(MOTIONSENSOR, INPUT);
  pinMode(RGB_BLUE, OUTPUT);
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(DHTPIN, INPUT);

  dht.begin(); // Start Temp and Humid sensor

  // Set debouncetimes for buttons
  redButton.setDebounceTime(100);
  yellowButton.setDebounceTime(100);
  whiteButton.setDebounceTime(100);
  doorBell.setDebounceTime(100);

  // Turn off led on startup
  analogWrite(RGB_RED, 255);
  analogWrite(RGB_GREEN, 255);
  analogWrite(RGB_BLUE, 255);

  startTimeTemp = millis();
  Door.write(170); // Close door on startup
}

void ReadTemp()
{ // Only update display every couple seconds (DHTDelay)
  if (millis() - startTimeTemp >= DHTDelay)
  {
    // Read sensor
    Temp = dht.readTemperature();
    Humidity = dht.readHumidity();

    // If not a number return from function
    if (isnan(Temp) || isnan(Humidity))
      return;

    Display.firstPage();
    do
    { // Write values to display
      Display.setCursor(0, 10);
      Display.print("Temp: ");
      Display.print(Temp);
      Display.setCursor(0, 30);
      Display.print("Humidity: ");
      Display.print(Humidity);
    } while (Display.nextPage());
    startTimeTemp += DHTDelay; // Increment startTimeTemp
  }
}

void ReadMotion()
{
  motionVal = digitalRead(MOTIONSENSOR); // Read motionsensor
  if (motionVal == HIGH)
  { // check if the sensor is HIGH and state is LOW
    if (motionState == LOW)
    {
      motionState = HIGH; // Door is open
      Door.write(30);
    }
  }
  else
  { // If sensor is LOW and state is HIGH
    if (motionState == HIGH)
    {
      motionState = LOW; // Door is closed
      Door.write(170);
    }
  }
}

void ChangeRGB()
{
  if ((whiteButton.isPressed() || yellowButton.isPressed() || redButton.isPressed()) && RGB_Status)
  { // If a button is pressed while LED is on -> turn off LED
    analogWrite(RGB_RED, 255);
    analogWrite(RGB_GREEN, 255);
    analogWrite(RGB_BLUE, 255);
    RGB_Status = false;
    return;
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

void DoorBell()
{
  if (doorBell.isPressed())
  { // Sound doorbell when button is pressed
    tone(BUZZER, 200, 300);
    /*
      Wou een liedje afspelen met de ezBuzzer library, maar vanaf dat ik deze code implementeerde werkte de
      van het programma niet meer
    */
  }
}

void loop()
{
  // Loop over buttons
  redButton.loop();
  whiteButton.loop();
  yellowButton.loop();
  doorBell.loop();

  DoorBell();
  ReadTemp();
  ReadMotion();
  ChangeRGB();
}

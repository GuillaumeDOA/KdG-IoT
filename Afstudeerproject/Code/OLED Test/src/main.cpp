#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED reset pin (not used with some OLEDs like SSD1315)
#define OLED_RESET    -1

// Use I2C address 0x3C (most common for SSD1306/SSD1315)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// initialize the display
void initDisplay(){
  Serial.println(F("Initializing OLED display..."));
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1315 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);      
  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE);
  display.println(F("Server Room"));
  display.setCursor(0, 16);
  display.println(F("Booting up System"));
  display.display();
}

void writeToDisplay(const char* message) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Server Room"));
  display.setCursor(0, 16);
  display.println(message);
  display.display();
}

void setup() {
  Serial.begin(115200);

  initDisplay();
  delay(2000); // Wait for 2 seconds to show the boot message
  writeToDisplay("System Ready");
  delay(2000); // Wait for 2 seconds to show the ready message
  writeToDisplay("Een hele lange string die ik wil laten zien op de display maar ik weet niet of dat gaat lukken");
}

void loop() {
  // You can update display content here
}

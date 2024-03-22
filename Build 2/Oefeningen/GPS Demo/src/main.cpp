#include <Arduino.h>
#include <HardwareSerial.h>
//#include <SoftwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17

// Define buffer size
const int BUFFER_SIZE = 64;

// Define the buffer array for the NMEA messages
unsigned char buffer[BUFFER_SIZE];

// Serial1 on most ESP32 boards 
HardwareSerial gpsSerial(1); 
//SoftwareSerial gpsSerial(RX_PIN, TX_PIN);

void setup() {
    Serial.begin(9600);
    
    //Initialise gpsSerial
    gpsSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    //gpsSerial.begin(9600);
}
 
void loop() {
  // get the number of bytes from the serial port that are ready to be read 
  int gpsSerialAvailableBytes = gpsSerial.available();
  
  // if data is ready to be read
  if (gpsSerialAvailableBytes > 0) {
    // reading data into char array
    for (int i=0; i < min(gpsSerialAvailableBytes, BUFFER_SIZE); i++) { 
      // writing data into array
      buffer[i] = gpsSerial.read();
    }
        
    // if data transmission ends, write buffer to hardware serial port
    Serial.write(buffer, min(gpsSerialAvailableBytes, BUFFER_SIZE));

    // clear the buffer array for reuse                 
    for (int i=0; i < BUFFER_SIZE; i++) { 
      buffer[i] = 0;
    }
  }
}
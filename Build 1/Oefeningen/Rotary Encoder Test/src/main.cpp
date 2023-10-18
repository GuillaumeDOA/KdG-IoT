// Rotary Encoder Inputs
#include <Arduino.h>
#include <Servo.h>

#define CLK 7
#define DT 8
// #define SW 4

Servo myservo;

int counter = 0;
int currentStateCLK;
int lastStateCLK;
// unsigned long lastButtonPress = 0;

void setup()
{
	// Set encoder pins as inputs
	pinMode(CLK, INPUT);
	pinMode(DT, INPUT);
	// pinMode(SW, INPUT_PULLUP);

	// Setup Serial Monitor
	Serial.begin(9600);
	// Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);
	myservo.attach(9);
}

void loop()
{
	// Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
	{

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK)
		{
			if (counter != 0)
				counter-=10;
		}
		else
		{
			// Encoder is rotating CW so increment
			if (counter != 180)
				counter+=10;
		}
		Serial.print("Angle: ");
		Serial.println(counter);
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;

	// Read the button state
	// int btnState = digitalRead(SW);

	// If we detect LOW signal, button is pressed
	// if (btnState == LOW)
	// {
	// 	// if 50ms have passed since last LOW pulse, it means that the
	// 	// button has been pressed, released and pressed again
	// 	if (millis() - lastButtonPress > 50)
	// 	{
	// 		Serial.println("Button pressed!");
	// 	}

	// 	// Remember last button press event
	// 	lastButtonPress = millis();
	// }

	// Put in a slight delay to help debounce the reading

	myservo.write(counter);

	delay(1);
}
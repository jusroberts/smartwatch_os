/*
 Name:		smartwatch_os.ino
 Created:	1/22/2016 8:37:06 PM
 Author:	jusroberts
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <math.h>
#include <accelerometer.h>


// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
Accelerometer accelerometer = Accelerometer(A0, A1, A2);

enum State {
	menu,
	time,
	stopwatch,
	activity
};

State watchState = State::time;
State previousWatchState;

volatile bool button_1_triggered;

//PINS
#define BUTTON_1 2
#define BUTTON_2 12

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
B00000001, B11000000,
B00000001, B11000000,
B00000011, B11100000,
B11110011, B11100000,
B11111110, B11111000,
B01111110, B11111111,
B00110011, B10011111,
B00011111, B11111100,
B00001101, B01110000,
B00011011, B10100000,
B00111111, B11100000,
B00111111, B11110000,
B01111100, B11110000,
B01110000, B01110000,
B00000000, B00110000 };

void setup() {
	Serial.begin(9600);

	display.begin();
	// init done

	// you can change the contrast around to adapt the display
	// for the best viewing!
	display.setContrast(60);

	display.display(); // show splashscreen
	delay(2000);
	display.clearDisplay();   // clears the screen and buffer

	watchState = State::stopwatch;
	previousWatchState = watchState;

	pinMode(BUTTON_1, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_1), setButtonTriggers, RISING);
	button_1_triggered = false;
}


void loop() {
	delay(10);
	accelerometer.checkForActivity();
	if (previousWatchState != watchState) {
		display.clearDisplay();
	}
	previousWatchState = watchState;

	switch (watchState) {
		case time: displayTime(); break;
		case stopwatch: displayStopwatch(); break;
		case activity: displayActivity(); break;
	}

	display.display();
}

void displayActivity() {
	display.setTextSize(1);
	display.println("Activity");
	display.setTextSize(3);
	display.println(String(accelerometer.getActivity()));
}

void displayTime() {
	display.setTextSize(3);
	display.setTextColor(BLACK);
	display.setCursor(0, 0);
	display.println("07");
	display.println("30");

	display.setCursor(3 * 6 * 2, 0);
	display.setTextSize(2);
	display.print("pm");
}

unsigned long stopwatch_start_time = 0;
bool stopwatch_active = false;
long elapsed_time = 0;
void displayStopwatch() {
	if (stopwatch_active) {
		elapsed_time = millis() - stopwatch_start_time;

		if (getButtonTriggered()) {
			stopwatch_active = false;
		}
	}
	else {
		if (getButtonTriggered()) {
			stopwatch_start_time = millis();
			stopwatch_active = true;
		}
	}

	display.clearDisplay();
	display.setCursor(0, 0);
	display.setTextSize(1);
	display.println("Stop Watch");
	printStopwatchStrings(elapsed_time);
}

void printStopwatchStrings(unsigned long passed_time) {
	int display_milliseconds = (int)(passed_time % 1000);
	int display_seconds, display_minutes, display_hours = 0;

	if (passed_time > 999) {
		display.setTextSize(2);
		long seconds = ((passed_time - display_milliseconds) / 1000);
		display_seconds = (int)(seconds % 60);
		if (seconds > 59) {
			long minutes = ((seconds - display_seconds) / 60);
			display_minutes = (int)(minutes % 60);
			if (minutes > 59) {
				int display_hours = ((minutes - display_minutes) / 60);
				display.print(String(display_hours) + "h");
			}
			display.println(String(display_minutes) + "m");
		}
		display.println(String(display_seconds) + "s");
	}
	display.setTextSize(1);
	String preMilliString = "";
	if (display_milliseconds < 100) {
		preMilliString += "0";
		if (display_milliseconds < 10) {
			preMilliString += "0";
		}
	}
	display.println(preMilliString + String(display_milliseconds));
}

bool getButtonTriggered() {
	if (button_1_triggered) {
		button_1_triggered = false;
		return true;
	}
	return false;
}

void setButtonTriggers() {
	button_1_triggered = true;
}
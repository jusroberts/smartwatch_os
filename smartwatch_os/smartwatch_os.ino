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


// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

enum State {
	menu,
	time,
	stopwatch
};

State watchState = State::time;
State previousWatchState;

int prev_button_state [2];

//PINS
#define BUTTON_1 11
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
	pinMode(BUTTON_2, INPUT_PULLUP);
	updateButtonStates();

}


void loop() {
	delay(10);
	if (previousWatchState != watchState) {
		display.clearDisplay();
	}
	previousWatchState = watchState;

	switch (watchState) {
		case time: displayTime(); break;
		case stopwatch: displayStopwatch(); break;
	}

	display.display();
	updateButtonStates();
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

		if (buttonReleased(BUTTON_1)) {
			stopwatch_active = false;
		}
	}
	else {
		if (buttonReleased(BUTTON_1)) {
			stopwatch_start_time = millis();
			stopwatch_active = true;
		}
	}

	display.clearDisplay();
	display.setTextSize(2);
	display.setCursor(0, 0);
	display.print(formatStopwatchString(elapsed_time));
}

String formatStopwatchString(unsigned long passed_time) {
	int display_milliseconds = (int)(passed_time % 1000);
	int display_seconds, display_minutes, display_hours = 0;
	String formatted_string = String(display_milliseconds);

	if (passed_time > 999) {
		long seconds = ((passed_time - display_milliseconds) / 1000);
		display_seconds = (int)(seconds % 60);
		formatted_string = String(display_seconds) + "." + formatted_string;
		if (seconds > 59) {
			long minutes = ((seconds - display_seconds) / 1000);
			display_minutes = (int)(minutes % 60);
			formatted_string = String(display_minutes) + ":" + formatted_string;
			if (minutes > 59) {
				int display_hours = ((minutes - display_minutes) / 60);
				formatted_string = String(display_hours) + "h" + formatted_string;
			}
		}
	}

	return formatted_string;
}


void updateButtonStates() {
	prev_button_state[BUTTON_1] = digitalRead(BUTTON_1);
	prev_button_state[BUTTON_2] = digitalRead(BUTTON_2);
}

bool buttonReleased(int button) {
	return (prev_button_state[button] == 0 && digitalRead(button) == 1);
}
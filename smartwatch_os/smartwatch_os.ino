/*
 Name:		smartwatch_os.ino
 Created:	1/22/2016 8:37:06 PM
 Author:	jusroberts
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
//#include <Adafruit_SSD1306.h>

//// If using software SPI (the default case):
//#define OLED_MOSI   9
//#define OLED_CLK   10
//#define OLED_DC    11
//#define OLED_CS    12
//#define OLED_RESET 13
//Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
////Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
//
////// Uncomment this block to use hardware SPI 
////#define OLED_DC     9
////#define OLED_CS     10
////#define OLED_RESET  12
////Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
//
//#define NUMFLAKES 10
//#define XPOS 0
//#define YPOS 1
//#define DELTAY 2
//
//#define LOGO16_GLCD_HEIGHT 16 
//#define LOGO16_GLCD_WIDTH  16 
//static const unsigned char PROGMEM logo16_glcd_bmp[] =
//{ B00000000, B11000000,
//B00000001, B11000000,
//B00000001, B11000000,
//B00000011, B11100000,
//B11110011, B11100000,
//B11111110, B11111000,
//B01111110, B11111111,
//B00110011, B10011111,
//B00011111, B11111100,
//B00001101, B01110000,
//B00011011, B10100000,
//B00111111, B11100000,
//B00111111, B11110000,
//B01111100, B11110000,
//B01110000, B01110000,
//B00000000, B00110000 };
//
//#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
//#endif
//
//void setup() {
//	Serial.begin(9600);
//
//	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
//	display.begin(SSD1306_SWITCHCAPVCC);
//	// init done
//
//	// Show image buffer on the display hardware.
//	// Since the buffer is intialized with an Adafruit splashscreen
//	// internally, this will display the splashscreen.
//	display.display();
//	delay(2000);
//	display.clearDisplay();
//
//	// text display tests
//	display.setTextSize(4);
//	display.setTextColor(WHITE);
//	display.setCursor(0, 0);
//	display.println("07:30");
//	display.display();
//}
//
//
//void loop() {
//
//}


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

	previousWatchState = watchState;

}


void loop() {
	if (previousWatchState != watchState) {
		display.clearDisplay();
	}
	previousWatchState = watchState;

	switch (watchState) {
		case time: displayTime(); break;
		case stopwatch: displayStopwatch(); break;
	}
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
	display.display();
}

int milliseconds = 0;
void displayStopwatch() {

}



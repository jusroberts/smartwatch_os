#include "Arduino.h"
#include "accelerometer.h"

Accelerometer::Accelerometer(int pin_x, int pin_y, int pin_z) {
	pinMode(pin_x, INPUT);
	pinMode(pin_y, INPUT);
	pinMode(pin_z, INPUT);

	_pin_x = pin_x;
	_pin_y = pin_y;
	_pin_z = pin_z;

	_midpoint = 330;
	_activity = 0;
	_threshold = 100;

	_begin_checking_again = 0;
	_wait_time = 50;
}

void Accelerometer::checkForActivity() {
	if (_begin_checking_again > millis()) {
		return;
	}

	int x = analogRead(_pin_x) - _midpoint;
	int y = analogRead(_pin_y) - _midpoint;
	int z = analogRead(_pin_z) - _midpoint;

	double acceleration_vector = sqrt(x*x + y*y + z*z);
	if (acceleration_vector > _threshold) {
		_activity++;
		_begin_checking_again = millis() + _wait_time;
	}
}

int Accelerometer::getActivity() {
	return _activity;
}
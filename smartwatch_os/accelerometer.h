#pragma once
#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "Arduino.h"

class Accelerometer
{
public:
	Accelerometer(int pin_x, int pin_y, int pin_z);
	void checkForActivity();
	int getActivity();
private:
	int _activity;
	int _pin_x, _pin_y, _pin_z;
	int _midpoint;
	int _threshold;
	long _begin_checking_again;
	int _wait_time;
};

#endif // !ACCELEROMETER_H

#include "../include/Delay.hpp"
#include <iostream>
Delay::Delay(int frameRate)
{
	this->frameRate = frameRate;
	this->lastCall = clock();
}

Delay::~Delay()
{
}

void Delay::delay() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frameRate - (clock()- lastCall) ));
	lastCall = clock();
}

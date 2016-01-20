#include <ctime>
#include <thread>

#ifndef DELAY_HPP
#define DELAY_HPP

class Delay
{
public:
	Delay(int frameRate);
	~Delay();

	void delay();

private:
	int frameRate;
	clock_t lastCall;
};

#endif
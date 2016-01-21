#include <ctime>
#include <thread>

#ifndef DELAY_HPP
#define DELAY_HPP
/*Classe servant � faire des pauses de fr�quence constante
 */
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
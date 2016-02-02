#include "Data\MiscData.hpp"

Axe int2Axe(int i) {
	switch (i) {
	case Axe::x:
		return Axe::x;
		break;
	case Axe::y:
		return Axe::y;
		break;
	case Axe::z:
		return Axe::z;
		break;
	default:
		throw DataError("DataError: int2Axe(int) can't convert " + std::to_string(i));
	}
}

//surcharge d'opérateurs pour coord
int& obj::coord::operator[](unsigned int i)
{
	switch (i)
	{
	case 1:
		return this->x;
		break;
	case 2:
		return this->y;
		break;
	case 3:
		return this->z;
		break;
	default:
		throw DataError("DataError: the specified component" + std::to_string(i) + "doesn't exist");
	}
}
bool obj::coord::operator==(obj::coord const& b)
{
	//Teste si a.m_heure == b.m_heure etc.  
	if (this->x == b.x &&this->y == b.y && this->z == b.z)
		return true;
	else
		return false;
}
obj::coord& obj::coord::operator*(const int &b)
{
	this->x *= b;
	this->y *= b;
	this->z *= b;
	return *this;
}

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
void obj::coord::setCoord(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Axe nextAxe(Axe a) {
	return int2Axe((a + 1) % 3);
}

//surcharge d'opérateurs pour coord
int& obj::coord::operator[](unsigned int i)
{
	switch (i)
	{
	case 0:
		return this->x;
		break;
	case 1:
		return this->y;
		break;
	case 2:
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
dir normale(dir v1, dir v2) {
	Axe a = nextAxe(v1.first);
	if (a == v2.first) {
		a = nextAxe(a);
	}
	return dir(a, (v1.second && v2.second) || (!v1.second && !v2.second));
}
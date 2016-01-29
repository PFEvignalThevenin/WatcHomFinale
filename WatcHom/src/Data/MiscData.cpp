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
bool operator==(obj::coord const& a, obj::coord const& b)
{
	//Teste si a.m_heure == b.m_heure etc.  
	if (a.x == b.x && a.y == b.y && a.z == b.z)
		return true;
	else
		return false;
}
obj::coord operator*(const float &a, const obj::coord &b)
{
	obj::coord result;
	result.x = a * b.x;
	result.y = a * b.y;
	result.z = a * b.z;
	return result;
}
obj::coord operator+(const obj::coord &a, const obj::coord &b)
{
	obj::coord result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

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

float & obj::Vertex::at(unsigned int i)
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

//surcharge d'opérateurs pour Vertex
float& obj::Vertex::operator[](unsigned int i)
{
	return this->at(i);
}

bool obj::Vertex::operator==(obj::Vertex const& b)
{
	//Teste si a.m_heure == b.m_heure etc.  
	if (this->x == b.x &&this->y == b.y && this->z == b.z)
		return true;
	else
		return false;
}
obj::Vertex obj::Vertex::operator*(const float &b)
{
	obj::Vertex result;
	result.x = this->x * b;
	result.y = this->y * b;
	result.z = this->z * b;
	return result;
}
obj::Vertex obj::Vertex::operator+(const obj::Vertex &b)
{
	obj::Vertex result;
	result.x = this->x + b.x;
	result.y = this->y + b.y;
	result.z = this->z + b.z;
	return result;
}

obj::Vertex obj::Vertex::translation(dir xyz, float dist)
{
	Axe axe = xyz.first;
	bool direct = xyz.second;
	this->at(axe) += (direct) ? dist : -dist;
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
obj::coord obj::coord::operator*(const int &b)
{
	obj::coord result;
	result.x = this->x * b;
	result.y = this->y * b;
	result.z = this->z * b;
	return result;
}

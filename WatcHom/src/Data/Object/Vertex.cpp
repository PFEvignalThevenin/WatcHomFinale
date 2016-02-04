#include "Data\Object\Vertex.hpp"


float & obj::Vertex::at(unsigned int i)
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
obj::Vertex& obj::Vertex::operator*(const float &b)
{
	this->x *= b;
	this->y *= b;
	this->z *= b;
	return *this;
}
obj::Vertex& obj::Vertex::operator+(const obj::Vertex &b)
{
	this->x += b.x;
	this->y += b.y;
	this->z += b.z;
	return *this;
}

obj::Vertex& obj::Vertex::translation(dir xyz, float dist)
{
	this->at(xyz.first) += (xyz.second) ? dist : -dist;
	return *this;
}
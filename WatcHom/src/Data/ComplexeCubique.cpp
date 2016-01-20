#ifndef COMPLEXE_CUBIQUE_TPP
#define COMPLEXE_CUBIQUE_TPP

#include "Data\ComplexeCubique.hpp"
#include <iostream>
using namespace std;

ComplexeCubique::ComplexeCubique() : ComplexeCubique(0, 0, 0)
{
}

ComplexeCubique::ComplexeCubique(int xSize, int ySize, int zSize) : StructureCubique<char>(xSize,  ySize,  zSize)
{
}

ComplexeCubique::~ComplexeCubique()
{
}
//dimension du cube à la position donnée
int ComplexeCubique::dim(int pos) {
	coord c= pos2coord(pos);
	return dim(c.x, c.y, c.z);
}
int ComplexeCubique::dim(int x, int y, int z) {
	if (z % 2 == 0) {
		if (x % 2 == 0 && y % 2 == 0) return 0;
		else if (x % 2 == 1 && y % 2 == 1) return 2;
		else return 1;
	}
	else {
		if (x % 2 == 0 && y % 2 == 0) return 1;
		else if (x % 2 == 1 && y % 2 == 1) return 3;
		else return 2;
	}
}
//adjacences
std::list<int> ComplexeCubique::boundary(int pos) {
	coord c = pos2coord(pos);
	return boundary(c.x, c.y, c.z);
}
std::list<int> ComplexeCubique::coboundary(int pos) {
	coord c = pos2coord(pos);
	return coboundary(c.x, c.y, c.z);
}
std::list<int> ComplexeCubique::boundary(int x, int y, int z) {
	list<int> ret;
	if (x % 2 == 1) {
		ret.push_back(coord2pos(x+1, y, z));
		ret.push_back(coord2pos(x-1, y, z));
	}
	if (y % 2 == 1) {
		ret.push_back(coord2pos(x, y+1, z));
		ret.push_back(coord2pos(x, y-1, z));
	}
	if (z % 2 == 1) {
		ret.push_back(coord2pos(x, y, z+1));
		ret.push_back(coord2pos(x, y, z-1));
	}
	return ret;
}
std::list<int> ComplexeCubique::coboundary(int x, int y, int z) {
	list<int> ret;
	if (x % 2 == 0) {
		ret.push_back(coord2pos(x + 1, y, z));
		ret.push_back(coord2pos(x - 1, y, z));
	}
	if (y % 2 == 0) {
		ret.push_back(coord2pos(x, y + 1, z));
		ret.push_back(coord2pos(x, y - 1, z));
	}
	if (z % 2 == 0) {
		ret.push_back(coord2pos(x, y, z + 1));
		ret.push_back(coord2pos(x, y, z - 1));
	}
	return ret;
}
//accessseurs : primal
void ComplexeCubique::setPrimal(bool p_primal) {
	primal = p_primal;
}
bool ComplexeCubique::isPrimal() {
	return primal;
}

#endif
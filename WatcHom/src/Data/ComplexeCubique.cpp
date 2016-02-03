#ifndef COMPLEXE_CUBIQUE_TPP
#define COMPLEXE_CUBIQUE_TPP

#include "Data\ComplexeCubique.hpp"
#include "Data\MiscData.hpp"
#include <iostream>
using namespace std;

ComplexeCubique::ComplexeCubique() : ComplexeCubique(0, 0, 0)
{
}

ComplexeCubique::ComplexeCubique(int xSize, int ySize, int zSize) : StructureCubique<bool>(xSize,  ySize,  zSize)
{
}

ComplexeCubique::~ComplexeCubique()
{
}
//dimension du cube à la position donnée
int ComplexeCubique::dim(int pos) {
	obj::coord c= pos2coord(pos);
	return dim(c.x, c.y, c.z);
}
int ComplexeCubique::dim(int x, int y, int z) {//primal change qqchose?
	int d=0;
	d+=z%2;
	d+=y%2;
	d+=x%2;
	return d;
}
//adjacences
std::list<int> ComplexeCubique::boundary(int pos) {
	obj::coord c = pos2coord(pos);
	return boundary(c.x, c.y, c.z);
}
std::list<int> ComplexeCubique::coboundary(int pos) {
	obj::coord c = pos2coord(pos);
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
		if (isOccupied(x + 1, y, z)) ret.push_back(coord2pos(x + 1, y, z));
		if (isOccupied(x - 1, y, z))ret.push_back(coord2pos(x - 1, y, z));
	}
	if (y % 2 == 0) {
		if (isOccupied(x, y + 1, z))ret.push_back(coord2pos(x, y + 1, z));
		if (isOccupied(x, y - 1, z))ret.push_back(coord2pos(x, y - 1, z));
	}
	if (z % 2 == 0) {
		if (isOccupied(x, y, z + 1))ret.push_back(coord2pos(x, y, z + 1));
		if (isOccupied(x, y, z - 1))ret.push_back(coord2pos(x, y, z - 1));
	}
	return ret;
}
std::list<int> ComplexeCubique::neighbors(int x, int y , int z)
{
	//liste contenant les voisins
	list <int> voisins;
	//suivant x
	if (x + 2 < size[0] && get(x+2,y,z)) 		
		voisins.push_back(coord2pos(x + 2,y,z));
	if (x >= 2 && get(x-2,y,z))
		voisins.push_back(coord2pos(x - 2, y, z));
	//suivant y
	if (y +2 < size[1] && get(x, y+2, z))
		voisins.push_back(coord2pos(x, y+2, z));
	if (y >= 2 && get(x, y - 2, z))
		voisins.push_back(coord2pos(x, y - 2, z));
	//suivant z
	if (z + 2 <size[2] && get(x, y, z + 2))
		voisins.push_back(coord2pos(x, y, z + 2));
	if (z >= 2 && get(x, y, z - 2))
		voisins.push_back(coord2pos(x, y, z - 2));

	return voisins;
}
std::list<int> ComplexeCubique::neighbors(int pos)
{
	obj::coord XYZ = pos2coord(pos);
	return neighbors(XYZ.x,XYZ.y,XYZ.z);
}
//accessseurs : primal
void ComplexeCubique::setPrimal(bool p_primal) {
	primal = p_primal;
}
bool ComplexeCubique::isPrimal() {
	return primal;
}

#endif

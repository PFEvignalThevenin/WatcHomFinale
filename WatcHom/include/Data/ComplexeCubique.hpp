#pragma once
#ifndef COMPLEXE_CUBIQUE_HPP
#define COMPLEXE_CUBIQUE_HPP

#include "Data\StructureCubique.hpp"
#include "Data\Complexe.hpp"

class ComplexeCubique : public StructureCubique<char>, public Complexe
{
public:
	ComplexeCubique();
	ComplexeCubique(int xSize, int ySize, int zSize);
	virtual ~ComplexeCubique();

	//dimension du cube
	//performances tips : pos délègue à coord
	int dim(int pos);
	int dim(int x, int y, int z);
	//voisins et covoisins
	//performances tips : pos délègue à coord 
	std::list<int> boundary(int pos) override;
	std::list<int> boundary(int x, int y, int z);
	std::list<int> coboundary(int pos) override;
	std::list<int> coboundary(int x, int y, int z);
	//primalité du complexe
	void setPrimal(bool p_primal);
	bool isPrimal();			
private:
	bool primal;

};

#endif
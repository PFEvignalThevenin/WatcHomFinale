#pragma once
#ifndef COMPLEXE_CUBIQUE_HPP
#define COMPLEXE_CUBIQUE_HPP

#include "Data\StructureCubique.hpp"
#include "Data\Complexe.hpp"
#include <memory>

/* ComplexeCubique.
 * Complexe dont les constituant sont des cubes, et sont donc organisés selon une structure cubique
 * Beaucoup de fonctions sont surchargées pour un accès selon les coordonées 3D ou l'index d'entité. (voir Classe Complexe)
 */
class ComplexeCubique : public StructureCubique<bool>, public Complexe
{
public:
	typedef std::shared_ptr<ComplexeCubique> Ptr;
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
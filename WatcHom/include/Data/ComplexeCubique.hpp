#pragma once
#ifndef COMPLEXE_CUBIQUE_HPP
#define COMPLEXE_CUBIQUE_HPP

#include "Data\StructureCubique.hpp"
#include "Data\Complexe.hpp"
#include <memory>

/* ComplexeCubique.
 * Complexe dont les constituant sont des cubes, et sont donc organis�s selon une structure cubique
 * Beaucoup de fonctions sont surcharg�es pour un acc�s selon les coordon�es 3D ou l'index d'entit�. (voir Classe Complexe)
 */
class ComplexeCubique : public StructureCubique<bool>, public Complexe
{
public:
	typedef std::shared_ptr<ComplexeCubique> Ptr;
	ComplexeCubique();
	ComplexeCubique(int xSize, int ySize, int zSize);
	virtual ~ComplexeCubique();

	//dimension du cube
	//performances tips : pos d�l�gue � coord
	int dim(int pos);
	int dim(int x, int y, int z);
	//voisins et covoisins
	//performances tips : pos d�l�gue � coord 
	std::list<int> boundary(int pos) override;
	std::list<int> boundary(int x, int y, int z);
	std::list<int> coboundary(int pos) override;
	std::list<int> coboundary(int x, int y, int z);
	//primalit� du complexe
	void setPrimal(bool p_primal);
	bool isPrimal();			
private:
	bool primal;

};

#endif
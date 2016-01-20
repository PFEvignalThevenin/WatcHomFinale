#ifndef STRUCTURE_CUBIQUE_TPP
#define STRUCTURE_CUBIQUE_TPP

#include <Data/StructureCubique.hpp>
#include <algorithm>

//************************************
//*****Constructeur & Destructeur*****
//************************************
template<typename D>
StructureCubique<D>::StructureCubique() : StructureCubique(0,0,0){}
template<typename D>
StructureCubique<D>::StructureCubique(int xSize, int ySize, int zSize)
{
	int s[3] = { xSize, ySize, zSize };
	setDataSize(s);
}
//copie
template<typename D>
StructureCubique<D>::StructureCubique(StructureCubique &copy) : StructureCubique(copy.size[Axe::x], copy.size[Axe::y], copy.size[Axe::z])
{
	this->copy(copy);
}
//destructeur
template<typename D>
StructureCubique<D>::~StructureCubique()
{
	for (int i = 0; i < size[Axe::z]; i++) {
		for (int j = 0; j < size[Axe::y]; j++) {
			delete[] data[i][j];
		}
		delete[] data[i];
	}
	delete[] data;
}
//************************************
//*********Fonctions d'accès**********
//************************************
template<typename D>
bool StructureCubique<D>::isEmpty(int x, int y, int z) const {
	return (data[z][y][x]) ? false : true;
}
template<typename D>
bool StructureCubique<D>::isOccupied(int x, int y, int z) const {
	return (data[z][y][x]) ? true : false;
}
template<typename D>
void StructureCubique<D>::set(int x, int y, int z, D val) {
	if (without(x, y, z)) return;
	data[z][y][x] = val;
}
template<typename D>
D StructureCubique<D>::get(int x, int y, int z) const {
	return data[z][y][x];
}
//avec pos
template<typename D>
bool StructureCubique<D>::isEmpty(int pos) const {
	coord c = pos2coord(pos);
	return isEmpty(c.x, c.y, c.z);
}
template<typename D>
bool StructureCubique<D>::isOccupied(int pos) const {
	coord c = pos2coord(pos);
	return isOccupied(c.x, c.y, c.z);
}
template<typename D>
void StructureCubique<D>::set(int pos, D val) {
	if (without(x, y, z)) return;
	coord c = pos2coord(pos);
	set(c.x, c.y, c.z, val);
}
template<typename D>
D StructureCubique<D>::get(int pos) const {
	coord c = pos2coord(pos);
	return get(c.x, c.y, c.z);
}
//metadatas
template<typename D>
int StructureCubique<D>::getSize(Axe axe) const {
	return size[axe];
}
template<typename D>
int StructureCubique<D>::nbrCubes() const {
	int tot = 1;
	for (int i = 0; i < 3; i++) {
		tot *= size[i];
	}
	return tot;
}
//check position
template<typename D>
bool StructureCubique<D>::without(int x,int y, int z)  const {
	return without(coord2pos(x,y,z));
}
template<typename D>
bool StructureCubique<D>::without(int pos)  const {
	return (pos < 0 || pos >= nbrCubes());
}
//************************************
//****Fonctions d'initialisation******
//************************************

template<typename D>
void StructureCubique<D>::initVal(D val){
	for (int pi = 0; pi < getSize(Axe::z); pi++) {//pour chaque cube du pgm
		for (int pj = 0; pj < getSize(Axe::y); pj++) {
			for (int pk = 0; pk < getSize(Axe::x); pk++) {
				data[pi][pj][pk] = val;	//assigner la même valeur
			}
		}
	}
}

template<typename D>
void StructureCubique<D>::setDataSize(int p_size[3]) {
	//initialiser les valeurs simples : vecteur de taille
	for (int i = 0; i < 3; i++) {
		size[i] = p_size[i];
	}
	squareSize = size[0] * size[1];	//taille d'un carré de même z
	//allocation des espaces pour le tableau de données
	data = new char**[p_size[Axe::z]];	//tableau de z vers x. lorsque boucle, favoriser les x mouvants
	for (int i = 0; i < p_size[Axe::z]; i++) {
		data[i] = new D*[p_size[Axe::y]];
		for (int j = 0; j < p_size[Axe::y]; j++) {
			data[i][j] = new D[p_size[Axe::x]];
		}
	}
}
template<typename D>
void StructureCubique<D>::copy(StructureCubique &cp) {
	int ps[3];
	for (int i = 0; i < 3; i++) {
		ps[i] = std::min(cp.size[i], size[i]);
	}
	for (int pi = 0; pi < ps[Axe::z]; pi++) {//pour chaque cube du pgm
		for (int pj = 0; pj < ps[Axe::y]; pj++) {
			for (int pk = 0; pk < ps[Axe::x]; pk++) {
				data[pi][pj][pk] = cp.data[pi][pj][pk];	//assigner la même valeur
			}
		}
	}
}

//conversion de position vers coordo et réciproque
template<typename D>
int StructureCubique<D>::coord2pos(int x, int y, int z)const {
	return (x + size[Axe::y] * y + squareSize*z) ;
}
template<typename D>
coord StructureCubique<D>::pos2coord(int pos)const {
	coord c;
	c.x =  pos					% size[Axe::x];
	c.y = (pos / size[Axe::x])	% size[Axe::y];
	c.z = (pos / squareSize)	% size[Axe::z];
	return c;
}

#endif

#ifndef STRUCTURE_CUBIQUE_TPP
#define STRUCTURE_CUBIQUE_TPP

#include <Data/StructureCubique.hpp>
#include <algorithm>

//************************************
//*****Constructeur & Destructeur*****
//************************************
template<typename D>
StructureCubique<D>::StructureCubique() : StructureCubique(0, 0, 0) {}
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
	//delete* data;
}
//************************************
//*********Fonctions d'accès**********
//************************************
template<typename D>
bool StructureCubique<D>::isEmpty(int x, int y, int z) const {
	if (without(x, y, z)) return true;
	return (data[coord2pos(x, y, z)]) ? false : true;
}
template<typename D>
bool StructureCubique<D>::isOccupied(int x, int y, int z) const {
	if (without(x, y, z))return false;
	return (data[coord2pos(x, y, z)]) ? true : false;
}
template<typename D>
void StructureCubique<D>::set(int x, int y, int z, D val) {
	if (without(x, y, z)) throw DataError("impossible d'assigner une valeur en dehors des limites : " + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z)+"\n");
	data[coord2pos(x, y, z)] = val;
}
template<typename D>
D StructureCubique<D>::get(int x, int y, int z) const {
	if (without(x, y, z)) throw DataError("impossible de récupérer une valeur en dehors des limites : " + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "\n");
	return data[coord2pos(x,y,z)];
}
//avec pos
template<typename D>
bool StructureCubique<D>::isEmpty(int pos) const {
	if (without(pos)) return true;
	return (data[pos]) ? false : true;
}
template<typename D>
bool StructureCubique<D>::isOccupied(int pos) const {
	if (without(pos))return false;
	return (data[pos]) ? true : false;
}
template<typename D>
void StructureCubique<D>::set(int pos, D val) {
	if (without(pos)) throw DataError("impossible d'assigner une valeur en dehors des limites [0;"+ std::to_string(nbrCubes())+"[ : "+ std::to_string(pos) + "\n");
	data[pos]=val;
}
template<typename D>
D StructureCubique<D>::get(int pos) const {
	if (without(pos)) throw DataError("impossible de réupérer une valeur en dehors des limites [0;" + std::to_string(nbrCubes()) + "[ : " + std::to_string(pos) + "\n");
	return data[pos];
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
bool StructureCubique<D>::without(int x, int y, int z)  const {
	if (x<0 || x>=size[Axe::x]) return true;
	if (y<0 || y>=size[Axe::y]) return true;
	if (z<0 || z>=size[Axe::z]) return true;
	return false;
}
template<typename D>
bool StructureCubique<D>::without(int pos)  const {
	return (pos < 0 || pos >= nbrCubes());
}
//************************************
//****Fonctions d'initialisation******
//************************************

template<typename D>
void StructureCubique<D>::initVal(D val) {
	for (int pi = 0; pi < nbrCubes(); pi++) {//pour chaque cube du pgm
		data[pi] = val;
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
	//data = new D[nbrCubes()];
	data.resize(nbrCubes());

}
template<typename D>
void StructureCubique<D>::copy(StructureCubique &cp) {
	int ps[3];//dimensions de travail
	for (int i = 0; i < 3; i++) {//conserver les plus petites dimensions
		ps[i] = std::min(cp.size[i], size[i]);
	}
	for (int pi = 0; pi < ps[Axe::z]; pi++) {//pour chaque cube du pgm
		for (int pj = 0; pj < ps[Axe::y]; pj++) {
			for (int pk = 0; pk < ps[Axe::x]; pk++) {
				data[coord2pos(pk,pj,pi)]= cp.get( pk, pj, pi );	//assigner la même valeur
			}
		}
	}
}

//conversion de position vers coordo et réciproque
template<typename D>
inline int StructureCubique<D>::coord2pos(int x, int y, int z)const {
	return (x + size[Axe::y] * y + squareSize*z);
}
template<typename D>
inline obj::coord StructureCubique<D>::pos2coord(int pos)const {
	obj::coord c;
	c.x = pos					% size[Axe::x];
	c.y = (pos / size[Axe::x]) % size[Axe::y];
	c.z = (pos / squareSize) % size[Axe::z];
	return c;
}

#endif

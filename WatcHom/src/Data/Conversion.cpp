#include "Data\Conversion.hpp"
#include <iostream>

using namespace std;
shared_ptr<ComplexeCubique> Conversion::PGM3D2ComplexeCubique(PGM3D &pgm, bool primal) {
	shared_ptr<ComplexeCubique> ret = shared_ptr<ComplexeCubique>(new ComplexeCubique(
		2 * pgm.getSize(PGM3D::Axe::x) + 1,
		2 * pgm.getSize(PGM3D::Axe::y) + 1,
		2 * pgm.getSize(PGM3D::Axe::z) + 1));
	//gestion primalité : conversion en char et initialisation
	ret->setPrimal(primal);
	char valPrimale = 0;
	if (primal) {
		valPrimale = 1;
	}else {
		ret->initVal( 1 );
	}
	//parcours d'initialisation :
	coord w;
	for (int pi = 0; pi < pgm.getSize(PGM3D::Axe::z); pi++) {//pour chaque cube du pgm
		for (int pj = 0; pj < pgm.getSize(PGM3D::Axe::y); pj++) {
			for (int pk = 0; pk < pgm.getSize(PGM3D::Axe::x); pk++) {
				if (pgm.isOccupied(pk, pj, pi)) {//si il est plein (chaque cube effectif)
					for (int i = -1; i <= 1; i++) {//alors pour tous les espaces adjacents
						for (int j = -1; j <= 1; j++) {
							for (int k = -1; k <= 1; k++) {
								w = coordPGM2ComplexeCub(pk, pj, pi);
								ret->set(w.x + k, w.y + j, w.z + i, valPrimale);//initialisation selon primalité
							}
						}
					}
				}// fin if
			}
		}
	}
	return ret;
}
shared_ptr<obj::Obj> Conversion::PGM3D2Obj(PGM3D &pgm) {
	shared_ptr<obj::Obj> ret = shared_ptr<obj::Obj>(new obj::Obj);
	return ret;
}
shared_ptr<obj::Obj> Conversion::ComplexeCubique2Obj(ComplexeCubique &pgm) {
	shared_ptr<obj::Obj> ret = shared_ptr<obj::Obj>(new obj::Obj);
	return ret;
}

inline coord Conversion::coordPGM2ComplexeCub(int px, int py, int pz) {
	coord ret;
	ret.x = 2 * px + 1;
	ret.y = 2 * py + 1;
	ret.z = 2 * pz + 1;
	return ret;
}
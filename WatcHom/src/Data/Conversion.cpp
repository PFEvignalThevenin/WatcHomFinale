#include "Data\Conversion.hpp"
#include <iostream>

using namespace std;
shared_ptr<ComplexeCubique> Conversion::PGM3D2ComplexeCubique(PGM3D &pgm, bool primal) {
	shared_ptr<ComplexeCubique> ret = shared_ptr<ComplexeCubique>(new ComplexeCubique(
		2 * pgm.getSize(Axe::x) + 1,
		2 * pgm.getSize(Axe::y) + 1,
		2 * pgm.getSize(Axe::z) + 1));
	//gestion primalité : conversion en char et initialisation
	ret->initVal(!primal);
	ret->setPrimal(primal);
	char valPrimale = 0;
	if (primal) {
		valPrimale = 1;
	}else {
		ret->initVal( primal );
	}
	//parcours d'initialisation :
	obj::coord w;
	for (int pi = 0; pi < pgm.getSize(Axe::z); pi++) {//pour chaque cube du pgm
		for (int pj = 0; pj < pgm.getSize(Axe::y); pj++) {
			for (int pk = 0; pk < pgm.getSize(Axe::x); pk++) {
				if (pgm.isOccupied(pk, pj, pi)) {//si il est plein (chaque cube effectif)
					w = coordPGM2ComplexeCub(pk, pj, pi);
					for (int i = -1; i <= 1; i++) {//alors pour tous les espaces adjacents
						for (int j = -1; j <= 1; j++) {
							for (int k = -1; k <= 1; k++) {
								ret->set(w.x + k, w.y + j, w.z + i, primal);//initialisation selon primalité
							}
						}
					}
				}// fin if
			}
		}
	}
	return ret;
}
//potentiellement pas a faire
shared_ptr<obj::Obj> Conversion::PGM3D2Obj(PGM3D &pgm) {
	shared_ptr<obj::Obj> ret = shared_ptr<obj::Obj>(new obj::Obj);
	return ret;
}
shared_ptr<obj::Obj> Conversion::ComplexeCubique2Obj(ComplexeCubique &pgm) {
	shared_ptr<obj::Obj> ret = shared_ptr<obj::Obj>(new obj::Obj);
	return ret;
}

inline obj::coord Conversion::coordPGM2ComplexeCub(int px, int py, int pz) {
	obj::coord ret;
	ret.x = 2 * px + 1;
	ret.y = 2 * py + 1;
	ret.z = 2 * pz + 1;
	return ret;
}
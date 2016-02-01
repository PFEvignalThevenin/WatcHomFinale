#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include "Data\ComplexeCubique.hpp"
#include "Data\PGM3D.hpp"
#include "Data\Obj.hpp"
#include <memory>
/* Méthodes statiques de conversion d'un type de donnée vers une autre
 *
 */
class Conversion
{
public:
	//Interpretation d'un PGM3D vers un Complexe Cubique intègre. taille 2x+1 supérieure.
	static std::shared_ptr<ComplexeCubique> PGM3D2ComplexeCubique(PGM3D &pgm, bool primal=true);//warning : ne fait pas des primal=false
	//Représentation maillée d'un PGM3D
	static std::shared_ptr<obj::Obj> PGM3D2Obj(PGM3D &pgm);
	//Représentation maillée d'un ComplexeCubique
	static std::shared_ptr<obj::Obj> ComplexeCubique2Obj(ComplexeCubique &pgm);
private:
	//calcul permettant de faire la correspondance pour l'interprétation de PGM3D2CC
	//En gros, multiplier par 2, +1
	inline static obj::coord coordPGM2ComplexeCub(int x, int y, int z);

};

#endif
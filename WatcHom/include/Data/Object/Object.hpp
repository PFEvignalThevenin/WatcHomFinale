#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../MiscData.hpp"

/*Classe de gestion des objets de maillage.
 *-Avec un nom
 *-d�finit par un ensemble de faces, cad des listes contenant la r�f�rence vers les vertex qui les composent
	dans la liste de la classe Obj2
 *-Contient en plus la dimension du cluster de cube qu'il repr�sente
 */
namespace obj{
class Object
{
public:
	Object() = default;
	Object(std::string name, int dim=0);
	~Object() = default;

	/*acc�s aux faces*/
	int addFace(face f);	//retourne le num�ro de la face
	face& getFace(int n);
	std::vector<face>& getFaces();
	int nbrFaces();
	/*acc�s au nom*/
	void setName(std::string name);
	std::string getName();
	/*acc�s � la dimension*/
	void setDimension(int dim);
	int getDimension();
private:
	int dimension;
	std::string nom;
	std::vector<face> faces;
};
}

#endif
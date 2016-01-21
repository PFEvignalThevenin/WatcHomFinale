#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../MiscData.hpp"

/*Classe de gestion des objets de maillage.
 *-Avec un nom
 *-définit par un ensemble de faces, cad des listes contenant la référence vers les vertex qui les composent
	dans la liste de la classe Obj2
 *-Contient en plus la dimension du cluster de cube qu'il représente
 */
namespace obj{
class Object
{
public:
	Object() = default;
	Object(std::string name, int dim=0);
	~Object() = default;

	/*accès aux faces*/
	int addFace(face f);	//retourne le numéro de la face
	face& getFace(int n);
	std::vector<face>& getFaces();
	int nbrFaces();
	/*accès au nom*/
	void setName(std::string name);
	std::string getName();
	/*accès à la dimension*/
	void setDimension(int dim);
	int getDimension();
private:
	int dimension;
	std::string nom;
	std::vector<face> faces;
};
}

#endif
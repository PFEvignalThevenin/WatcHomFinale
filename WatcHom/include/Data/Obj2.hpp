#ifndef Obj2_HPP
#define Obj2_HPP

#include "Data\FileSavable.hpp"
#include "Object\Object.hpp"
#include "Object\Vertex.hpp"
#include <vector>
#include <memory>

namespace obj{
/* Classe de correspondance avec un fichier .obj
 * Contient :
 * - une liste de tous les vertices du fichier
 * - Une liste d'objets (voir Data/Object/object) 
 *		dont les faces contiennent index vers vertices du tableau
 */
class Obj2 : public FileSavable
{
public:
	typedef std::shared_ptr<Obj2> Ptr;
	Obj2() = default;
	~Obj2() = default;
	virtual void save(std::string const& path) const override;
	virtual void load(std::string const& path) override;

	//accès objets
	int addObject(Object obj);	//renvoie le numéro de l'objet
	Object& getObject(int n);
	int nbrObjects();
	//accès vertices
	int addVertex(Vertex vert);	//renvoie le numéro du sommet
	Vertex& getVertex(int n);
	int nbrVertices();
private:
	std::vector<Vertex> vertices;
	std::vector<Object> objects;
};
}
#endif
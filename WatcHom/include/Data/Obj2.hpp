#ifndef Obj2_HPP
#define Obj2_HPP

#include "Data\FileSavable.hpp"
#include "Object\Object.hpp"
#include "Object\Vertex.hpp"
#include <vector>
#include <memory>

namespace obj{

class Obj2 : public FileSavable
{
public:
	typedef std::shared_ptr<Obj2> Ptr;
	Obj2() = default;
	~Obj2() = default;
	virtual void save(std::string const& path) const override;
	virtual void load(std::string const& path) override;

	int addObject(Object obj);	//renvoie le numéro de l'objet
	Object& getObject(int n);
	int addVertex(Vertex vert);	//renvoie le numéro du sommet
	Vertex& getVertex(int n);
	int nbrVertices();
	int nbrObjects();
private:
	std::vector<Vertex> vertices;
	std::vector<Object> objects;
};
}
#endif
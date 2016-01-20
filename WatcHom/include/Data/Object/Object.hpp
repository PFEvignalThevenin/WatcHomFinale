#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
namespace obj{
class Object
{
public:
	typedef std::vector<int> face;
	Object() = default;
	Object(std::string name, int dim=0);
	~Object() = default;

	int addFace(face f);	//retourne le numéro de la face
	face& getFace(int n);
	int nbrFaces();
	void setName(std::string name);
	std::string getName();
	std::vector<face>& getFaces();
	void setDimension(int dim);
	int getDimension();
private:
	int dimension;
	std::string nom;
	std::vector<face> faces;
};
}

#endif
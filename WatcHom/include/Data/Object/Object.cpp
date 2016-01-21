#include "Object.hpp"

namespace obj {

	Object::Object(std::string name, int dim) {
		nom = name;
		dimension = dim;
	}
/***********************  Accès FACE  ***********************************/
	int Object::addFace(obj::face f) {
		faces.push_back(f);
		return faces.size() - 1;//retourne l'index de la face ajoutée
	}
	obj::face& Object::getFace(int n) {
		return faces[n];
	}
	std::vector<obj::face>& Object::getFaces() {
		return faces;
	}
	int Object::nbrFaces() {
		return faces.size();
	}
/***********************  Accès NOM  ************************************/
	void Object::setName(std::string name) {
		nom = name;
	}
	std::string Object::getName() {
		return nom;
	}
/***********************  Accès Dimension  *******************************/
	void Object::setDimension(int dim) {
		dimension = dim;
	}
	int Object::getDimension() {
		return dimension;
	}
}
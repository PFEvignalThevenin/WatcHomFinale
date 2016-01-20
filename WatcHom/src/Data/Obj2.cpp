
#include "Data\Obj2.hpp"
#include <fstream>
#include <sstream> 
#include <iostream> 

namespace obj{
using namespace std;

Vertex parseVertex(string line) {
	Vertex v;
	istringstream iss(line);
	string mot;
	iss >> mot;
	v.x = stof(mot);
	iss >> mot;
	v.y = stof(mot);
	iss >> mot;
	v.z = stof(mot);
	//non lecture d'un éventuel dernier paramètre
	return v;
}
Object::face parseFace(string line) {
	Object::face f;
	istringstream iss(line);
	string mot;
	int i = 0;
	while (!iss.eof())
	{
		iss >> mot;
		f.push_back(stoi(mot));//assume there isn't '/' -> texture, material,...
	}
	return f;
}

void Obj2::save(std::string const& path) const {
	ofstream file;
	file.open(path);

	//check ouverture
	if (!file.good()) {
		throw FileError(0,"Error in OBJ save : \n\timpossible to create " + path + ".");
	}
	file << "# OBJ File" << endl;
	file << "# " << vertices.size() << " vertices" << endl;
	file << "# " << objects.size() << " objects" << endl;
	//écrire les sommets
	Vertex v;
	for (unsigned int i = 0; i < vertices.size(); i++) {//écrire tous les vertexes
		v = vertices[i];
		file << "v " << v.x << " " << v.y << " " << v.z << endl;
	}
	//écrire les objets
	Object o;
	vector<Object::face> faces;
	Object::face f;
	for (unsigned int i = 0; i < objects.size(); i++) {//pour chaque objet
		o = objects[i];
		file << "o " << o.getName() << endl;//écrire le nom
		faces = o.getFaces();
		for (unsigned int j = 0; j < faces.size(); j++) {//puis pour chacune des faces
			f = faces[j];
			file << "f ";
			for (unsigned int k = 0; k < f.size(); k++) {//écrire les coordonnées
				file << f[k] << " ";
			}
			file << endl;
		}
	}
}

void Obj2::load(std::string const& path) {
	ifstream file;
	string work;
	file.open(path.c_str(), ios::in);
	//check ouverture
	if (!file.good()) {
		throw FileError(0,"Error in OBJ load : \n\timpossible to open " + path + ".");
	}
	objects.clear();
	vertices.clear();
	//boucle de parcours pour tout le fichier
	int cptr = 0;
	Object *currentObject = nullptr;
	while (!file.eof()) {
		cptr++;
		file >> work;//premier mot de la ligne
		//cas face
		if (work == "f") {//on ne désespère pas. un jour on pourra switcher sur des strings. ptt.
			if (currentObject == nullptr) throw FileError(1,"Error in OBJ load : \n\tface before first object at line " + to_string(cptr));
			getline(file, work); 
			currentObject->addFace(parseFace(work));
		}//cas vertice
		else if (work == "v") {
			getline(file, work);
			addVertex(parseVertex(work));
		}//cas objet
		else if (work == "o") {
			file >> work;
			if (currentObject != nullptr) {
				currentObject->setDimension((currentObject->nbrFaces() == 6) ? 0 : 1);
				addObject(*currentObject);
			}currentObject = new Object(work, 1);
		}//cas commentaire
		else if (work == "#") {
			std::getline(file, work);//sauter la ligne
		}
		else {
			if(!file.eof())
				throw FileError(2,"Error in OBJ load : \n\tcant't read the caracter " + work + " at line " + to_string(cptr));
		}
	}
	currentObject->setDimension((currentObject->nbrFaces() == 6) ? 0 : 1);
	if (currentObject != nullptr)//add the last object
		addObject(*currentObject);
}

int Obj2::addObject(Object obj) {
	objects.push_back(obj);
	return objects.size() -1 ;//index
}

Object& Obj2::getObject(int n) {
	return objects[n];
}

int Obj2::addVertex(Vertex vert) {
	vertices.push_back(vert);
	return vertices.size();//ne pas '-1' car le premier vertex d'un obj est numéroté 1
}

Vertex& Obj2::getVertex(int n) {
	return vertices[n-1];//comme on numérote de 1 à size, il faut soustraire 1
}
int Obj2::nbrVertices() {
	return vertices.size();
}
int Obj2::nbrObjects() {
	return objects.size();
}
}
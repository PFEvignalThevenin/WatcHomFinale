#include "Engine\Modeleur.hpp"
#include <algorithm>
using namespace std;
#include <iostream>
using namespace obj;
Modeleur::Modeleur(std::shared_ptr<Controlleur2> ctrl)
{
	this->ctrl = ctrl;
}
void Modeleur::drawCube0(obj::Vertex center) {
	glBegin(GL_QUAD_STRIP);
	glVertex3f(center.x + rayon, center.y - rayon, center.z + rayon);//les côtés
	glVertex3f(center.x + rayon, center.y - rayon, center.z - rayon);
	glVertex3f(center.x + rayon, center.y + rayon, center.z + rayon);
	glVertex3f(center.x + rayon, center.y + rayon, center.z - rayon);
	glVertex3f(center.x - rayon, center.y + rayon, center.z + rayon);
	glVertex3f(center.x - rayon, center.y + rayon, center.z - rayon);
	glVertex3f(center.x - rayon, center.y - rayon, center.z + rayon);
	glVertex3f(center.x - rayon, center.y - rayon, center.z - rayon);
	glVertex3f(center.x + rayon, center.y - rayon, center.z + rayon);
	glVertex3f(center.x + rayon, center.y - rayon, center.z - rayon);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(center.x + rayon, center.y - rayon, center.z + rayon);//le dessus
	glVertex3f(center.x + rayon, center.y + rayon, center.z + rayon);
	glVertex3f(center.x - rayon, center.y + rayon, center.z + rayon);
	glVertex3f(center.x - rayon, center.y - rayon, center.z + rayon);
	glVertex3f(center.x + rayon, center.y - rayon, center.z - rayon);//le dessous
	glVertex3f(center.x + rayon, center.y + rayon, center.z - rayon);
	glVertex3f(center.x - rayon, center.y + rayon, center.z - rayon);
	glVertex3f(center.x - rayon, center.y - rayon, center.z - rayon);
	glEnd();
}
void Modeleur::drawCube1(std::vector<obj::Vertex> line) {
	//enlever les points si alignements (et les doublons)

	//si reste 1 point : cube

	//si reste 2 points : rectangle

	//sinon : algo
}
void Modeleur::drawFace(const Object::face &fa) {
	Vertex v;
	glBegin(GL_POLYGON);
	for (int i : fa) {
		v = objAffiche->getVertex(i);
		glVertex3f(v.x, v.y, v.z);
	}
	glEnd();
}
void Modeleur::computeCenter() {
	Vertex v = objAffiche->getVertex(1);;
	float xmin = v.x, xmax = v.x;
	float ymin = v.y, ymax = v.y;
	float zmin = v.z, zmax = v.z;
	for (int i = 2; i <= objAffiche->nbrVertices(); i++) {
		v = objAffiche->getVertex(i);
		xmin = std::min(xmin, v.x);
		xmax = std::max(xmax, v.x);
		ymin = std::min(ymin, v.y);
		ymax = std::max(ymax, v.y);
		zmin = std::min(zmin, v.z);
		zmax = std::max(zmax, v.z);
	}
	ctrl->computeCenter(xmin, xmax, ymin, ymax, zmin, zmax);
}
void Modeleur::setObj(obj::Obj2::Ptr obj) {
	objAffiche = obj;
}
void Modeleur::initiateObjs() {
	ctrl->resetLists();
	Object obj;
	std::vector<GLuint> *listObj;
	cout << "construction" << endl;
	int cptr = 1;//compte toutes les listes. Il ne doit pas y avoir de doublons
	for (Controlleur2::Dim a : {Controlleur2::Dim::d0, Controlleur2::Dim::d1, Controlleur2::Dim::d2, Controlleur2::Dim::d3}) {
		//pour chaque dimension
		cout << "DIM : " << to_string(a) << endl;
		listObj = ctrl->getFormes(a);
		for (int i = 0; i < objAffiche->nbrObjects(); i++) {//parcourir les objets
			obj = objAffiche->getObject(i);
			if (obj.getDimension() == a) {//si objet de la dimension a
				glNewList(cptr, GL_COMPILE);	//créer nouvelle liste
				listObj->push_back(cptr);		//conserver identificateur
				for (Object::face f : obj.getFaces()) {//tracer l'objet
					drawFace(f);
				}
				cout << to_string(cptr) << endl;
				glEndList();
				cptr++;
			}
		}
	}
	computeCenter();
}
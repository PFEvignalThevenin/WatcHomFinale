#include "Engine\Controlleur2.hpp"
#include <iostream>

using namespace std;
using namespace obj;
//*********************************************Constructeur***************************************************
Controlleur2::Controlleur2() {
	glEnable(GL_DEPTH_TEST);
	glutInitDisplayMode(GLUT_RGBA);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	static const auto pi = 3.1415926535897932384626433832795f;
	static const auto fov = 90.f;
	static const auto near_distance = .1f;
	static const auto far_distance = 100.f;
	static const auto aspect = 800.f / 600.f;
	auto frustum_height = std::tan(fov / 360 * pi) * near_distance;
	auto frustum_width = frustum_height * aspect;
	glFrustum(-frustum_width, frustum_width, -frustum_height, frustum_height, near_distance, far_distance);
	setCouleur(Dim::d0, 255, 0, 0);
	setCouleur(Dim::d1, 0, 255, 0);
	setCouleur(Dim::d2, 0, 0, 255);
	setCouleur(Dim::d3, 100, 0, 100);
}
Controlleur2::Ptr Controlleur2::get() {
	static  Controlleur2::Ptr inst = Controlleur2::Ptr(new Controlleur2);
	inst->modeleur = Modeleur::Ptr(new Modeleur(inst));
	return inst;
}
//************************************************Rafrichissement affichage************************************************
void Controlleur2::drawGL() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	static float angle = 0;
	angle += 2;
	glTranslatef(0, 0, -50.0f);
	glRotatef(angle, 0, 0, 1);
	glRotatef(angle*0.3f, 0, 1, 0);
	glRotatef(angle*1.4f, 1, 0, 0);
	//affichage spécifique
	glPushMatrix();
	glTranslatef(center.x, center.y, center.z);
	if (!afficher) {
		drawDefault();
	}
	else {
		drawObj();
	}
	glPopMatrix();
}
//**************************************************Dessin**********************************************
void Controlleur2::drawDefault() {
	int i = 0;
	glColor4f(couleurs[i].rouge, couleurs[i].vert, couleurs[i].bleu,50);
	modeleur->drawCube0(obj::Vertex());
}

void Controlleur2::drawObj() {
	//affichage par dimension
	for (int i = 0; i < DIM; i++) {//pour chaque dimension
		glColor4f(couleurs[i].rouge, couleurs[i].vert, couleurs[i].bleu, couleurs[i].alpha);//couleur appropriée
		for (int j : listObj[i]) {//afficher chaque objet
			glCallList(j);
		}
	}
}
//****************************************Gestion Obj2 ************************************************
void Controlleur2::initiateObjs() {
	//vider les anciennes listes.
	resetLists();
	//faire reconstruire les listes par le modeleur
	modeleur->initiateObjs();
	afficher = true;
}
//charge un obj et le stocke ds le modelleur pour l'affichage
void Controlleur2::loadObj(std::string path) {
	try {
		Obj2::Ptr objAffiche = Obj2::Ptr(new Obj2());
		objAffiche->load(path);
		modeleur->setObj(objAffiche);
		initiateObjs();
	}
	catch (FileError fe) {
		std::cout << fe.what();
	}
	catch (exception e) {
		std::cout << e.what();
	}
}
//****************************************Gestion listes ************************************************
void Controlleur2::resetLists() {
	for (int i = 0; i < DIM; i++) {//pour chaque dimension
		for (size_t j = 0; j < listObj[i].size(); j++) {
			glDeleteLists(listObj[i].at(j), 1);
		}
		listObj[i].resize(0);
	}
}
vector<GLuint>* Controlleur2::getFormes(Dim dim) {
	return &listObj[dim];
}
void Controlleur2::computeCenter(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) {
	center.x = -(xmin + xmax) / 2;
	center.y = -(ymin + ymax) / 2;
	center.z = -(zmin + zmax) / 2;
}
//****************************************Gestion diverse ************************************************
void Controlleur2::setCouleur(Dim dim, GLfloat rouge, GLfloat vert, GLfloat bleu, GLfloat alpha) {
	couleurs[dim].rouge = rouge;
	couleurs[dim].vert = vert;
	couleurs[dim].bleu = bleu;
	couleurs[dim].alpha = alpha;
}
Dim Controlleur2::int2Dim(int d) {
	switch (d) {
	case 0:
		return Dim::d0;
	case 1:
		return Dim::d1;
	case 2:
		return Dim::d2;
	case 3:
		return Dim::d3;
	default:
		throw CtrlError(to_string(d) + "ne correspond à aucune dimension. Limité entre 0 et 3");
	}
}

#include "Engine\Controlleur2.hpp"
#include "Data\PGM3D.hpp"
#include "Data\Conversion.hpp"
#include <iostream>

using namespace std;
using namespace obj;
//*********************************************Constructeur***************************************************
Controlleur2::Controlleur2() {
	glEnable(GL_DEPTH_TEST);
	//light
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_SPECULAR);
	glutInitDisplayMode(GLUT_RGBA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setViewPort();
	setCouleur(Dim::d0, 255, 0, 0,255);
	setCouleur(Dim::d1, 0, 255, 0, 255);
	setCouleur(Dim::d2, 0, 0, 255, 255);
	setCouleur(Dim::d3, 100, 0, 100, 255);
}
Controlleur2::Ptr Controlleur2::get() {
	static  Controlleur2::Ptr inst = Controlleur2::Ptr(new Controlleur2);
	if (inst->modeleur == nullptr) {
		Modeleur::Ptr mod(new Modeleur(inst));
		inst->modeleur = mod;
	}
	return inst;
}
//************************************************Rafraichissement affichage************************************************
void Controlleur2::drawGL() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setViewPort();
	glClearColor(0.5f, 0.7f, 1.0f, 1.0f);//couleur de fond
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	static int LightPos[4] = { 0,0,1,1 };
	glLightiv(GL_LIGHT0, GL_POSITION, LightPos);
	glLoadIdentity();
	//position du repère de l'objet
	static float angle = 0;
	glTranslatef(translations[0], translations[1], -20.0f + translations[2]);//décalages
	if (autoroll) {//incrément autoroll
		angle += 2;
	}
	glRotatef(angle, 0, 0, 1);//rotation autoroll
	glRotatef(angle*0.3f, 0, 1, 0);
	glRotatef(angle*1.4f, 1, 0, 0);
	//TODO : ici trackball
	//affichage spécifique de 'objet
	glPushMatrix();
	glTranslatef(center.x, center.y, center.z);//centrer
	if (!afficher) {//si aucun objet n'a été chargé
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
	glColor4f(couleurs[i].rouge, couleurs[i].vert, couleurs[i].bleu, couleurs[i].alpha);
	modeleur->drawCube0(obj::Vertex());
}

void Controlleur2::drawObj() {
	//affichage par dimension
	for (int i = 0; i < DIM; i++) {//pour chaque dimension
		if (!affDim[i]) continue;//court-circuiter l'affichage de cette dimension
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
//charge un obj et le stocke ds le modeleur pour l'affichage
bool Controlleur2::loadObj(std::string path) {
	try {
		Obj2::Ptr objAffiche = Obj2::Ptr(new Obj2());
		objAffiche->load(path);
		modeleur->setObj(objAffiche);
		initiateObjs();
		return true;
	}
	catch (FileError fe) {
		std::cout << fe.what();
	}
	catch (exception e) {
		std::cout << e.what();
	}
	return false;
}
//**********************************************Chargement PGM************************************************
bool Controlleur2::loadPgm(std::string path) {
	try {
		PGM3D::Ptr pgm = PGM3D::Ptr(new PGM3D());
		pgm->load(path);
		ComplexeCubique::Ptr cc = Conversion::PGM3D2ComplexeCubique(*pgm);
		dgvf = DGVF::Ptr(new DGVF(cc));
		modeleur->setPgm(pgm);
		modeleur->initiatePgm();
		modeleur->setComplexeCubique(cc);
		afficher = true;
		return true;
	}
	catch (FileError fe) {
		std::cout << fe.what();
	}
	catch (exception e) {
		std::cout << e.what();
	}
	return false;
}
//***********************************************fonctions DGVF***********************************************
void Controlleur2::cellClustering() {
	dgvf->CellClustering();
	modeleur->initiateComplexeCubique(dgvf->getGinv());
	if (saveObj) {
		cout << "TODO : saveOBJ after cell clustering" << endl;
	}if (saveMorse) {
		cout << "TODO : saveMorse after cell clustering" << endl;
	}
}
bool Controlleur2::isPerfect() {
	return dgvf->perfect();
}
bool Controlleur2::isClusterisable() {
	return dgvf->clusterisable();
}
std::shared_ptr<std::vector<DGVF::cellBound>> Controlleur2::getCollapses() {
	return dgvf->computeCollapses();
}
void Controlleur2::collapse(int c1, int c2) {
	cout << "cell collapse" << c1<<" "<<c2<<endl;
	dgvf->add2V(c1, c2);
	modeleur->initiateComplexeCubique(dgvf->getGinv());
	if (saveObj) {
		cout << "TODO : saveOBJ after collapse" << endl;
	}if (saveMorse) {
		cout << "TODO : saveMorse after collapse" << endl;
	}
}
int Controlleur2::getDim(int pos) {
	return dgvf->getDim(pos);
}

//**********************************************Gestion listes ************************************************
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
	couleurs[dim].rouge = rouge/255;
	couleurs[dim].vert = vert / 255;
	couleurs[dim].bleu = bleu / 255;
	couleurs[dim].alpha = alpha / 255;
}
void Controlleur2::setDistances(float rayon, float longueur, float separation) {
	modeleur->setDistances(rayon, longueur, separation);
}
void Controlleur2::setDimFenetre(double width, double height) {
	viewX = width;
	viewH = height;
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
void Controlleur2::zoom(GLfloat mod) {
	translations[2] += mod;
}
void Controlleur2::translation(GLfloat axeX, GLfloat axeY) {
	translations[0] += axeX;
	translations[1] += axeY;
}
void Controlleur2::recentrer() {
	for (int i = 0; i < 3; i++) {
		translations[i] = 0;
	}
}
void Controlleur2::rotation(GLfloat dx, GLfloat dy) {
	cout << "rotation dx: " << dx << " et dy: " << dy << endl;
}
void Controlleur2::setAutoroll(bool set) {
	autoroll = set;
}
void Controlleur2::setAffichageDim(Dim d, bool set ) {
	affDim[d] = set;
}
void Controlleur2::setSave(bool obj , bool morse ) {
	saveObj = obj;
	saveMorse = morse;
}
//***************************fonctions privées diverses***************************
void Controlleur2::setViewPort() {
	static const auto pi = 3.1415926535897932384626433832795f;
	static const auto fov = 90.f;
	static const auto near_distance = .1f;
	static const auto far_distance = 100.f;
	auto aspect = viewX / viewH;
	auto frustum_height = std::tan(fov / 360 * pi) * near_distance;
	auto frustum_width = frustum_height * aspect;
	glFrustum(-frustum_width, frustum_width, -frustum_height, frustum_height, near_distance, far_distance);
}
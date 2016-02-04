#include "Engine\Controlleur2.hpp"
#include "Data\PGM3D.hpp"
#include "Data\Conversion.hpp"
#include <iostream>
#include <ctime>

using namespace std;
using namespace obj;
//*********************************************Constructeur***************************************************
Controlleur2::Controlleur2() {
	//initiateOpenGL();
	setCouleur(Dim::d0, 255, 0, 0,255);
	setCouleur(Dim::d1, 0, 255, 0, 255);
	setCouleur(Dim::d2, 0, 0, 255, 255);
	setCouleur(Dim::d3, 100, 0, 100, 255);
	tb.tbReshape(800, 600);
}
void Controlleur2::initiateOpenGL() {
	glutInitDisplayMode(GLUT_RGBA);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	GLfloat light_ambient[] = { 0.5f,0.5f,0.5f, 1.0f };
	GLfloat light_diffuse[] = {0.9f,0.9f,0.9f, 1.0f };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

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
	static bool initiate = true;
	if (initiate) {
		initiate = false;
		initiateOpenGL();
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setViewPort();
	//couleur de fond
	glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//début tracé
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//position du repère de l'objet
	static float angle = 0;
	glTranslatef(translations[0], translations[1], -10.0f + translations[2]);//décalages
	glPushMatrix();//rotations
	tb.tbMatrix();//utilisateur
	if (autoroll) {// autoroll : incrément
		angle += 2;
	}
	glRotatef(angle, 0, 0, 1);	//autoroll : rotation
	glRotatef(angle*0.3f, 0, 1, 0);
	glRotatef(angle*1.4f, 1, 0, 0);
	//affichage spécifique de 'objet
	glScalef(facteurZoom, facteurZoom, facteurZoom);
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
			if(infoListObj[j].second)//si candidat à l'affichage
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
		etapesSave.clear();
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
	clock_t begin = clock();
	dgvf->CellClustering();
	std::cout << "Cell Clustering in " << double(clock() - begin) / CLOCKS_PER_SEC << " secs." << endl;
	etapesSave.push_back(dgvf->getG());
	modeleur->initiateComplexeCubique(etapesSave.back());
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
	clock_t begin = clock();
	dgvf->add2V(c1, c2);
	std::cout << "Collapse in " << double(clock() - begin) / CLOCKS_PER_SEC << " secs." << endl;
	etapesSave.push_back(dgvf->getG());
	modeleur->initiateComplexeCubique(etapesSave.back());
}
int Controlleur2::getDim(int pos) {
	return dgvf->getDim(pos);
}
int Controlleur2::getNbrIterations() {
	return etapesSave.size();
}
void Controlleur2::retourIterPrecedente() {
	cout << "Retour itération précedente, ok pour affichage, mais pas pour l'algorithme, donc en suspend" << endl;
	bool DGVFPeuxRetourArrière = false;
	if (DGVFPeuxRetourArrière) {
		if (etapesSave.size() < 2) return;
		etapesSave.pop_back();
		modeleur->initiateComplexeCubique(etapesSave.back());
	}
}

//**********************************************Gestion listes ************************************************
void Controlleur2::resetLists() {
	for (int i = 0; i < DIM; i++) {//pour chaque dimension
		for (size_t j = 0; j < listObj[i].size(); j++) {
			glDeleteLists(listObj[i].at(j), 1);
		}
		listObj[i].resize(0);
	}
	infoListObj.clear();
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
	tb.tbReshape((int)width,(int) height);
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
void Controlleur2::travelling(GLfloat mod) {
	translations[2] += mod;
}
void Controlleur2::setZoom(GLfloat z) {
	facteurZoom = (z < 0.01) ? 0.01f : z;
}
GLfloat Controlleur2::getZoom() {
	return facteurZoom;
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
void Controlleur2::rotation(int x, int y) {
	tb.tbMotion(x, y);
}
void Controlleur2::startRotation(int x, int y) {
	tb.tbStart(x, y);
}
void Controlleur2::stopRotation() {
	tb.tbStop();
}
void Controlleur2::setAutoroll(bool set) {
	autoroll = set;
}
void Controlleur2::setAffichageDim(Dim d, bool set ) {
	affDim[d] = set;
}
//***************************fonctions privées diverses***************************
void Controlleur2::setViewPort() {
	static const auto pi = 3.1415926535897932384626433832795f;
	static const auto fov = 90.f;
	static const auto near_distance = .1f;
	static const auto far_distance = 1000.f;
	auto aspect = viewX / viewH;
	auto frustum_height = std::tan(fov / 360 * pi) * near_distance;
	auto frustum_width = frustum_height * aspect;
	glFrustum(-frustum_width, frustum_width, -frustum_height, frustum_height, near_distance, far_distance);
}

//***************************Fontions de liste***************************

void Controlleur2::setNameList(GLuint liste, string nom) {
	//si il n'existe pas d'entrée, la créer. Sinon faire modif
	if (infoListObj.find(liste) != infoListObj.end()) {//il existe une entrée
		infoListObj[liste].first = nom;
	}
	else {
		infoListObj.insert(pair<GLuint,pair<string,bool>>(liste,  pair<string, bool>(nom, true)));
	}
}
string Controlleur2::getNameList(int liste) {
	//si il n'existe pas d'entrée, la créer. Sinon faire modif
	if (infoListObj.find(liste) != infoListObj.end()) {//il existe une entrée
		return infoListObj[liste].first;
	}
	else {
		infoListObj.insert(pair<GLuint, pair<string, bool>>(liste, pair<string, bool>(to_string(liste), true)));
		return to_string(liste);
	}
}
void Controlleur2::setAfficherList(GLuint liste, bool aff) {
	//si il n'existe pas d'entrée, la créer. Sinon faire modif
	if (infoListObj.find(liste) != infoListObj.end()) {//il existe une entrée
		infoListObj[liste].second = aff;
	}
	else {
		infoListObj.insert(pair<GLuint, pair<string, bool>>(liste, pair<string, bool>(to_string(liste), aff)));
	}
}
bool Controlleur2::getAfficherListe(GLuint liste) {
	//si il n'existe pas d'entrée, la créer. Sinon faire modif
	if (infoListObj.find(liste) != infoListObj.end()) {//il existe une entrée
		return infoListObj[liste].second;
	}
	else {
		infoListObj.insert(pair<GLuint, pair<string, bool>>(liste, pair<string, bool>(to_string(liste), true)));
		return true;
	}
}
//***************************fonctions de sauvegarde***************************
bool Controlleur2::saveObj(std::string path) {
	cout << "save OBJ at : \n\t" << path << endl;
	cout << "\tTODO : utiliser une autre fonction, qui soit mieux construite et dans le modeleur, ou équivalent"<< endl;
	try {
		clock_t begin = clock();
		bool ret = dgvf->saveObj(path);
		std::cout << path << " written in " << double(clock() - begin) / CLOCKS_PER_SEC << " secs." << endl;
		return ret;
	}
	catch (FileError fe) {
		std::cout << fe.what();
	}
	catch (exception fe) {
		std::cout << fe.what();
	}
	return false;
}
bool Controlleur2::saveMorse(std::string path) {
	cout << "save Morse at :\n\t" << path << endl;
	try {
		return 	dgvf->saveMorse(path);
	}
	catch (FileError fe) {
		std::cout << fe.what();
	}catch (exception fe) {
		std::cout << fe.what();
	}
	return false;
}
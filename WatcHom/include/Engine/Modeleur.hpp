#ifndef MODELEUR_HPP
#define MODELEUR_HPP

#include "Engine\Controlleur2.hpp"
#include "Engine\DGVF.hpp"
#include "Data\Obj2.hpp"
#include "Data\PGM3D.hpp"
#include "Data\ComplexeCubique.hpp"
#include "GL\freeglut.h"
#include <map>

class Controlleur2;//inclusion croisée
/*Classe de dessin openGL
 *contient un Obj sur lequel il se base pour dessiner.
 *sert notemment pour l'initialisation des listes de controleur2
 */
class Modeleur
{
public:
	typedef std::shared_ptr<Modeleur> Ptr;
	Modeleur( std::shared_ptr<Controlleur2> &ctrl );
	~Modeleur() = default;
	void setDistances(float rayon, float longueur, float separation);
	//****************************Fonctions de Dessin par type********************************
	//initialise les listes de Controlleur2 selon l'objet à afficher
	void initiateObjs();
	void initiatePgm();
	void initiateComplexeCubique(std::shared_ptr<std::vector<std::map<int, std::list<int>>>> &g);
	//***********************Fonctions de définition des objets à afficher********************************
	//set l'objet a afficher, mais ne change pas l'affichage effectif
	void setObj(obj::Obj2::Ptr obj);
	void setPgm(PGM3D::Ptr pgm);
	void setComplexeCubique(ComplexeCubique::Ptr cc);
	//****************************Fonctions de Dessin Complexes********************************
	void drawCube0(obj::Vertex center);
	void drawCube1(int pos, std::vector<dir> Axes);
	void drawCube1(DGVF::cellList cluster);
	void drawCube2(DGVF::cellList cluster);
	void drawCube3(DGVF::cellList cluster);
	//****************************Fonctions de Dessin générales********************************
	void drawPave(obj::Vertex center, GLfloat largeur, GLfloat hauteur, GLfloat profondeur, Axe sensLargeur = Axe::x, Axe sensProfondeur = Axe::z);
private:
	//un pointeur vers son inséparable controleur2
	std::shared_ptr<Controlleur2> ctrl;
	/*les valeurs de parametrage de l'affichage 
	 * dist(voisins dim0) = 2*rayon + 2*separation + longueur = dist
	 * rayon : celui des cubes de dim 0
	 * longuer : celle des cubes de dim 1
	 * separation : espace entre les cubes
	 */
	float rayon, longueur, separation;
	float dist;
	//les objets à afficher
	obj::Obj2::Ptr objAffiche;
	PGM3D::Ptr pgmTraite;
	ComplexeCubique::Ptr ccTraite;
protected:
	//****************************Fonctions de Dessin spécifiques*******************************
	void drawContour(std::vector<obj::Vertex> positions_2, std::vector<obj::Vertex> positions_tmp);
	std::vector <obj::Vertex> Modeleur::computeCarre(obj::Vertex center, dir, float, float);
	std::vector <obj::Vertex> Modeleur::computeRectangle(obj::Vertex center, dir axe, float profondeur, float largeur, float hauteur);
	void Modeleur::drawCarre(std::vector<obj::Vertex> pos);
	void drawCoude(obj::coord center, dir direct1, dir direct2, float lg);
	void drawCoin(obj::coord center, dir direct1, dir direct2, dir direct3);
	void drawFace(const obj::face &fa);
	void DrawRalonge(std::vector<obj::Vertex> positions, dir direct, unsigned int a, unsigned int b);
	void drawRaccord(std::vector<obj::Vertex> positions, dir direct1, dir durect2, unsigned int i);
	void drawPgm(PGM3D &pgm);
	dir Modeleur::computeAxe(int first, int next);
	//****************************Fonctions de Gestion diverse**********************************
	bool InCluster(DGVF::cellList cluster, int pos);
	bool InCluster(DGVF::cellList cluster, int x, int y, int z);
	std::list<int> cluster_neighbors(std::list<int> cluster, int position);
	//return within 'center' the coordinates of the center of the object
	void computeCenter();
	obj::Vertex coord2Vert(obj::coord co);
	obj::Vertex computeNormales(obj::Vertex p1, obj::Vertex p2, obj::Vertex p3);
	obj::Vertex computeNormales(obj::Vertex points[3]);
	
};

#endif

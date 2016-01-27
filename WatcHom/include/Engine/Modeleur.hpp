#ifndef MODELEUR_HPP
#define MODELEUR_HPP

#include "Engine\Controlleur2.hpp"
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
	Modeleur( std::shared_ptr<Controlleur2> ctrl );
	~Modeleur() = default;
	//****************************Fonctions de Dessin********************************
	void drawCube0(obj::Vertex center);
	void drawCube1(std::vector<obj::Vertex> line);
	void drawFace(const obj::face &fa);
	void drawPgm(PGM3D &pgm);
	//return within 'center' the coordinates of the center of the object
	void computeCenter();
	void setDistances(float rayon, float longueur, float separation);
	//set l'objet a afficher, mais ne change pas l'affichage effectif
	void setObj(obj::Obj2::Ptr obj);
	void setPgm(PGM3D::Ptr pgm);
	void setComplexeCubique(ComplexeCubique::Ptr cc);
	//initialise les listes de Controlleur2 selon l'objet à afficher
	void initiateObjs();
	void initiatePgm();
	void initiateComplexeCubique(std::shared_ptr<std::vector<std::map<int, int>>> g_inv);

	obj::Vertex coord2Vert(obj::coord co);
private:
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

	//un pointeur vers son inséparable controleur2
	std::shared_ptr<Controlleur2> ctrl;
	
};


#endif
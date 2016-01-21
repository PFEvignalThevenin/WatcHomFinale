#ifndef MODELEUR_HPP
#define MODELEUR_HPP

#include "Engine\Controlleur2.hpp"
#include "Data\Obj2.hpp"
#include "GL\freeglut.h"

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
	//return within 'center' the coordinates of the center of the object
	void computeCenter();
	//set l'objet a afficher, mais ne change pas l'affichage effectif
	void setObj(obj::Obj2::Ptr obj);
	//initialise les listes de Controlleur2 selon l'objet à afficher
	void initiateObjs();
private:
	//les valeurs de parametrage de l'affichage 
	float rayon = 1, espace, separation;
	//l'objet à afficher
	obj::Obj2::Ptr objAffiche;
	//un pointeur vers son inséparable controleur2
	std::shared_ptr<Controlleur2> ctrl;
	
};


#endif
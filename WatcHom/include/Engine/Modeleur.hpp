#ifndef MODELEUR_HPP
#define MODELEUR_HPP

#include "Engine\Controlleur2.hpp"
#include "Data\Obj2.hpp"
#include "GL\freeglut.h"

class Controlleur2;//inclusion croisée

class Modeleur
{
public:
	typedef std::shared_ptr<Modeleur> Ptr;
	Modeleur( std::shared_ptr<Controlleur2> ctrl );
	~Modeleur() = default;

	void drawCube0(obj::Vertex center);
	void drawCube1(std::vector<obj::Vertex> line);
	void drawFace(const obj::Object::face &fa);
	//return within 'center' the coordinates of the center of the object
	void computeCenter();

	void setObj(obj::Obj2::Ptr obj);
	void initiateObjs();	//initialise les listes de Controlleur
private:
	std::shared_ptr<Controlleur2> ctrl;
	float rayon=1, espace, separation;
	obj::Obj2::Ptr objAffiche;	//l'objet à afficher
};


#endif
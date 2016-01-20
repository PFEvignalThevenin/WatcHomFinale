#ifndef CONTROLLEUR_HPP
#define CONTROLLEUR_HPP

#include <memory>
#include "Data\Obj.hpp"
#include "GL\freeglut.h"

class Controlleur
{
public:
	~Controlleur() = default;
	typedef std::shared_ptr<Controlleur> Ptr;
	//fonction de singleton et d'accès au bidule
	static Ptr get();
	//fonction de rafraichissement utilisée par la fenetre d'affichage
	void drawGL();	//gère les autres fonctions de dessin en fonction du contexte

					//fonctions de chargement
	void loadObj(std::string path);//charge un obj et le sauvegarde ds objAffiche pour l'afficher
private:
	Controlleur() = default;
								//fonctions de dessin
	void drawDefault();
	void drawObj();

	GLuint objAffiche;	//une GL list;
};

#endif
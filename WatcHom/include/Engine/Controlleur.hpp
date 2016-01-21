#ifndef CONTROLLEUR_HPP
#define CONTROLLEUR_HPP

#include <memory>
#include "Data\Obj.hpp"
#include "GL\freeglut.h"
/*Classe de controle du programme. Genre totallement as-been. 
 * Ne comptez pas dessus, je l'enl�ve d�s que possible.
 * utilise la classe Obj, qui est as-been.
 * Voyez plut�t Controlleur2, qui est du grand art.
 */
class Controlleur
{
public:
	~Controlleur() = default;
	typedef std::shared_ptr<Controlleur> Ptr;
	//fonction de singleton et d'acc�s au bidule
	static Ptr get();
	//fonction de rafraichissement utilis�e par la fenetre d'affichage
	void drawGL();	//g�re les autres fonctions de dessin en fonction du contexte

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
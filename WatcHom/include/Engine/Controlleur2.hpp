#ifndef CONTROLLEUR2_HPP
#define CONTROLLEUR2_HPP

#include <memory>
#include "GL\freeglut.h"
#include "Engine\Modeleur.hpp"
#include "Data\Object\Vertex.hpp"
#include <vector>
#define DIM 4

class Modeleur;//inclusion croisée

struct couleur {
	GLfloat rouge, vert, bleu,alpha;
};
class Controlleur2
{
public:
	enum Dim { d0 = 0, d1 = 1, d2 = 2, d3 = 3};
	~Controlleur2() = default;
	typedef std::shared_ptr<Controlleur2> Ptr;
	//fonction de singleton et d'accès au bidule
	static Ptr get();
	//fonction de rafraichissement utilisée par la fenetre d'affichage
	void drawGL();	//gère les autres fonctions de dessin en fonction du contexte

	//setCouleurs f(dimension)
	void setCouleur(Dim dim, GLfloat rouge, GLfloat vert, GLfloat bleu, GLfloat alpha=255);
	//accès aux id des lists de calcul des polygones
	std::vector<GLuint>* getFormes(Dim dim);

	//fonctions de chargement
	void loadObj(std::string path);//charge un obj et le sauvegarde ds objAffiche pour l'afficher
	void Controlleur2::computeCenter(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);	//calcule les valeurs du centre selon la taille de l'objet
	void initiateObjs();	//initialise les listes de Controlleur
	void resetLists();
	static Dim int2Dim(int d);
private:
	Controlleur2();
	std::shared_ptr<Modeleur> modeleur;

	bool afficher = false;
	obj::Vertex center;			//le centre du repère d'affichage. Pas encore utilisé
	std::vector<GLuint> listObj[DIM];	//une list par dimensions
	couleur couleurs[DIM];		//une couleur par dimension

	//fonctions de dessin
	void drawDefault();
	void drawObj();
};
/*
*Classe d'exception pour les pb de controlleur
*
*/
class CtrlError : public std::exception
{
public:
	CtrlError(std::string const& phrase = "Controlleur Error") throw() :m_phrase(phrase) {}

	virtual const char* what() const throw()
	{
		return m_phrase.c_str();
	}

	virtual ~CtrlError() throw()
	{}

private:
	std::string m_phrase;            //Description de l'erreur
};

#endif
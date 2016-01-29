#ifndef CONTROLLEUR2_HPP
#define CONTROLLEUR2_HPP

#include <memory>
#include "GL\freeglut.h"
#include "Engine\Modeleur.hpp"
#include "Engine\DGVF.hpp"
#include "Data\Object\Vertex.hpp"
#include "Data\MiscData.hpp"
#include <iostream>

class Modeleur;//inclusion croisée
/* Classe de contrôle du programme.
 * Gère la logique d'affichage, les comportements généraux, les chargements, tout ça,...
 * Pour les autres délégations, voir avec les autres classes de "Engine/"
 * en particulier : 'Modeleur" pour le tracer openGL
 *
 * les différents objets sont stockés sous forme de listes openGL puis gérés par Controleur.
 * -> il y a une liste par dimension afin de permettre les affichages différents
 */
class Controlleur2
{
public:
	~Controlleur2() = default;
	typedef std::shared_ptr<Controlleur2> Ptr;
	//fonction de singleton et d'accès au bidule
	static Ptr get();
	//fonction de rafraichissement utilisée par la fenêtre d'affichage
	void drawGL();	//gère les autres fonctions de dessin en fonction du contexte

	//***************************Affichage et navigation***************************
	/*setCouleurs f(dimension)
	 *valeurs en entrée entre 0 et 255 svp.
	 */
	void setCouleur(Dim dim, GLfloat rouge, GLfloat vert, GLfloat bleu, GLfloat alpha=255);
	//set tailles et espaces des cubes
	void setDistances(float rayon, float longueur, float separation);
	//set taille de la fenêtre
	void setDimFenetre(double width, double height);
	//change la valeur de zoom. (addition avec l'ancienne valeur)
	void zoom(GLfloat mod);
	//translation
	void translation(GLfloat axeX, GLfloat axeY);
	//rotation
	void rotation(GLfloat dx, GLfloat dy);
	//remet à 0 les distances de translation
	void recentrer();
	//active ou désactive l'autoroll
	void setAutoroll(bool set);
	//activation du court-circuitage d'affichages des objets de certaines dimensions
	void setAffichageDim(Dim d, bool set = true);
	//paramétrage pour enregistrer le résultat aux formats obj ou morse après une étape de l'algo
	void setSave(bool obj = true, bool morse = false);
	//accès aux id des lists de calcul des polygones
	std::vector<GLuint>* getFormes(Dim dim);

	//***************************fonctions de chargement***************************
	//charge un obj et le sauvegarde ds objAffiche pour l'afficher
	//return false si pb de chargement
	bool loadObj(std::string path);
	/*charge un pgm et commence le traitement dans DGVF, puis affiche une vue rapide.
	 *init aussi le panneau des clusters
	 *return false si pb de chargement
	 */
	bool Controlleur2::loadPgm(std::string path);

	//***************************fonctions DGVF***************************
	void cellClustering();
	bool isPerfect();
	bool isClusterisable();
	std::shared_ptr<std::vector<DGVF::cellBound>> getCollapses();
	void collapse(int c1, int c2);
	int getDim(int pos);


	//calcule les valeurs du centre selon la taille du cube délimité par les paramètres
	void Controlleur2::computeCenter(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	
	//***************************initialisation de liste***************************
	//initialise les listes de Controlleur avec tracés du Modeleur
	void initiateObjs();
	//vide les listes openGL
	void resetLists();

	//convertit de int vers l'énumération correspondante
	static Dim int2Dim(int d);
private:
	Controlleur2();
	std::shared_ptr<Modeleur> modeleur;//ne pas remplacer par Modeleur::Ptr car inclusion croisée, tout ça...
	DGVF::Ptr dgvf;

	bool afficher = false;
	obj::Vertex center;			//le centre du repère d'affichage
	std::vector<GLuint> listObj[DIM];	//une liste par dimension
	couleur couleurs[DIM];		//une couleur par dimension, (entre 0 et 1)
	GLdouble viewX=800.f, viewH=600.f;		//taille de la fenêtre
	GLfloat translations[3];
	bool autoroll;
	bool affDim[DIM];
	bool saveObj, saveMorse;

	//***************************fonctions de dessin***************************
	//dessine un petit cube tout mignion
	void drawDefault();
	//dessine l'obj tel que listé par le Modeleur
	void drawObj();
	//***************************fonctions privées diverses***************************
	void setViewPort();
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
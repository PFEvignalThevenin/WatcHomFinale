#ifndef CONTROLLEUR2_HPP
#define CONTROLLEUR2_HPP

#include <memory>
#include "GL\freeglut.h"
#include "Engine\Modeleur.hpp"
#include "Engine\DGVF.hpp"
#include "Engine\Trackball.hpp"
#include "Data\Object\Vertex.hpp"
#include "Data\MiscData.hpp"
#include <iostream>

class Modeleur;//inclusion crois�e
/* Classe de contr�le du programme.
 * G�re la logique d'affichage, les comportements g�n�raux, les chargements, tout �a,...
 * Pour les autres d�l�gations, voir avec les autres classes de "Engine/"
 * en particulier : 'Modeleur" pour le tracer openGL
 *
 * les diff�rents objets sont stock�s sous forme de listes openGL puis g�r�s par Controleur.
 * -> il y a une liste par dimension afin de permettre les affichages diff�rents
 */
class Controlleur2
{
public:
	~Controlleur2() = default;
	typedef std::shared_ptr<Controlleur2> Ptr;
	//fonction de singleton et d'acc�s au bidule
	static Ptr get();
	//fonction de rafraichissement utilis�e par la fen�tre d'affichage
	void drawGL();	//g�re les autres fonctions de dessin en fonction du contexte

	//***************************Affichage et navigation***************************
	/*setCouleurs f(dimension)
	 *valeurs en entr�e entre 0 et 255 svp.
	 */
	void setCouleur(Dim dim, GLfloat rouge, GLfloat vert, GLfloat bleu, GLfloat alpha=255);
	//set tailles et espaces des cubes
	void setDistances(float rayon, float longueur, float separation);
	//set taille de la fen�tre
	void setDimFenetre(double width, double height);
	//change la valeur de zoom. (addition avec l'ancienne valeur)
	void zoom(GLfloat mod);
	//translation
	void translation(GLfloat axeX, GLfloat axeY);
	//rotation
	void startRotation(int x, int y);
	void rotation(int x, int y);//x et y des positions
	void stopRotation();
	//remet � 0 les distances de translation
	void recentrer();
	//active ou d�sactive l'autoroll
	void setAutoroll(bool set);
	//activation du court-circuitage d'affichages des objets de certaines dimensions
	void setAffichageDim(Dim d, bool set = true);

	//***************************fonctions de chargement***************************
	//charge un obj et le sauvegarde ds objAffiche pour l'afficher
	//return false si pb de chargement
	bool loadObj(std::string path);
	//initialise les listes de Controlleur avec trac�s du Modeleur
	void initiateObjs();
	/*charge un pgm et commence le traitement dans DGVF, puis affiche une vue rapide.
	 *init aussi le panneau des clusters
	 *return false si pb de chargement
	 */
	bool Controlleur2::loadPgm(std::string path);

	//***************************fonctions de sauvegarde***************************
	bool saveObj(std::string path);
	bool saveMorse(std::string path);

	//******************************fonctions DGVF*********************************
	void cellClustering();
	bool isPerfect();
	bool isClusterisable();
	std::shared_ptr<std::vector<DGVF::cellBound>> getCollapses();
	void collapse(int c1, int c2);
	int getDim(int pos);
	int getNbrIterations();//renvoie le nombre de fois qu'in collapse ou un merge a �t� effectu�
	void retourIterPrecedente();//recharge l'it�ration pr�c�dente et supprime la derni�re

	//calcule les valeurs du centre selon la taille du cube d�limit� par les param�tres
	void Controlleur2::computeCenter(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);

	//***************************Fontions de liste***************************
	//acc�s aux id des lists de calcul des polygones
	std::vector<GLuint>* getFormes(Dim dim);
	//vide les listes openGL et la map associ�e : 'infoListObj'
	void resetLists();
	void setNameList(GLuint liste, std::string nom);
	std::string getNameList(int liste);
	void setAfficherList(GLuint liste, bool aff = true);
	bool getAfficherListe(GLuint liste);

	//convertit de int vers l'�num�ration correspondante
	static Dim int2Dim(int d);
private:
	Controlleur2();
	std::shared_ptr<Modeleur> modeleur;//ne pas remplacer par Modeleur::Ptr car inclusion crois�e, tout �a...
	DGVF::Ptr dgvf;

	std::vector<std::shared_ptr<std::vector<DGVF::clustMap>>> etapesSave;//tableau des clusters par �tapes
	bool afficher = false;
	obj::Vertex center;			//le centre du rep�re d'affichage
	std::vector<GLuint> listObj[DIM];	//une liste par dimension
	std::map<int, std::pair<std::string, bool>> infoListObj;//associe � un liste, un nom et l'autorisation d'afficher
	couleur couleurs[DIM];		//une couleur par dimension, (entre 0 et 1)
	GLdouble viewX=800.f, viewH=600.f;		//taille de la fen�tre
	GLfloat translations[3];
	Trackball tb;
	bool autoroll;
	bool affDim[DIM];

	//******************************fonctions de dessin******************************
	//dessine un petit cube tout mignion
	void drawDefault();
	//dessine l'obj tel que list� par le Modeleur
	void drawObj();
	//***************************fonctions priv�es diverses***************************
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
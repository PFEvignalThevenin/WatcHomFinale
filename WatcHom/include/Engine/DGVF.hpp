#ifndef DGVF_HPP
#define DGVF_HPP

#include "Data\ComplexeCubique.hpp"
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <vector>
#include <memory>
#include <iostream>

class DGVF
{
public:
	typedef std::shared_ptr<DGVF> Ptr;
	typedef std::pair<int, int> cellBound;
	typedef std::list<int> cellList;
	typedef std::set<int> cellSet;
	typedef std::pair<int, cellList> cluster;
	typedef std::map<int, cellList> clustMap;

	DGVF(ComplexeCubique::Ptr p_K);
	~DGVF() = default;

	//fonction principale. Sur Console
	void homology(std::string name, double r, double s);
	//calculer les clusters. Puis updateComplex();
	void CellClustering();
	//calcul des clusters pour une dimension donn�e. Dim>0
	void CellClusteringByDim(int dim);
	// update d, g and inv_g � partir des clusters calcul�s
	void updateComplex();
	// true if the DGVF is perfect, that is, dM = 0
	bool perfect();
	//V�rifie si il existe au moins un couple de cellules clusterisable
	bool clusterisable();
	/*calcule la liste des clusters qu'il est possible de collapser.
	 *couple de cellules (??,Cr du cluster)
	 */
	std::shared_ptr<std::vector<cellBound>> computeCollapses();
	//affiche la liste calcul�e par 'computeCollapses'
	void  DGVF::afficherChoixCollapse(std::shared_ptr<std::vector<DGVF::cellBound>> choix);
	//ajouter une fl�che ds V, met � jour dM, codmM,...
	void add2V(int sigma, int tau);
	//une map qui a chaque cellule, associe la cellule critique qui d�finit le cluster, tri�s dans un tableau par dimension
	std::shared_ptr<std::vector<std::map<int, int>>> getGinv();
	//G tri� par dimension et genre donn�es s�par�es
	std::shared_ptr<std::vector<clustMap>> getG();

	int getDim(int pos);

	/*param�tre le nombre de threads � utiliser. D�termin� par utilisateur. >0.*/
	void setNbrThread(unsigned int nbr);

	/**********************************Sauvegarde********************************************/
	bool saveMorse(std::string path);
	bool saveObj(std::string path, float r = 0.1, float s = 0.05 );//par piti�, enlevez-moi �a! mettre un �quivalent dans le modeleur.
	void DGVF::addFace(std::vector<int> p, int dir, int axis,
		double r, double s, std::map<std::vector<int>, int> * M,
		std::vector< std::vector<double> > * vertices, std::map<std::vector<int>, int> * index,
		std::vector< std::vector<int> > * faces, std::map<int, std::list<int> > * facesG);//et pour �a aussi!!!!!!!!!!!!!!!!!!!!!!!!
protected:
	//calcule les bords et renvoie l'ensemble des Cr parmis les bords
	std::shared_ptr<cellList> boundary_cr(int c);
	std::shared_ptr<cellList> coboundary_cr(int c);
	//?
	int coface_spread(int c, std::set<int> * fi);    // the only critical coface not in the finals. -1 if nothing
	void spread(int gamma, std::set<int> * bl, std::set<int> * fi);
	//bords et fl�che? semble inutilis�e
	std::list<int> Vd(int c);
	int getV(int c);
	//utilis�e uniquement par add2V, car incertain de la structure � utiliser
	void setV(int c, int vc);

	/*********************   Pleins de fonctions red�coup�es, car trop intriqu�es, et multi-thread   **********************/
	/*trier les cellules par dimension dans les tableaux des cellules critiques*/
	void trierCellulesCritiques();
	//permet de calculer toutes les combinaisons de V selon n (n index de la combinaison)
	cellSet subset(std::set<int> v, int n);
	//choix utilisateur entre 0 et max
	int choix(std::string message, int max);
	//acc�s au complexe trait�
	ComplexeCubique::Ptr getComplexe();

private:
	//indicateur du nombre de threads � utiliser
	unsigned int nbrThread=1;

	std::map<int, int> V;       // the matching. Key : toutes les valeurs de cellules-> val : la cellule bind�e (primaire vers secondaire)
								// => si rien : critique ou secondaire
								//errata : le binding est bidirectionnel
	ComplexeCubique::Ptr K;             // the cubical complex
	bool cubical;               // true: the original complex; false: Morse complex
	std::set<int> Cr[DIM];      // critical cells par dimension
	std::map<int, std::list<int>> g;     // g (Cr -> ensemble de cellules)
	std::map<int, std::set<int> > dM;     // boundary in the Morse complex. A une cellule, ses faces
	std::map<int, std::set<int> > codM;   // coboundary in the Morse complex. A une cellule, ses cofaces

};

//AU TERME DU BINDING
//Cellule primaire : fl�che sortante
//Cellule secondaire : fl�che entrante
//Cellule critique : pas de fl�che
#endif
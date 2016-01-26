#ifndef DGVF_HPP
#define DGVF_HPP

#include "Data\ComplexeCubique.hpp"
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <vector>
#include <memory>

class DGVF
{
public:
	typedef std::shared_ptr<DGVF> Ptr;
	typedef std::pair<int, int> cellBound;
	typedef std::list<int> cellList;
	typedef std::set<int> cellSet;

	DGVF(ComplexeCubique::Ptr p_K);

	//fonction principale
	void homology(std::string name, double r, double s);
	//calculer les clusters
	void CellClustering();	// The cell clustering
	void updateComplex();   // update d, g and inv_g
	bool simpleCollapse();  // do a simple collapse
	bool mergeComplex();    // merge the complex
	bool perfect();         // true if the DGVF is perfect, that is, dM = 0
							//écrit un fichier qui associe à chaque cluster, le cluster de ses faces
	/*calcule la liste des clusters qu'il est possible de collapser.
	 *couple de cellules (??,Cr du cluster)
	 */
	std::shared_ptr<std::vector<cellBound>> computeCollapses();
	//calcule les bords et renvoie l'ensemble des Cr parmis les bords
	std::shared_ptr<cellList> boundary_cr(int c);
	std::shared_ptr<cellList> coboundary_cr(int c);
	//ajouter une flèche ds V, met à jour dM, codmM,...
	void add2V(int sigma, int tau);
	//?
	int coface_spread(int c, std::set<int> * fi);    // the only critical coface not in the finals. -1 if nothing
	void spread(int gamma, std::set<int> * bl, std::set<int> * fi);
	//bords et flèche? semble inutilisée
	std::list<int> Vd(int c);
	int getV(int c);
	//utilisée uniquement par add2V, car incertain de la structure à utiliser
	void setV(int c, int vc);
	void printList(std::list<int> l);

	/******************Fonctions de sauvegarde**********************/
	//Sauvegarde tout dans des fichiers .obj et 'morse'
	void save(std::string name,int iteration, double r, double s);
	// Writes the Morse boundary in a file
	void writeD(std::string fileName);
	/*Fonctions pour création obj*/
	void writeObj(std::string fileName, double r, double s);
	void addFace(std::vector<int> p, int dir, int i_axis,
		double r, double s, std::map<std::vector<int>, int> * M,
		std::vector< std::vector<double> > * vertices, std::map<std::vector<int>, int> * index,
		std::vector< std::vector<int> > * faces, std::map<int, std::list<int> > * facesG);

	/******************   Subset   **********************/
	//permet de calculer toutes les combinaisons de V selon n
	std::set<int> subset(std::set<int> v, int n);

	/******************   Pleins de fonctions redécoupées, car envisager multi-thread   **********************/
	/*paramètre le nombre de threads à utiliser. Déterminé par utilisateur. >0.*/
	void setNbrThread(unsigned int nbr);
	/*trier les cellules par dimension dans les tableaux des cellules critiques*/
	void trierCellulesCritiques();
private:
	//indicateur du nombre de threads à utiliser
	unsigned int nbrThread=1;


	std::map<int, int> V;            // the matching. Key : toutes les valeurs de cellules-> val : la cellule bindée (primaire vers secondaire)
								// => si rien : critique ou secondaire
								//errata : le binding est bidirectionnel
	ComplexeCubique::Ptr K;             // the cubical complex
	bool cubical;               // true: the original complex; false: Morse complex
	std::set<int> Cr[DIM];      // critical cells par dimension
	std::map<int, std::list<int> > g;      // g, à une Cr, un ensemble de cellules
	std::map<int, std::set<int> > dM;     // boundary in the Morse complex. A une cellule, ses faces
	std::map<int, std::set<int> > codM;   // coboundary in the Morse complex. A une cellule, ses cofaces

};

//AU TERME DU BINDING
//Cellule primaire : flèche sortante
//Cellule secondaire : flèche entrante
//Cellule critique : pas de flèche
#endif
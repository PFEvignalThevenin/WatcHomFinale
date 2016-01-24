#ifndef DGVF_H
#define DGVF_H

#include "cubcomplex.h"
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <vector>

using namespace std;

class DGVF
{
public:
    DGVF(CubComplex *p_K);

	//fonction principale
    void homology(string name, double r, double s);
	//calculer les clusters
    void CellClustering();	// The cell clustering
    void updateComplex();   // update d, g and inv_g
    bool simpleCollapse();  // do a simple collapse
    bool mergeComplex();    // merge the complex
    bool perfect();         // true if the DGVF is perfect, that is, dM = 0
	//écrit un fichier qui associe à chaque cluster, le cluster de ses faces
	void writeD(string fileName);	// Writes the Morse boundary in a file

	//calcule les bords et renvoie l'ensemble des Cr parmis les bords
    list<int> boundary_cr(int c);
    list<int> coboundary_cr(int c);
	//ajouter une flèche ds V, met à jour dM, codmM,...
    void add2V(int sigma, int tau);
	//?
    int coface_spread(int c, set<int> * fi);    // the only critical coface not in the finals. -1 if nothing
    void spread(int gamma, set<int> * bl, set<int> * fi);
	//bords et flèche? semble inutilisée
    list<int> Vd(int c);
    int getV(int c);
	//utilisée uniquement par add2V, car pas sur de la structure à utiliser
    void setV(int c, int vc);
    void printList(list<int> l);

	/*Fonctions pour création obj*/
    void writeObj(string fileName, double r, double s);
    void addFace(vector<int> p, int dir, int axis, 
			double r, double s, map<vector<int>,int> * M, 
			vector< vector<double> > * vertices, map<vector<int>,int> * index, 
			vector< vector<int> > * faces, map<int, list<int> > * facesG);
    void writeCuboid(ofstream * file, vector<double> * v, int vertices);
    set<int> subset(set<int> v, int n);//permet de calculer toutes les combinaisons de V selon n
    
private:
    map<int, int> V;            // the matching. Key : toutes les valeurs de cellules-> val : la cellule bindée (primaire vers secondaire)
								// => si rien : critique ou secondaire
								//errata : le binding est bidirectionnel
    CubComplex * K;             // the cubical complex
    bool cubical;               // true: the original complex; false: Morse complex
    vector< set<int> > Cr;      // critical cells par dimension
    map<int,list<int> > g;      // g, à une Cr, un ensemble de cellules
    map<int,set<int> > dM;     // boundary in the Morse complex. A une cellule, ses faces
    map<int,set<int> > codM;   // coboundary in the Morse complex. A une cellule, ses cofaces

};

//AU TERME DU BINDING
//Cellule primaire : flèche sortante
//Cellule secondaire : flèche entrante
//Cellule critique : pas de flèche

#endif // DGVF_H

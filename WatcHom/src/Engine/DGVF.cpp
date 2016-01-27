#include "Engine\DGVF.hpp"

#include <iostream>
#include <fstream>	// ifstream, ofstream
#include <list>
#include <math.h>   // pow()
#include <ctime>
#include <thread>

using namespace std;

DGVF::DGVF(ComplexeCubique::Ptr p_K)
{
	K = p_K;
	cubical = true;
	setNbrThread(1);//ici pour les tests. Enlever après.
	trierCellulesCritiques();
}


void DGVF::homology(string name, double r, double s)
{
	bool b1, b2;//permet de savoir si modifications peuvet encore se faire
	//initialisation
	CellClustering();
	//save
	do
	{
		auto free = computeCollapses();	//calcul des couples
		afficherChoixCollapse(free);	//afficher menu
		int c = choix("Choose free coface (0 to quit):", free->size() + 1);
		if (c == 0) {//quitter si on boude
			b1 = false;
		} else {
			add2V(free->at(c - 1).first, free->at(c - 1).second);//calculer selon choix
			b1 =  true;
		}
		//save
		if (clusterisable() && choix("Do ya wanna merge the complex ? (1:yes; 0:non):", 1) == 1){//si clusterisable
				CellClustering();//si oui
				b2 = true;
		} else {
			b2 = false;
		}
		//save
	} while(!perfect() && (b1 || b2));
	//end
	if (perfect()){//voir : !(b1 || b2)
		cout << "The homology was succesfully computed" << endl;
	}else{
		cout << "You stopped the computation" << endl;
	}
}


void DGVF::CellClustering()
{
	cout << "cell clustering" << endl;
	set<int> final;
	bool idem;
	for (int q = 3; q >= 1; q--)// from dimension 3 to 1
	{
		/* block critical q-1 cells with !=2 critical cofaces */
		set<int> blocked;
		for (int it : Cr[q - 1])
		{
			if (coboundary_cr(it)->size() != 2)
			{
				blocked.insert(it);
				//cout << "blocked <- "<<*it <<"["<<K->dim(*it)<<"]"<<endl;
			}
		}
		do {
			/* We look for a not final critical q-cube */
			int gamma = -1; //cube final choisi
			for (set<int>::iterator it = Cr[q].begin(); it != Cr[q].end() && gamma<0; ++it)
			{
				if (final.count(*it) == 0)
				{
					gamma = *it;
				}
			}
			/* We spread this cube *///flèches qui partent de ce cube
			if (gamma >= 0)
			{
				idem = false;                        // a new gamma was found
				final.insert(gamma);                 // add it to final
				spread(gamma, &blocked, &final);
			}
			else//une cellule finale n'a pas été trouvée, donc on passe à la suivante
			{
				idem = true;
			}
		} while (!idem);
	}
	updateComplex();
}


void DGVF::updateComplex()
{
	cubical = false; // the complex is not cubical anymore
	dM.clear();
	codM.clear();
	cout << "update Complexe..." << endl;
	for (set<int>::iterator it = Cr[0].begin(); it != Cr[0].end(); ++it)//pour toutes les cells critiques de dim 0
	{
		list<int> l;
		l.push_back(*it);//elle est sa seule paire
		g[*it] = l;
	}
	for (int q = 0; q <= 3; q++)   // for chaque dim (sauf 0?)
	{
		for (set<int>::iterator it = Cr[q].begin(); it != Cr[q].end(); ++it) //pour toutes les cells critiques de dim q
		{
			int c = *it;//cellule
			set<int> dM_c;//bords de la cellule
			map<int, list<int> >::iterator it_m = g.find(c);//trouver le cluster de cette cellule
			if (it_m != g.end())//si non vide
			{
				for (list<int>::iterator it_l = it_m->second.begin(); it_l != it_m->second.end(); ++it_l)//pour toutes cells du cluster
				{
					list<int> b = K->boundary(*it_l);//prendre ses bords
					for (list<int>::iterator it_l2 = b.begin(); it_l2 != b.end(); ++it_l2)  // we traverse its boundary (produit matriciel particulier car opti)
					{
						int c2 = *it_l2;
						if (getV(c2) < 0)	// c2 is critical
						{
							//traitement des bords
							/* dM(c) = dM(c) + c2 (in Z2) */
							pair<std::set<int>::iterator, bool> ret;
							ret = dM_c.insert(c2);//je l'insère. Si il y est déjàn returne false
							if (ret.second == false)	// if c2 in dM_c, we remove it.
								dM_c.erase(ret.first);//donc l'enlève car s'annulent
													  //traitement des cobords de même manière
													  /* codM(c2) = codM(c2) + c (in Z2) */
							map<int, set<int> >::iterator it_m = codM.find(c2);
							if (it_m == codM.end())//le cobord n'est pas définit
							{
								//on le définit
								set<int> codM_c2;
								codM_c2.insert(c);
								codM[c2] = codM_c2;
							}
							else//sinon
							{
								//on insère
								ret = it_m->second.insert(c);
								if (ret.second == false)	// if c in codM_c1, we remove it.
									it_m->second.erase(ret.first);//annulation si il y est déjà
							}
						}
					}
				}
			}
			dM[c] = dM_c;//en profiter pour construire dM
		}
	}
}


std::shared_ptr<std::vector<DGVF::cellBound>> DGVF::computeCollapses() {
	shared_ptr<vector<cellBound>> free(new vector<cellBound>());
	for (int q = 0; q < 3; q++)//toutes dimensions sauf 3
	{
		for (int it : Cr[q])//parcourir cellules critiques
		{
			auto l = coboundary_cr(it);
			if (l->size() == 1)//si une unique coface : ??
			{
				free->push_back(cellBound(it, l->front()));//ajouter à la liste
			}
		}
	}
	return free;
}

void  DGVF::afficherChoixCollapse(std::shared_ptr<std::vector<DGVF::cellBound>> choix) {
	int i = 1, q;
	//afficher menu
	cout << "\n--liste des clusters--" << endl;
	for (cellBound cb : *choix) {//afficher les couples de cellules critiques à collapser
		q = K->dim(cb.first);
		cout << i << ") " << q << "_" << cb.first << " -> " << q + 1 << "_" << cb.second << endl;
		i++;
	}
}

bool DGVF::perfect()
{
	for (int q = 0; q < DIM; q++)
	{
		for (int it : Cr[q])
		{
			set<int> s = dM[it];
			if (!s.empty())
				return false;
		}
	}
	return true;
}

shared_ptr<DGVF::cellList> DGVF::boundary_cr(int c)
{
	std::shared_ptr<cellList> b(new DGVF::cellList());
	if (cubical)
	{
		list<int> l = K->boundary(c);
		for (list<int>::iterator it = l.begin(); it != l.end(); ++it)
		{
			if (getV(*it) < 0)
				b->push_back(*it);
		}
	}
	else    // the Morse complex
	{
		map<int, set<int> >::iterator it = dM.find(c);
		if (it != dM.end())
		{
			set<int> s = it->second;
			for (set<int>::iterator it_s = s.begin(); it_s != s.end(); ++it_s)
			{
				if (getV(*it_s) < 0)
					b->push_back(*it_s);
			}
		}
	}
	return b;
}


shared_ptr<DGVF::cellList> DGVF::coboundary_cr(int c)
{
	shared_ptr<cellList> cob(new DGVF::cellList());
	if (cubical)
	{
		list<int> l = K->coboundary(c);
		for (list<int>::iterator it = l.begin(); it != l.end(); ++it)
		{
			if (getV(*it) < 0)
				cob->push_back(*it);
		}
	}
	else    // the Morse complex
	{
		map<int, set<int> >::iterator it = codM.find(c);
		if (it != codM.end())
		{
			set<int> s = it->second;
			for (set<int>::iterator it_s = s.begin(); it_s != s.end(); ++it_s)
			{
				if (getV(*it_s) < 0)
					cob->push_back(*it_s);
			}
		}
	}
	return cob;
}


void DGVF::add2V(int sigma, int tau)
{
	int q = K->dim(sigma);
	setV(sigma, tau);
	setV(tau, sigma);
	Cr[q].erase(sigma);
	Cr[q + 1].erase(tau);
	/* We update dM and codM */
	//if(!cubical) {cout << "V <- " << sigma << ", " << tau << endl;}
	//cout << "V <- " << sigma<<"["<<K->dim(sigma)<<"] - "<< tau<<"["<<K->dim(tau)<<"]"<<endl;
}


int DGVF::coface_spread(int c, set<int> * fi)
{
	int n = 0, cr;

	auto l = coboundary_cr(c);
	//		cout << "coface_spread("<<c<<") - ";
	for (list<int>::iterator it = l->begin(); it != l->end(); ++it)
	{
		//		cout << *it << " ";
		if (fi->count(*it) == 0)
		{
			n++;
			cr = *it;
			//            cout << "(nf) ";
		}
	}
	//    cout << endl;
	return (n == 1) ? cr : -1;
}


void DGVF::spread(int gamma, set<int> * bl, set<int> * fi)
{
	queue<int> Q;
	list<int> g_gamma;
	int sigma, tau;

	if (cubical){
		g_gamma.push_back(gamma);
	}else{
		g_gamma = g[gamma];
	}

	auto l = boundary_cr(gamma);
	for (int it : *l)
		Q.push(it);

	while (!Q.empty())
	{
		sigma = Q.front();
		Q.pop();
		if (getV(sigma) < 0 && bl->count(sigma) == 0) // critical and not blocked
		{
			tau = coface_spread(sigma, fi);	// if it has only one critical coface we save the pair
			if (tau >= 0)
			{
				add2V(sigma, tau);
				/* Update g */
				if (K->dim(tau) == K->dim(gamma))
				{
					if (!cubical)
					{
						map<int, list<int> >::iterator it_m = g.find(tau);
						if (it_m != g.end())
						{
							for (int it_l : it_m->second)
								g_gamma.push_back(it_l);
							g.erase(it_m);
						}
					}
					else
					{
						g_gamma.push_back(tau);
					}
				}
				/* Push new cubes into the queue */
				l = boundary_cr(sigma);
				for (int it : *l)
					Q.push(it);
				l = boundary_cr(tau);
				for (int it : *l) // NOTHING IS GOING INTO Q
					Q.push(it);
			}
		}
	}
	g[gamma] = g_gamma;
}


list<int> DGVF::Vd(int c)//de primaire vers secondaire
{
	list<int> d = K->boundary(c);
	list<int> vd;

	for (list<int>::iterator it = d.begin(); it != d.end(); ++it)
	{
		int v = getV(*it);
		if (v >= 0 && v != c && K->dim(*it) < K->dim(v))//si pointe vers dimension supérieure : secondaire
			vd.push_back(v);//on l'ajoute
	}

	return vd;
}


int DGVF::getV(int c)
{
	map<int, int>::iterator f = V.find(c);
	if (f != V.end())
		return f->second;
	return -1;
}


void DGVF::setV(int c, int vc)
{
	V[c] = vc;
}

/******************   Pleins de fonctions redécoupées, car envisager multi-thread   **********************/
/*paramètre le nombre de threads à utiliser. Déterminé par utilisateur. >0.*/
void DGVF::setNbrThread(unsigned int nbr) {
	nbrThread = nbr;
}
/*trier les cellules par dimension dans les tableaux des cellules critiques*/
void DGVF::trierCellulesCritiques() {
	if (true /*nbrThread == 1*/) {//ne pas toucher tant que pas d'exlusion mutuelle des Cr[]
		for (int c = 0; c < K->nbrCubes(); c++)//trier toutes les cellules par dimension
		{
			if (K->get(c))//there is a cube at position c
				Cr[K->dim(c)].insert(c);
		}
	}
	else {//si utiliser plusieurs threads
		vector<thread> threads;//pour garder les threads
		int sup, inf = 0;
		for (unsigned int i = 1; i <= nbrThread; i++) { //pour chaque thread
			sup= (i*K->nbrCubes()) / nbrThread;//calcul des bornes [min,max[

			//fonction de tri d'une partie du tableau
			threads.push_back(thread([this](int min, int max) {
				for (int c = min; c < max; c++)//trier toutes les cellules par dimension (si ilya une cellule)
				{
					if (K->get(c)) {
						Cr[K->dim(c)].insert(c);// <- ici pb : pas thread safe en ériture
					}
				}
			}, inf, sup));

			inf = sup;//on passe à la suite : ancienne borne sup est la nouvelle min
		}
		for (unsigned int i = 0; i < threads.size(); i++) {//tous les attendre
			threads[i].join();
		}
	}
}

bool DGVF::clusterisable() {
	for (int q = 0; q < 3 ; q++)
	{
		for (int pos : Cr[q])
		{
			auto l = coboundary_cr(pos);
			if (l->size() == 2)
			{
				return true;
			}
		}
	}
	return false;
}

set<int> DGVF::subset(set<int> v, int n)
{
	set<int> sub;
	for (int it : v)
	{
		if (n % 2 == 1)
			sub.insert(it);
		n /= 2;
	}
	return sub;
}

int DGVF::choix(std::string message, int max) {
	int ans;
	max = (max < 0) ? 0 : max;
	cout <<endl<< message<<endl;//demander si clusteriser
	do {
		cin >> ans;
	} while (ans<0 || ans>max);
	return ans;
}

std::shared_ptr<std::vector<map<int, int>>> DGVF::getGinv() {
	/* Compute g_inv */
	/*On parcours tous les clusters et on cherche auquel appartient la cellule */
	//g : cluster
	//g_inv : map qui associe à une cellule un numéro de cluster (numéro de la cellule critique qui le définit après calcul)
	//	le numéro du cluster est celui d'ne cellule de même dimension. Dim0 : 1!cellule
	vector<map<int, int>> g_inv(DIM);//cellules secondaire vers cellule critique, donc définit le cluster
	for (int q = 0; q < DIM; q++)   // for each dimension
	{
		for (int it : Cr[q])//pour chaque cellule critique de dim q
		{
			map<int, list<int> >::iterator it_m = g.find(it);//itérateur vers g
			if (it_m != g.end())//si non vide : cad si la Cr est ds g : cad définit un cluster->liste
			{
				for (int it_l : it_m->second)//mettre à jour g_inv : associer le num du cluster pour à cellule de ce cluster
				{
					if (g_inv[q].count(it_l) > 0) {//débogage
						throw DataError("Error: " + to_string(it_l) + " has two inverses by g");
					}
					g_inv[q][it_l] = it;
				}
			}
		}
	}
	return make_shared<vector<map<int, int>>>(g_inv);
}

int DGVF::getDim(int pos) {
	return K->dim(pos);
}

ComplexeCubique::Ptr DGVF::getComplexe() {
	return K;
}
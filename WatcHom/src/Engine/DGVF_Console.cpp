#include "Engine\DGVF_Console.hpp"
#include <iostream>
#include <fstream>	// ifstream, ofstream
#include <list>
#include <math.h>   // pow()
#include <ctime>
#include <thread>

using namespace std;

DGVF_Console::DGVF_Console(ComplexeCubique::Ptr p_K)
{
	K = p_K;
	cubical = true;
	setNbrThread(1);//ici pour les tests. Enlever après.
	trierCellulesCritiques();
}


void DGVF_Console::homology(string name, double r, double s)
{
	int i = 1;
	bool b1, b2;
	cout << "Merging complexe" << endl;
	CellClustering();
	cout << "Complexe merged" << endl;
	save(name, i, r, s);
	i++;
	do
	{
		b1 = simpleCollapse();
		if (b1)
		{
			save(name, i, r, s);
			i++;
		}
		b2 = mergeComplex();
		if (b2)
		{
			save(name, i, r, s);
			i++;
		}
	} while (!perfect() && (b1 || b2));
	if (perfect()) {
		cout << "The homology was succesfully computed" << endl;
	}
	else {
		cout << "You stopped the computation" << endl;
	}
}


void DGVF_Console::CellClustering()
{
	cout << "Cell clustering in progress..." << endl;
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
		cout << "Cells blocked..." << endl;
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
	/* Now the complex is not cubical */
	cout << "updating..." << endl;
	updateComplex();
	cout << "Cell clustering done." << endl;
}


void DGVF_Console::updateComplex()
{
	cubical = false; // the complex is not cubical anymore
	dM.clear();
	codM.clear();
	cout << "update dim0..." << endl;
	for (set<int>::iterator it = Cr[0].begin(); it != Cr[0].end(); ++it)//pour toutes les cells critiques de dim 0
	{
		list<int> l;
		l.push_back(*it);//elle est sa seule paire
		g[*it] = l;
	}
	cout << "update autres dims..." << endl;
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


bool DGVF_Console::simpleCollapse()
{
	auto free = computeCollapses();//calcul des couples

								   //menu de sélection des cofaces pour collapser
	cout << "\nChoose free coface (0 to quit):" << endl;
	int i = 1, c, q;
	//afficher menu
	for (cellBound cb : *free) {//afficher les couples de cellules critiques à collapser
		q = K->dim(cb.first);
		cout << i << ") " << q << "_" << cb.first << " -> " << q + 1 << "_" << cb.second << endl;
		i++;
	}
	do {
		cin >> c;//choisir
	} while (c >= i || c < 0);
	if (c == 0)//quitter si on boude
		return false;
	add2V(free->at(c - 1).first, free->at(c - 1).second);//calculer selon choix
	return true;
}


std::shared_ptr<std::vector<DGVF_Console::cellBound>> DGVF_Console::computeCollapses() {
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


bool DGVF_Console::mergeComplex()
{
	if (clusterisable())//si clusterisable
	{
		int ans;
		cout << "\nDo ya wanna merge the complex ? (1:yes; 0:non): ";//demander si clusteriser
		cin >> ans;
		if (ans == 0)//si non
			return false;
		CellClustering();//si oui
		return true;
	}
	return false;
}


bool DGVF_Console::perfect()
{
	for (int q = 0; q < DIM; q++)
	{
		for (set<int>::iterator it = Cr[q].begin(); it != Cr[q].end(); ++it)
		{
			set<int> s = dM[*it];
			if (!s.empty())
				return false;
		}
	}
	return true;
}


shared_ptr<DGVF_Console::cellList> DGVF_Console::boundary_cr(int c)
{
	std::shared_ptr<cellList> b(new DGVF_Console::cellList());
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


shared_ptr<DGVF_Console::cellList> DGVF_Console::coboundary_cr(int c)
{
	shared_ptr<cellList> cob(new DGVF_Console::cellList());
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


void DGVF_Console::add2V(int sigma, int tau)
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


int DGVF_Console::coface_spread(int c, set<int> * fi)
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


void DGVF_Console::spread(int gamma, set<int> * bl, set<int> * fi)
{
	queue<int> Q;
	list<int> g_gamma;
	int sigma, tau;

	if (cubical) {
		g_gamma.push_back(gamma);
	}
	else {
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


list<int> DGVF_Console::Vd(int c)//de primaire vers secondaire
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


int DGVF_Console::getV(int c)
{
	map<int, int>::iterator f = V.find(c);
	if (f != V.end())
		return f->second;
	return -1;
}


void DGVF_Console::setV(int c, int vc)
{
	V[c] = vc;
}


void DGVF_Console::printList(list<int> l)
{
	for (int it : l)
	{
		cout << it << " ";
	}cout << endl;
}

/******************   Pleins de fonctions redécoupées, car envisager multi-thread   **********************/
/*paramètre le nombre de threads à utiliser. Déterminé par utilisateur. >0.*/
void DGVF_Console::setNbrThread(unsigned int nbr) {
	nbrThread = nbr;
}
/*trier les cellules par dimension dans les tableaux des cellules critiques*/
void DGVF_Console::trierCellulesCritiques() {
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
			sup = (i*K->nbrCubes()) / nbrThread;//calcul des bornes [min,max[

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
bool DGVF_Console::clusterisable() {
	for (int q = 0; q < 3; q++)
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

set<int> DGVF_Console::subset(set<int> v, int n)
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

/**********************************Fonctions de Sauvegarde fichier******************************************/
void DGVF_Console::save(std::string name, int iteration, double r, double s) {
	//todo : vérifier et créer dossier log
	writeD("log/" + name + "_" + to_string(iteration) + "_d");
	writeObj("log/" + name + "_" + to_string(iteration) + "_V.obj", r, s);
}

void DGVF_Console::writeD(string fileName)
{
	/* Create the file */
	ofstream file(fileName.c_str(), ios::out | ios::trunc);
	if (!file)
	{
		cout << "Error in DGVF_Console::writeD(" << fileName << "): impossible to create the output text file." << endl;
		return;
	}

	for (int q = 0; q < DIM; q++)
	{
		for (set<int>::iterator it = Cr[q].begin(); it != Cr[q].end(); ++it)
		{
			//cout << "d_" << q << "(" << *it << ") = ";
			file << "\"" << q << "_" << *it << "\": ";
			auto l = boundary_cr(*it);
			for (list<int>::iterator it_l = l->begin(); it_l != l->end(); ++it_l)
			{
				//cout << *it_l << " ";
				file << "\"" << q - 1 << "_" << *it_l << "\" ";
			}
			//cout << endl;
			file << endl;
		}
	}
	file.close();
}

void DGVF_Console::writeObj(string fileName, double r, double s)
{
	clock_t begin = clock();

	/* Create the file */
	ofstream file(fileName.c_str(), ios::out | ios::trunc);
	if (!file)
	{
		cout << "Error in DGVF_Console::write2Obj(" << fileName << "): impossible to create the output text file." << endl;
		return;
	}
	file << "# Obj file created by WatcHom (Aldo Gonzalez-Lorenzo)" << endl;//commentaire de l'auteur
	if (0 >= s || s >= r || r >= 1)//vérifications paramètres de distances (rayon et séparation)
	{
		throw DataError("Error in fileName: wrong parameters. Choose 0 < s < r < 1 s= " + to_string(s) + " ed r = " + to_string(r));
	}
	/* Compute g_inv */
	/*On parcours tous les clusters et on cherche auquel appartient la cellule */
	//g : cluster
	//g_inv : map qui associe à une cellule un numéro de cluster (numéro de la cellule critique qui le définit après calcul)
	map<int, int> g_inv;//cellules secondaire vers cellule critique, donc définit le cluster
	for (int q = 0; q < DIM; q++)   // for each dimension
	{
		for (int it : Cr[q])//pour chaque cellule critique de dim q
		{
			map<int, list<int> >::iterator it_m = g.find(it);//itérateur vers g
			if (it_m != g.end())//si non vide : cad si la Cr est ds g : cad définit un cluster->liste
			{
				for (int it_l : it_m->second)//mettre à jour g_inv : associer le num du cluster pour à cellule de ce cluster
				{
					if (g_inv.count(it_l) > 0) {//débogage
						throw DataError("Error: " + to_string(it_l) + " has two inverses by g");
					}
					g_inv[it_l] = it;
				}
			}
		}
	}

	/* We compute the shape of the complex, leaving spaces between the cells */
	map< vector<int>, int > M;   // cuboids printed
	for (int q = 0; q <= 3; q++)//pour chaque dimension
	{
		for (int c = 0; c < K->nbrCubes(); c++)//pour toutes les cellules
		{
			map<int, int>::iterator it_g = g_inv.find(c);//itérateur qui pointe sur c
			if (it_g != g_inv.end() && K->dim(c) == q)//si de dimension q
			{
				obj::coord temp_coord = K->pos2coord(c);//lire coords
				vector<int> coor(3);					//et convertir
				coor[0] = temp_coord.x;
				coor[1] = temp_coord.y;
				coor[2] = temp_coord.z;
				//cout << "c = " << c << ", ["<<coor[0]<<" "<<coor[1]<<" "<<coor[2]<<"], g-inv = "<<it_g->second<<endl;
				set<int> v;          // set des coords impaires
				for (int i = 0; i < 3; i++)//pour chaque coords
				{
					if (coor.at(i) % 2 == 1)//si position impaire
						v.insert(i);//insérer ds v
				}
				for (int i1 = 0; i1 < pow(2, v.size()); i1++)//de 0 à 2^taille (nbr de sous ensembles possibles)
				{
					set<int> v1 = subset(v, i1);     // corner that we check, in absolute value
					for (int i2 = 0; i2 < pow(2, v1.size()); i2++)
					{
						set<int> v2 = subset(v1, i2);     // the coordinates that are negative
						vector<int> p = coor; p[0] = 2 * p[0]; p[1] = 2 * p[1]; p[2] = 2 * p[2];//avoir espace pour afficher points
						for (set<int>::iterator it = v1.begin(); it != v1.end(); ++it)
							p.at(*it) += 2;
						for (set<int>::iterator it = v2.begin(); it != v2.end(); ++it)
							p.at(*it) -= 4;
						//                        cout << "  p = "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
						map<vector<int>, int>::iterator f = M.find(p);
						if (f == M.end() || f->second == it_g->second) // if that cuboid does not exist or was put by the same critical cell
						{
							for (int i3 = 0; i3 < pow(2, v1.size()); i3++)
							{
								set<int> v3 = subset(v1, i3);     // the coordinates that are negative
								vector<int> p2 = p;
								for (set<int>::iterator it = v3.begin(); it != v3.end(); ++it)
								{
									p2.at(*it) = (v2.count(*it) > 0) ? p2.at(*it) + 1 : p2.at(*it) - 1;
								}
								M[p2] = it_g->second;
								//                                cout << "write "<<p2[0]<<" "<<p2[1]<<" "<<p2[2]<<" -> "<<it_g->second<<endl;
							}
						}
					}
				}
			}
		}
	}

	/* We decide which square faces we will print */
	vector< vector<double> > vertices;
	map<vector<int>, int> index;
	vector< vector<int> > faces;
	map<int, list<int> > facesG;
	for (map< vector<int>, int >::iterator it = M.begin(); it != M.end(); ++it)
	{
		vector<int> p = it->first;
		addFace(p, +1, 0, r, s, &M, &vertices, &index, &faces, &facesG);
		addFace(p, -1, 0, r, s, &M, &vertices, &index, &faces, &facesG);
		addFace(p, +1, 1, r, s, &M, &vertices, &index, &faces, &facesG);
		addFace(p, -1, 1, r, s, &M, &vertices, &index, &faces, &facesG);
		addFace(p, +1, 2, r, s, &M, &vertices, &index, &faces, &facesG);
		addFace(p, -1, 2, r, s, &M, &vertices, &index, &faces, &facesG);
	}
	//	cout << "faces computed" << endl;

	/* Write the file */
	file << "# " << vertices.size() << " vertices" << endl;
	file << "# " << faces.size() << " square faces" << endl;
	for (vector<vector<double> >::iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		file << "v " << it->at(0) << " " << it->at(1) << " " << it->at(2) << endl;
	}
	for (int q = 0; q <= 3; q++)
	{
		for (set<int>::iterator it_cr = Cr[q].begin(); it_cr != Cr[q].end(); ++it_cr)
		{
			file << "o " << q << "_" << *it_cr << endl;
			list<int> l = facesG[*it_cr];
			for (list<int>::iterator it_l = l.begin(); it_l != l.end(); ++it_l)
			{
				vector<int> v = faces.at(*it_l);
				file << "f " << v.at(0) << " " << v.at(1) << " " << v.at(2) << " " << v.at(3) << endl;
			}
		}
	}
	file.close();

	cout << fileName << " written in " << double(clock() - begin) / CLOCKS_PER_SEC << " secs." << endl;
}

void DGVF_Console::addFace(vector<int> p, int dir, int axis,
	double r, double s, map<vector<int>, int> * M,
	vector< vector<double> > * vertices, map<vector<int>, int> * index,
	vector< vector<int> > * faces, map<int, list<int> > * facesG)
{
	vector<int> next = p; next[axis] += dir;
	//on ne doit pas avoir un voisin
	if (next[axis] < 0 || next[axis] >= 2 * K->getSize(int2Axe(axis)) - 1 || M->count(next) == 0)
	{
		vector< vector<int> > a(4);
		vector<int> v = p;
		if (dir>0)// Calcul de 4 sommets de la face. pour changer selon dim?
		{
			v[axis]++;			a[0] = v;
			v[(axis + 1) % 3]++;	a[1] = v;
			v[(axis + 2) % 3]++;	a[2] = v;
			v[(axis + 1) % 3]--;	a[3] = v;
		}
		else
		{
			a[0] = v;
			v[(axis + 2) % 3]++;	a[1] = v;
			v[(axis + 1) % 3]++;	a[2] = v;
			v[(axis + 2) % 3]--;	a[3] = v;
		}

		vector<int> face(4);
		for (int i = 0; i < 4; i++)
		{
			map<vector<int>, int>::iterator it = index->find(a[i]);//chercher sommet
			if (it == index->end())	// if the vertex is found for the first time
			{
				vector<double> a2(3);
				for (int j = 0; j < 3; j++)//le creer
				{
					if (a[i][j] % 4 == 0)
						a2[j] = a[i][j] / 4 - r + s;
					else if (a[i][j] % 4 == 1)
						a2[j] = a[i][j] / 4 + r - s;
					else if (a[i][j] % 4 == 2)
						a2[j] = a[i][j] / 4 + r + s;
					else if (a[i][j] % 4 == 3)
						a2[j] = a[i][j] / 4 + 1 - r - s;
				}
				vertices->push_back(a2);//l'ajouter
				index->insert(pair<vector<int>, int>(a[i], vertices->size()));//avec map pour rapidité
				face[i] = vertices->size();//index du sommet
			}
			else//sinon utiliser celui trouvé
			{
				face[i] = it->second;
			}
		}
		/* We add the new face */
		faces->push_back(face);
		map<int, list<int> >::iterator it = facesG->find(M->at(p));//chercher cluster
		if (it == facesG->end())//si pas encore définit
		{
			list<int> l;
			l.push_back(faces->size() - 1);
			facesG->insert(pair<int, list<int> >(M->at(p), l));//ajouter
		}
		else
		{
			it->second.push_back(faces->size() - 1);
		}
	}
}

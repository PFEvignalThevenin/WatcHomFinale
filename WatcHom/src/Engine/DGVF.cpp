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
	setNbrThread(1);//ici pour les tests. Enlever apr�s.
	trierCellulesCritiques();
}


void DGVF::homology(string name, double r, double s)
{
	int i = 1;
	bool b1, b2;
	cout << "Merging complexe" << endl;
	mergeComplex();
	cout << "Complexe merged" << endl;
	save( name , i , r, s);
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
	if (perfect()){
		cout << "The homology was succesfully computed" << endl;
	}else{
		cout << "You stopped the computation" << endl;
	}
}


void DGVF::CellClustering()
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
			/* We spread this cube *///fl�ches qui partent de ce cube
			if (gamma >= 0)
			{
				idem = false;                        // a new gamma was found
				final.insert(gamma);                 // add it to final
				spread(gamma, &blocked, &final);
			}
			else//une cellule finale n'a pas �t� trouv�e, donc on passe � la suivante
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


void DGVF::updateComplex()
{
	cubical = false; // the complex is not cubical anymore
	dM.clear();
	codM.clear();
	cout << "update dim0..." << endl;
	for (set<int>::iterator it = Cr[0].begin(); it != Cr[0].end(); ++it)//pour toutes les cells critiques de dim 0
	{
		list<int> l; l.push_back(*it);//trivial
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
							ret = dM_c.insert(c2);//je l'ins�re. Si il y est d�j�n returne false
							if (ret.second == false)	// if c2 in dM_c, we remove it.
								dM_c.erase(ret.first);//donc l'enl�ve car s'annulent
													  //traitement des cobords de m�me mani�re
													  /* codM(c2) = codM(c2) + c (in Z2) */
							map<int, set<int> >::iterator it_m = codM.find(c2);
							if (it_m == codM.end())//le cobord n'est pas d�finit
							{
								//on le d�finit
								set<int> codM_c2;
								codM_c2.insert(c);
								codM[c2] = codM_c2;
							}
							else//sinon
							{
								//on ins�re
								ret = it_m->second.insert(c);
								if (ret.second == false)	// if c in codM_c1, we remove it.
									it_m->second.erase(ret.first);//annulation si il y est d�j�
							}
						}
					}
				}
			}
			dM[c] = dM_c;//en profiter pour construire dM
		}
	}
}


bool DGVF::simpleCollapse()
{
	//menu de s�lection des cofaces pour collapser
	cout << "\nChoose free coface (0 to quit):" << endl;
	int i = 1, c,q;
	auto free = computeCollapses();
	//afficher menu
	for (cellBound cb : *free) {//afficher les couples de cellules critiques � collapser
		q = K->dim(cb.first);
		cout << i << ") " << q << "_" << cb.first << " -> " << q + 1 << "_" << cb.second << endl;
		i++;
	}
	do{
		cin >> c;//choisir
	} while (c >= i || c < 0);
	if (c == 0)//quitter si on boude
		return false;
	add2V(free->at(c - 1).first, free->at(c - 1).second);//calculer selon choix
	return true;
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
				free->push_back(cellBound(it, l->front()));//ajouter � la liste
			}
		}
	}
	return free;
}

bool DGVF::mergeComplex()
{
	if (cubical)//la premi�re fois, on ne v�rifie pas si clusterisable, car le sera forcement (depuis pgm)
	{
		CellClustering();
		return true;
	}
	//trouver cellule avec 2 cofaces pour les assembler
	bool b = false;//v�rifie si clusterisable
	for (int q = 0; q < 3 && !b; q++)
	{
		for (set<int>::iterator it = Cr[q].begin(); it != Cr[q].end() && !b; ++it)
		{
			auto l = coboundary_cr(*it);
			if (l->size() == 2)
			{
				b = true;
			}
		}
	}
	if (b)//si clusterisable
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


bool DGVF::perfect()
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


void DGVF::writeD(string fileName)
{
	/* Create the file */
	ofstream file(fileName.c_str(), ios::out | ios::trunc);
	if (!file)
	{
		cout << "Error in DGVF::writeD(" << fileName << "): impossible to create the output text file." << endl;
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
	//    if(!cubical) cout << "gamma = " << gamma <<"["<<K->dim(gamma)<<"]"<<endl;

	list<int> g_gamma;
	if (cubical)
	{
		g_gamma.push_back(gamma);
	}
	else
	{
		g_gamma = g[gamma];
	}

	auto l = boundary_cr(gamma);
	for (list<int>::iterator it = l->begin(); it != l->end(); ++it)
		Q.push(*it);

	while (!Q.empty())
	{
		int sigma = Q.front(); Q.pop();
		//		cout << "Q -> "<<sigma<<endl;
		if (getV(sigma) < 0 && bl->count(sigma) == 0) // critical and not blocked
		{
			int tau = coface_spread(sigma, fi);	// if it has only one critical coface we save the pair
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
							//                            g_gamma.insert(g_gamma.end(), it_m->second.begin(), it_m->second.end());
							for (list<int>::iterator it_l = it_m->second.begin(); it_l != it_m->second.end(); ++it_l)
								g_gamma.push_back(*it_l);
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
				for (list<int>::iterator it = l->begin(); it != l->end(); ++it)
				{
					Q.push(*it);
				}
				l = boundary_cr(tau);
				//                cout << "boundary " << tau << " "; printList(l); cout<<endl;
				for (list<int>::iterator it = l->begin(); it != l->end(); ++it) // NOTHING IS GOING INTO Q
				{
					Q.push(*it);
				}
			}
		}
	}
	g[gamma] = g_gamma;
	//    cout << "spread(" << gamma << ") done." << endl;
}


list<int> DGVF::Vd(int c)//de primaire vers secondaire
{
	list<int> d = K->boundary(c);
	list<int> vd;

	for (list<int>::iterator it = d.begin(); it != d.end(); ++it)
	{
		int v = getV(*it);
		if (v >= 0 && v != c && K->dim(*it) < K->dim(v))//si pointe vers dimension sup�rieure : secondaire
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


void DGVF::printList(list<int> l)
{
	for (list<int>::iterator it = l.begin(); it != l.end(); ++it)
	{
		cout << *it << " ";
	}
}


//interpr�tation du cubique pour l'enregistrer sous format obj
void DGVF::writeObj(string fileName, double r, double s)
{
	clock_t begin = clock();

	/* Create the file */
	ofstream file(fileName.c_str(), ios::out | ios::trunc);
	if (!file)
	{
		cout << "Error in DGVF::write2Obj(" << fileName << "): impossible to create the output text file." << endl;
		return;
	}
	file << "# Obj file created by WatcHom (Aldo Gonzalez-Lorenzo)" << endl;//commentaire de l'auteur
	if (0 >= s || s >= r || r >= 1)//v�rifications param�tres de distances (rayon et s�paration)
	{
		throw DataError("Error in fileName: wrong parameters. Choose 0 < s < r < 1 s= "+to_string(s)+" ed r = "+ to_string(r));
	}
	/* Compute g_inv */
	/*On parcours tous les clusters et on v�rifie auquel la cellule appartient*/
	//g : cluster
	//g_inv : map qui associe � une cellule un num�ro de cluster (num�ro de la cellule critique qui le d�finit apr�s calcul)
	map<int, int> g_inv;//cellules secondaire vers cellule critique, donc d�finit le cluster
	for (int q = 0; q <= 3; q++)   // for each dimension
	{
		for (set<int>::iterator it = Cr[q].begin(); it != Cr[q].end(); ++it)//pour chaque cell critique de dim q
		{
			map<int, list<int> >::iterator it_m = g.find(*it);//it�rateur vers g
			if (it_m != g.end())//si non vide
			{
				for (list<int>::iterator it_l = it_m->second.begin(); it_l != it_m->second.end(); ++it_l)//mettre � jour g_inv
				{
					if (g_inv.count(*it_l) > 0) {//d�bogage
						throw DataError("Error: "+ to_string(*it_l) +" has two inverses by g");
					}
					g_inv[*it_l] = *it;
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
			map<int, int>::iterator it_g = g_inv.find(c);//it�rateur qui pointe sur c
			if (it_g != g_inv.end() && K->dim(c) == q)//si de dimension q
			{
				obj::coord temp_coord = K->pos2coord(c);//lire coords
				vector<int> coor(3);					//et convertir
				coor[0]=temp_coord.x;
				coor[1] = temp_coord.y;
				coor[2] = temp_coord.z;
				//cout << "c = " << c << ", ["<<coor[0]<<" "<<coor[1]<<" "<<coor[2]<<"], g-inv = "<<it_g->second<<endl;
				set<int> v;          // set des coords impaires
				for (int i = 0; i < 3; i++)//pour chaque coords
				{
					if (coor.at(i) % 2 == 1)//si position impaire
						v.insert(i);//ins�rer ds v
				}
				for (int i1 = 0; i1 < pow(2, v.size()); i1++)//de 0 � 2^taille (nbr de sous ensembles possibles)
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


void DGVF::addFace(vector<int> p, int dir, int axis,
	double r, double s, map<vector<int>, int> * M,
	vector< vector<double> > * vertices, map<vector<int>, int> * index,
	vector< vector<int> > * faces, map<int, list<int> > * facesG)
{
	vector<int> next = p; next[axis] += dir;

	if (next[axis] < 0 || next[axis] >= 2 * K->getSize(int2Axe(axis)) - 1 || M->count(next) == 0)
	{
		vector< vector<int> > a(4);
		vector<int> v = p;
		if (dir>0)
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
			map<vector<int>, int>::iterator it = index->find(a[i]);
			if (it == index->end())	// if the vertex is found for the first time
			{
				vector<double> a2(3);
				for (int j = 0; j < 3; j++)
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
				vertices->push_back(a2);
				//				cout << "vertices <- ["<<a2[0]<<","<<a2[1]<<","<<a2[2]<<"]"<<endl;
				index->insert(pair<vector<int>, int>(a[i], vertices->size()));
				face[i] = vertices->size();
			}
			else
			{
				face[i] = it->second;
			}
		}
		/* We add the new face */
		faces->push_back(face);
		map<int, list<int> >::iterator it = facesG->find(M->at(p));
		if (it == facesG->end())
		{
			list<int> l;
			l.push_back(faces->size() - 1);
			facesG->insert(pair<int, list<int> >(M->at(p), l));
		}
		else
		{
			it->second.push_back(faces->size() - 1);
		}
	}
	//cout << "nextVoxel(voxel=" << voxel << ", dir=" << dir << ", axis=" << axis << ") computed" << endl;
}

//permet d'avoir toutes les combinaisons possibles de v, en fonction de n (n index de la combinaison)
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

void DGVF::save(std::string name, int iteration, double r, double s) {
	//todo : v�rifier et cr�er dossier log
	writeD("log/" + name + "_" + to_string(iteration) + "_d");
	writeObj("log/" + name + "_" + to_string(iteration) + "_V.obj", r, s);
}

/******************   Pleins de fonctions red�coup�es, car envisager multi-thread   **********************/
/*param�tre le nombre de threads � utiliser. D�termin� par utilisateur. >0.*/
void DGVF::setNbrThread(unsigned int nbr) {
	nbrThread = nbr;
}
/*trier les cellules par dimension dans les tableaux des cellules critiques*/
void DGVF::trierCellulesCritiques() {
	if (nbrThread == true /*1*/) {//ne pas toucher tant que pas d'exlusion mutuelle des Cr[]
		for (int c = 0; c < K->nbrCubes(); c++)//trier toutes les cellules par dimension
		{
			if (K->get(c))//there is a cube at position c
				Cr[K->dim(c)].insert(c);
		}
	}
	else {
		//vector<int> vals(nbrThread);//bornes de travail des threads
		vector<thread> threads;
		int sup, inf = 0;
		for (unsigned int i = 1; i <= nbrThread; i++) { //vals.size = nbrThread
			//vals[i] = (i*K->nbrCubes()) / nbrThread;
			sup= (i*K->nbrCubes()) / nbrThread;//calcul des bornes [min,max[
			threads.push_back(thread([this](int min, int max) {
				for (int c = min; c < max; c++)//trier toutes les cellules par dimension
				{
					if (K->get(c)) {//there is a cube at position c
						Cr[K->dim(c)].insert(c);
					}
				}
			}, inf, sup));
			inf = sup;//on passe � la suite
		}
		for (unsigned int i = 0; i < threads.size(); i++) {
			threads[i].join();
		}
	}
}
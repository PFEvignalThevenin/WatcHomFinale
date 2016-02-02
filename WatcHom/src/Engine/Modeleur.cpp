#include "Engine\Modeleur.hpp"
#include <algorithm>
#include <iostream>
using namespace std;
using namespace obj;

Modeleur::Modeleur(std::shared_ptr<Controlleur2> ctrl)
{
	this->ctrl = ctrl;
	setDistances(1, 2, 1);
}
//****************************************Dessin de PGM ************************************************
void Modeleur::setPgm(PGM3D::Ptr pgm) {
	pgmTraite = pgm;
}
void Modeleur::drawPgm(PGM3D &pgm) {
	coord co;
	Vertex ve;
	for (int i = 0; i < pgm.nbrCubes(); i++) {
		if (pgm.isOccupied(i)) {
			co = pgm.pos2coord(i);
			ve.x = co.x*dist;
			ve.y = co.y*dist;
			ve.z = co.z*dist;
			drawCube0(ve);
		}
	}
}
void Modeleur::initiatePgm() {
	ctrl->resetLists();
	std::vector<GLuint> *listObj = ctrl->getFormes(Dim::d0);
	cout << "affichage PGM..." << endl;
	glNewList(1, GL_COMPILE);	//créer nouvelle liste
	listObj->push_back(1);		//conserver identificateur
	drawPgm(*pgmTraite);
	glEndList();
	cout << "affichage PGM Done" << endl;
	ctrl->computeCenter(
		0, pgmTraite->getSize(Axe::x)*dist,
		0, pgmTraite->getSize(Axe::y)*dist,
		0, pgmTraite->getSize(Axe::z)*dist);
}
//*******************************Dessin de Complexe Cubique par dimension*********************************
void Modeleur::initiateComplexeCubique(shared_ptr<vector<map<int, list<int>>>> g) {
	ctrl->resetLists();
	//traiter les clusters par dimension
	cout << "TODO : \tfaire l'affichage des clusters dans Modeleur::initiateComplexeCubique" << endl;
	cout << "\tfait :  Dim0" << endl;
	cout << "\ta faire :  Dim 1,2,3" << endl;
	int cptr = 1;//compte toutes les listes. Il ne doit pas y avoir de doublons
	std::vector<GLuint> *listObj;//variable des indices de liste d'une dimension
	
	//dim0
	listObj = ctrl->getFormes(Dim::d0);//liste de dim0
	for (DGVF::cluster bind : g->at(0)) {//pour tout les clusters de dim 0
		glNewList(cptr, GL_COMPILE);	//créer nouvelle liste
		listObj->push_back(cptr);		//conserver identificateur
		drawCube0(coord2Vert(ccTraite->pos2coord(bind.first)));//dessin
		glEndList();//fin definition liste
		cptr++;
	}
	//dim1
	cout << "DIM1" << endl;
	vector<dir> Axes;
	listObj = ctrl->getFormes(Dim::d1);//liste de dim0
	try {
		for (DGVF::cluster clust : g->at(1)) {//tous clusters dim 1
			DGVF::cluster cluster = clust;
			//initialisation de la boucle
			//permet de ne pas avoir de boucle infinie
			int initial_size = cluster.second.size();
			int count = 0;
			//liste des elements qui vont servir à calculer les axes
			list<int> list_for_axes;
			list_for_axes = cluster_neighbors(cluster.second, cluster.first);
			//calculer les axes et retirer les éléments traitées du cluster
			for (int cell : list_for_axes) {
				Axes.push_back(computeAxe(cluster.first, cell));
				cluster.second.remove(cell);
			}
			while (cluster.second.size() != 0 && count <= initial_size) {
				list<int> list_traitee = list_for_axes;
				for (int cell_traitee : list_traitee) {
					list_for_axes = cluster_neighbors(cluster.second, cell_traitee);
					//calculer les axes et retirer les éléments traitées du cluster
					for (int cell : list_for_axes) {
						Axes.push_back(computeAxe(cell_traitee, cell));
						cluster.second.remove(cell);
					}
				}
				count++;
			}
			drawCube1(cluster.first, Axes);
		}
	}
	catch (exception e) {
		cout << e.what();
	}cout << endl;

	//dim2
	cout << "DIM2" << endl;
	listObj = ctrl->getFormes(Dim::d2);//liste de dim0
	for (DGVF::cluster clust : g->at(2)) {//tous clusters dim2
		glNewList(cptr, GL_COMPILE);	//créer nouvelle liste
		listObj->push_back(cptr);		//conserver identificateur
		drawCube2(clust.second);//calculer la liste
		glEndList();//fin definition liste
		cptr++;
	}

	//dim3
	cout << "DIM3" << endl;
	listObj = ctrl->getFormes(Dim::d3);//liste de dim0
	for (DGVF::cluster clust : g->at(3)) {//tous clusters dim3
		glNewList(cptr, GL_COMPILE);	//créer nouvelle liste
		listObj->push_back(cptr);		//conserver identificateur
		drawCube3(clust.second);//calculer la liste
		glEndList();//fin definition liste
		cptr++;
	}
	//finalisations
	ctrl->computeCenter(
		0, ccTraite->getSize(Axe::x)*dist,
		0, ccTraite->getSize(Axe::y)*dist,
		0, ccTraite->getSize(Axe::z)*dist);
}

void Modeleur::drawCube0(obj::Vertex center) {
	glBegin(GL_QUAD_STRIP);
	glVertex3f(center.x + rayon, center.y - rayon, center.z + rayon);//les côtés
	glVertex3f(center.x + rayon, center.y - rayon, center.z - rayon);
	glVertex3f(center.x + rayon, center.y + rayon, center.z + rayon);
	glVertex3f(center.x + rayon, center.y + rayon, center.z - rayon);
	glVertex3f(center.x - rayon, center.y + rayon, center.z + rayon);
	glVertex3f(center.x - rayon, center.y + rayon, center.z - rayon);
	glVertex3f(center.x - rayon, center.y - rayon, center.z + rayon);
	glVertex3f(center.x - rayon, center.y - rayon, center.z - rayon);
	glVertex3f(center.x + rayon, center.y - rayon, center.z + rayon);
	glVertex3f(center.x + rayon, center.y - rayon, center.z - rayon);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(center.x + rayon, center.y - rayon, center.z + rayon);//le dessus
	glVertex3f(center.x + rayon, center.y + rayon, center.z + rayon);
	glVertex3f(center.x - rayon, center.y + rayon, center.z + rayon);
	glVertex3f(center.x - rayon, center.y - rayon, center.z + rayon);
	glVertex3f(center.x - rayon, center.y - rayon, center.z - rayon);//le dessous
	glVertex3f(center.x - rayon, center.y + rayon, center.z - rayon);
	glVertex3f(center.x + rayon, center.y + rayon, center.z - rayon);
	glVertex3f(center.x + rayon, center.y - rayon, center.z - rayon);
	glEnd();
}
void Modeleur::drawCube1(int pos, std::vector<dir> Axes) {
	coord center = ccTraite->pos2coord(pos);
	Vertex center2 = coord2Vert(center);
	
	vector<Vertex> positions = computeCarre(center2, Axes.at(0));
	drawCarre(positions);

	//positions des 4 points de départ
	vector<Vertex> positions_2 = positions;
	//positions des 4 points après translation
	vector<Vertex> positions_tmp;
	positions_tmp.resize(4);
	//initialisation
	for (unsigned int i = 0; i < positions_tmp.size(); i++)
		positions_tmp.at(i).translation(Axes.at(0), dist - rayon);

	//permet de compenser la distance en cas de coude 
	vector<int> compense;
	compense.resize(4);
	for (unsigned int i = 0; i < 4; i++)
		compense.at(i) = 0;

	// desinner tous les contours autour du chemin sauf le dernier
	for (unsigned int i = 0; i < Axes.size() - 1; i++) {
		bool isLine = false;
		//ne pas déssiner de vertices tant qu'on a la même direction mais faire la translation suivant l'axe 
		while (Axes.at(i).first == Axes.at(i + 1).first && i < Axes.size() - 1) {
			for (unsigned int j = 0; j < positions_tmp.size(); j++) {
				positions_tmp.at(j).translation(Axes.at(i), dist - rayon);
			}
			i++;
			isLine = true;
		}
		// si on a eu une line droite juste avant la déssiner
		if (isLine) {
			//déssiner le contour
			cout << &positions_2 << " " << &positions_tmp << endl;
			drawContour(positions_2, positions_tmp);
			//mettre à jour les sommets de départ
			positions_2 = positions_tmp;
		}

		if (i < Axes.size() - 1) {
			//redéfinir la position de certain des nouveaux sommets en fonction de l'axe
			unsigned int sens;
			sens = (Axes.at(i).second) ? 1 : 0; // 1 si x sens positif, 0 sinon
			if (Axes.at(i).first == 0) {
				if (Axes.at(i + 1).first == 1 && Axes.at(i + 1).second) {
					positions_tmp.at(0).translation(Axes.at(i), sens*rayon);
					positions_tmp.at(1).translation(Axes.at(i), sens*rayon);
					positions_tmp.at(2).translation(Axes.at(i), (1 - sens)*rayon);
					positions_tmp.at(3).translation(Axes.at(i), (1 - sens)*rayon);
					compense.at(0) = compense.at(1) = sens;
					compense.at(2) = compense.at(3) = (1 - sens);
				}
				else if (Axes.at(i + 1).first == 1 && !Axes.at(i + 1).second) {
					positions_tmp.at(0).translation(Axes.at(i), (1 - sens)*rayon);
					positions_tmp.at(1).translation(Axes.at(i), (1 - sens)*rayon);
					positions_tmp.at(2).translation(Axes.at(i), sens*rayon);
					positions_tmp.at(3).translation(Axes.at(i), sens*rayon);
					compense.at(2) = compense.at(3) = sens;
					compense.at(0) = compense.at(1) = (1 - sens);
				}
				else if (Axes.at(i + 1).first == 2 && Axes.at(i + 1).second) {
					positions_tmp.at(1).translation(Axes.at(i), rayon);
					positions_tmp.at(2).translation(Axes.at(i), (1 - sens)*rayon);
					compense.at(1) = compense.at(2) = 1;
					compense.at(0) = compense.at(3) = 0;
				}
				else if (Axes.at(i + 1).first == 2 && !Axes.at(i + 1).second) {
					positions_tmp.at(0).translation(Axes.at(i), rayon);
					positions_tmp.at(3).translation(Axes.at(i), rayon);
					compense.at(0) = compense.at(3) = 1;
					compense.at(1) = compense.at(2) = 0;
				}
			}
			else if (Axes.at(i).first == 1) {
				if (Axes.at(i + 1).first == 0 && Axes.at(i + 1).second) {
					positions_tmp.at(0).translation(Axes.at(i), (1 - sens)*rayon);
					positions_tmp.at(1).translation(Axes.at(i), (1 - sens)*rayon);
					positions_tmp.at(2).translation(Axes.at(i), sens*rayon);
					positions_tmp.at(3).translation(Axes.at(i), sens*rayon);
					compense.at(2) = compense.at(3) = sens;
					compense.at(0) = compense.at(1) = (1 - sens);
				}
				else if (Axes.at(i + 1).first == 0 && !Axes.at(i + 1).second) {
					positions_tmp.at(0).translation(Axes.at(i), sens*rayon);
					positions_tmp.at(1).translation(Axes.at(i), sens*rayon);
					positions_tmp.at(2).translation(Axes.at(i), (1 - sens)*rayon);
					positions_tmp.at(3).translation(Axes.at(i), (1 - sens)*rayon);
					compense.at(0) = compense.at(1) = sens;
					compense.at(2) = compense.at(3) = (1 - sens);
				}
				else if (Axes.at(i + 1).first == 2 && Axes.at(i + 1).second) {
					positions_tmp.at(1).translation(Axes.at(i), rayon);
					positions_tmp.at(2).translation(Axes.at(i), rayon);
					compense.at(1) = compense.at(2) = 1;
					compense.at(0) = compense.at(3) = 0;
				}
				else if (Axes.at(i + 1).first == 2 && !Axes.at(i + 1).second) {
					positions_tmp.at(0).translation(Axes.at(i), rayon);
					positions_tmp.at(3).translation(Axes.at(i), rayon);
					compense.at(0) = compense.at(3) = 1;
					compense.at(1) = compense.at(2) = 0;
				}
			}
			else if (Axes.at(i).first == 2) {
				if (Axes.at(i + 1).first == 0 && Axes.at(i + 1).second) {
					positions_tmp.at(0).translation(Axes.at(i), rayon);
					positions_tmp.at(3).translation(Axes.at(i), rayon);
					compense.at(0) = compense.at(3) = 1;
					compense.at(1) = compense.at(2) = 0;
				}
				else if (Axes.at(i + 1).first == 0 && !Axes.at(i + 1).second) {
					positions_tmp.at(1).translation(Axes.at(i), rayon);
					positions_tmp.at(2).translation(Axes.at(i), rayon);
					compense.at(1) = compense.at(2) = 1;
					compense.at(0) = compense.at(3) = 0;
				}
				else if (Axes.at(i + 1).first == 1 && Axes.at(i + 1).second) {
					positions_tmp.at(0).translation(Axes.at(i), rayon);
					positions_tmp.at(1).translation(Axes.at(i), rayon);
					compense.at(0) = compense.at(1) = 1;
					compense.at(2) = compense.at(3) = 0;
				}
				else if (Axes.at(i + 1).first == 1 && !Axes.at(i + 1).second) {
					positions_tmp.at(2).translation(Axes.at(i), rayon);
					positions_tmp.at(3).translation(Axes.at(i), rayon);
					compense.at(2) = compense.at(3) = 1;
					compense.at(0) = compense.at(1) = 0;
				}
			}
			cout << "azr" << endl;

			//déssiner le contour
			drawContour(positions_2, positions_tmp);
			//mettre à jour les sommets de départ
			positions_2 = positions_tmp;
			//Faire la compensation
			for (unsigned int j = 0; j < 4; i++) {
				int direction = (Axes.at(i + 1).second) ? 1 : -1;
				positions_tmp.at(j).x += compense.at(j) * direction*rayon;
				positions_tmp.at(j).y += compense.at(j) * direction*rayon;
				positions_tmp.at(j).z += compense.at(j) * direction*rayon;
				compense.at(j) = 0;
			}
			cout << "azr" << endl;
		}
	}
	cout << "azr" << endl;
	//Désiner le dernier contour
	for (unsigned int i = 0; i < positions_tmp.size(); i++) {
		int direction = (Axes.at(Axes.size() - 1).second) ? 1 : -1;
		positions_tmp.at(i)[Axes.at(Axes.size() - 1).first] = positions_2.at(i)[Axes.at(Axes.size() - 1).first] + direction*(dist - rayon);
	}
	drawContour(positions_2, positions_tmp);
	//mettre à jour les positions des derniers sommet
	positions_2 = positions_tmp;
	cout << "azr" << endl;

	//dessiner la dernière face
	drawCarre(positions_2);
	cout << "machinC" << endl;

}
void Modeleur::drawCube2(DGVF::cellList cluster) {
	for (int cell : cluster) {
		drawCube0(coord2Vert(ccTraite->pos2coord(cell)));
	}
}
void Modeleur::drawCube3(DGVF::cellList cluster) {
	for (int cell : cluster) {
		drawCube0(coord2Vert(ccTraite->pos2coord(cell)));
	}
}
//*************************************Dessin de CC spécifiques *******************************************
void Modeleur::drawContour(vector<Vertex> positions_2, vector<Vertex> positions_tmp) {
	glBegin(GL_QUAD_STRIP);
	int pos = 0;
	for (int i = 0; i <= 4; i++) {
		cout << positions_2.at(pos).x << endl;
		glVertex3f(positions_2.at(pos).x, positions_2.at(pos).y, positions_2.at(pos).z);
		glVertex3f(positions_tmp.at(pos).x, positions_tmp.at(pos).y, positions_tmp.at(pos).z);
		pos++;
		pos = pos % 4;
	}
	glEnd();
}
vector <Vertex> Modeleur::computeCarre(Vertex center, dir axe) {
	vector <Vertex> positions;
	positions.resize(4);
	float sens = (axe.second) ? 1.0f : -1.0f;
	for (unsigned int j = 0; j < 3; j++) {
			for (unsigned int i = 0; i < 4; i++) {

				positions.at(i)[axe.first] = center[axe.first] - sens * (rayon + dist / 2);
			}
			unsigned int j1 = (j + 1) % 3;
			unsigned int j2 = (j + 2) % 3;
			positions.at(0)[j1] = center[j1] - sens * rayon / 2;
			positions.at(0)[j2] = center[j2] + sens * rayon / 2;
			positions.at(1)[j1] = center[j1] + rayon / 2;
			positions.at(1)[j2] = center[j2] + rayon / 2;
			positions.at(2)[j1] = center[j1] + sens * rayon / 2;
			positions.at(2)[j2] = center[j2] - sens * rayon / 2;
			positions.at(3)[j1] = center[j1] - rayon / 2;
			positions.at(3)[j2] = center[j2] - rayon / 2;
	}
	return positions;
}
void Modeleur::drawCarre(vector<Vertex> pos) {
	glBegin(GL_QUADS);
	for (Vertex v : pos) {
		glVertex3f(v[0], v[1], v[2]);
	}
	glEnd();
}
void Modeleur::drawFace(const obj::face &fa) {
	Vertex v;
	glBegin(GL_POLYGON);
	for (int i : fa) {
		v = objAffiche->getVertex(i);
		glVertex3f(v.x, v.y, v.z);
	}
	glEnd();
}
//****************************************Gestion Obj2 ************************************************
void Modeleur::setObj(obj::Obj2::Ptr obj) {
	objAffiche = obj;
}
void Modeleur::setComplexeCubique(ComplexeCubique::Ptr cc) {
	ccTraite = cc;
}
void Modeleur::initiateObjs() {
	ctrl->resetLists();
	Object obj;
	std::vector<GLuint> *listObj;
	cout << "affichage Obj..." << endl;
	int cptr = 1;//compte toutes les listes. Il ne doit pas y avoir de doublons
	for (Dim a : {Dim::d0,Dim::d1,Dim::d2,Dim::d3}) {
		//pour chaque dimension
		listObj = ctrl->getFormes(a);
		for (int i = 0; i < objAffiche->nbrObjects(); i++) {//parcourir les objets
			obj = objAffiche->getObject(i);
			if (obj.getDimension() == a) {//si objet de la dimension a
				glNewList(cptr, GL_COMPILE);	//créer nouvelle liste
				listObj->push_back(cptr);		//conserver identificateur
				for (obj::face f : obj.getFaces()) {//tracer l'objet
					drawFace(f);
				}
				glEndList();
				ctrl->setNameList(cptr,obj.getName());
				cptr++;
			}
		}
	}
	cout << "affichage Obj Done" << endl;
	computeCenter();
}
//****************************************Gestion Diverse **********************************************
void Modeleur::computeCenter() {
	Vertex v = objAffiche->getVertex(1);;
	float xmin = v.x, xmax = v.x;
	float ymin = v.y, ymax = v.y;
	float zmin = v.z, zmax = v.z;
	for (int i = 2; i <= objAffiche->nbrVertices(); i++) {
		v = objAffiche->getVertex(i);
		xmin = std::min(xmin, v.x);
		xmax = std::max(xmax, v.x);
		ymin = std::min(ymin, v.y);
		ymax = std::max(ymax, v.y);
		zmin = std::min(zmin, v.z);
		zmax = std::max(zmax, v.z);
	}
	ctrl->computeCenter(xmin, xmax, ymin, ymax, zmin, zmax);
}
dir Modeleur::computeAxe(int first_pos, int next_pos) {
	//conversion position ->coordonnées
	dir axe;
	coord first_coord = ccTraite->pos2coord(first_pos);
	coord next_coord = ccTraite->pos2coord(next_pos);
	if (first_coord.x != next_coord.x) {
		axe.first  = Axe::x;
		axe.second = (first_coord.x < next_coord.x) ? true : false;
	}
	else if (first_coord.y != next_coord.y) {
		axe.first = Axe::y;
		axe.second = (first_coord.y < next_coord.y) ? true : false;
	}
	else if (first_coord.z != next_coord.z) {
		axe.first = Axe::z;
		axe.second = (first_coord.z < next_coord.z) ? true : false;
	}
	else 
		throw CtrlError(to_string(1) + "deux éléments du cluster sont en réalité les mêmes");
	return axe;
}
void Modeleur::setDistances(float rayon, float longueur, float separation) {
	this->rayon = rayon;
	this->longueur = longueur;
	this->separation = separation;
	dist = 2*rayon + 2 * separation + longueur;
}
Vertex Modeleur::coord2Vert(coord co) {
	Vertex v;
	v.x = co.x*dist;
	v.y = co.y*dist;
	v.z = co.z * dist;
	return v;
}
list<int>  Modeleur::cluster_neighbors(list<int> cluster, int position)
{
	list<int> incluster;
	list<int> voisins = ccTraite->neighbors(position);
	for (int voisin : voisins) {
		for (int cell : cluster)
			if (voisin == cell) {
				incluster.push_back(voisin);
			}
	}
	return incluster;
}


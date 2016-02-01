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
//****************************************Gestion dessin ************************************************
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
void Modeleur::drawContour(vector<Vertex> positions_2, vector<Vertex> positions_tmp) {
	glBegin(GL_QUAD_STRIP);
	glVertex3f(positions_2.at(2).x, positions_2.at(2).y, positions_2.at(2).z);
	glVertex3f(positions_tmp.at(2).x, positions_tmp.at(2).y, positions_tmp.at(2).z);
	glVertex3f(positions_2.at(1).x, positions_2.at(1).y, positions_2.at(1).z);
	glVertex3f(positions_tmp.at(1).x, positions_tmp.at(1).y, positions_tmp.at(1).z);
	glVertex3f(positions_2.at(4).x, positions_2.at(4).y, positions_2.at(4).z);
	glVertex3f(positions_tmp.at(4).x, positions_tmp.at(4).y, positions_tmp.at(4).z);
	glVertex3f(positions_2.at(3).x, positions_2.at(3).y, positions_2.at(3).z);
	glVertex3f(positions_tmp.at(3).x, positions_tmp.at(3).y, positions_tmp.at(3).z);
	glVertex3f(positions_2.at(2).x, positions_2.at(2).y, positions_2.at(2).z);
	glVertex3f(positions_tmp.at(2).x, positions_tmp.at(2).y, positions_tmp.at(2).z);
	glEnd();
}
vector<Vertex> Modeleur::computePositions(vector<Vertex> positions, std::vector<coord> Axes) {

	//positions des 4 points de départ
	vector<Vertex> positions_2 = positions;
	//positions des 4 points après translation
	vector<Vertex> positions_tmp;
	positions_tmp.resize(4);
	//initialisation
	for (unsigned int i = 0; i < positions_tmp.size(); i++)
		positions_tmp.at(i) = positions_2.at(i) + (dist-rayon)*Axes.at(0);
	
	//permet de compenser la distance en cas de coude 
	vector<int> compense;
	compense.resize(4);
	for (unsigned int i = 0; i < 4; i++)
		compense.at(i) = 0;

	// desinner tous les contours autour du chemin sauf le dernier
	for (unsigned int i = 0; i < Axes.size()-1; i++) {
		bool isLine = false;
		//ne pas déssiner de vertices tant qu'on a la même direction mais faire la translation suivant l'axe 
		while (Axes.at(i) == Axes.at(i + 1) && i < Axes.size()-1) {
			for (unsigned int j = 0; j < positions_tmp.size(); j++) {
				positions_tmp.at(j) = positions_tmp.at(j) + (dist-rayon)*Axes.at(i);
			}
			i++;
			isLine = true;
		}
		// si on a eu une line droite juste avant la déssiner
		if (isLine) {
			//déssiner le contour
			drawContour(positions_2, positions_tmp);
			//mettre à jour les sommets de départ
			positions_2 = positions_tmp;
		}

		if (i < Axes.size()-1){
			//redéfinir la position de certain des nouveaux sommets en fonction de l'axe
			unsigned int sens;
			if (Axes.at(i).x != 0) {
				sens = (1 + Axes.at(i).x) / 2; // 1 si x sens positif, 0 sinon
				if (Axes.at(i + 1).y = 1) {
					positions_tmp.at(0) = positions_tmp.at(0) + sens*rayon*Axes.at(i);
					positions_tmp.at(1) = positions_tmp.at(1) + sens*rayon*Axes.at(i);
					positions_tmp.at(2) = positions_tmp.at(2) + (1 - sens)*rayon*Axes.at(i);
					positions_tmp.at(3) = positions_tmp.at(3) + (1 - sens)*rayon*Axes.at(i);
					compense.at(0) = compense.at(1) = sens;
					compense.at(2) = compense.at(3) = (1 -sens);
				}
				else if (Axes.at(i + 1).y = -1) {
					positions_tmp.at(0) = positions_tmp.at(0) + (1 - sens)*rayon*Axes.at(i);
					positions_tmp.at(1) = positions_tmp.at(1) + (1 - sens)*rayon*Axes.at(i);
					positions_tmp.at(2) = positions_tmp.at(2) + sens*rayon*Axes.at(i);
					positions_tmp.at(3) = positions_tmp.at(3) + sens*rayon*Axes.at(i);
					compense.at(2) = compense.at(3) = sens;
					compense.at(0) = compense.at(1) = (1 - sens);
				}
				else if (Axes.at(i + 1).z = 1) {
					positions_tmp.at(1) = positions_tmp.at(1) + rayon*Axes.at(i);
					positions_tmp.at(2) = positions_tmp.at(2) + rayon*Axes.at(i);
					compense.at(1) = compense.at(2) = 1;
					compense.at(0) = compense.at(3) = 0;
				}
				else if (Axes.at(i + 1).z = -1) {
					positions_tmp.at(0) = positions_tmp.at(0) + rayon*Axes.at(i);
					positions_tmp.at(3) = positions_tmp.at(3) + rayon*Axes.at(i);
					compense.at(0) = compense.at(3) = 1;
					compense.at(1) = compense.at(2) = 0;
				}
			}
			else if (Axes.at(i).y != 0) {
				sens = (1 + Axes.at(i).y) / 2; // 1 si y sens positif, 0 sinon
				if (Axes.at(i + 1).x = 1) {
					positions_tmp.at(0) = positions_tmp.at(0) + (1 - sens)*rayon*Axes.at(i);
					positions_tmp.at(1) = positions_tmp.at(1) + (1 - sens)*rayon*Axes.at(i);
					positions_tmp.at(2) = positions_tmp.at(2) + sens*rayon*Axes.at(i);
					positions_tmp.at(3) = positions_tmp.at(3) + sens*rayon*Axes.at(i);
					compense.at(2) = compense.at(3) = sens;
					compense.at(0) = compense.at(1) = (1 - sens);
				}
				else if (Axes.at(i + 1).x = -1) {
					positions_tmp.at(0) = positions_tmp.at(0) + sens*rayon*Axes.at(i);
					positions_tmp.at(1) = positions_tmp.at(1) + sens*rayon*Axes.at(i);
					positions_tmp.at(2) = positions_tmp.at(2) + (1 - sens)*rayon*Axes.at(i);
					positions_tmp.at(3) = positions_tmp.at(3) + (1 - sens)*rayon*Axes.at(i);
					compense.at(0) = compense.at(1) = sens;
					compense.at(2) = compense.at(3) = (1 - sens);
				}
				else if (Axes.at(i + 1).z = 1) {
					positions_tmp.at(1) = positions_tmp.at(1) + rayon*Axes.at(i);
					positions_tmp.at(2) = positions_tmp.at(2) + rayon*Axes.at(i);
					compense.at(1) = compense.at(2) = 1;
					compense.at(0) = compense.at(3) = 0;
				}
				else if (Axes.at(i + 1).z = -1) {
					positions_tmp.at(0) = positions_tmp.at(0) + rayon*Axes.at(i);
					positions_tmp.at(3) = positions_tmp.at(3) + rayon*Axes.at(i);
					compense.at(0) = compense.at(3) = 1;
					compense.at(1) = compense.at(2) = 0;
				}
			}
			else if (Axes.at(i).z != 0) {
				if (Axes.at(i + 1).x = 1) {
					positions_tmp.at(0) = positions_tmp.at(0) + rayon*Axes.at(i);
					positions_tmp.at(3) = positions_tmp.at(3) + rayon*Axes.at(i);
					compense.at(0) = compense.at(3) = 1;
					compense.at(1) = compense.at(2) = 0;
				}
				else if (Axes.at(i + 1).x = -1) {
					positions_tmp.at(1) = positions_tmp.at(1) + rayon*Axes.at(i);
					positions_tmp.at(2) = positions_tmp.at(2) + rayon*Axes.at(i);
					compense.at(1) = compense.at(2) = 1;
					compense.at(0) = compense.at(3) = 0;
				}
				else if (Axes.at(i + 1).y = 1) {
					positions_tmp.at(0) = positions_tmp.at(0) + rayon*Axes.at(i);
					positions_tmp.at(1) = positions_tmp.at(1) + rayon*Axes.at(i);
					compense.at(0) = compense.at(1) = 1;
					compense.at(2) = compense.at(3) = 0;
				}
				else if (Axes.at(i + 1).y = -1) {
					positions_tmp.at(2) = positions_tmp.at(2) + rayon*Axes.at(i);
					positions_tmp.at(3) = positions_tmp.at(3) + rayon*Axes.at(i);
					compense.at(2) = compense.at(3) = 1;
					compense.at(0) = compense.at(1) = 0;
				}
			}

			//déssiner le contour
			drawContour(positions_2, positions_tmp);
			//mettre à jour les sommets de départ
			positions_2 = positions_tmp;
			//Faire la compensation
			for (unsigned int j = 0; j < 4; i++) {
				positions_tmp.at(j).x += compense.at(j) * Axes.at(i+1).x*rayon;
				positions_tmp.at(j).y += compense.at(j) * Axes.at(i+1).y*rayon;
				positions_tmp.at(j).z += compense.at(j) * Axes.at(i+1).z*rayon;
				compense.at(i) = 0;
			}
		}
	}
	//Désiner le dernier contour
	for (unsigned int i = 0; i < positions_tmp.size(); i++)
		positions_tmp.at(i) = positions_2.at(i) + (dist-rayon)*Axes.at(Axes.size()-1);
	
	drawContour(positions_2, positions_tmp);
	//mettre à jour les positions des derniers sommet
	positions_2 = positions_tmp;
	
	return positions_2;
}
void Modeleur::drawCube1(int pos, std::vector<coord> Axes) {
	coord center = ccTraite->pos2coord(pos);
	vector <Vertex> positions;
	positions.resize(4);
	Vertex center2 = coord2Vert(center);

	if (Axes.at(0).x != 0) {
		positions.at(0).x = center2.x - Axes.at(0).x*(rayon+dist/2);
		positions.at(0).y = center2.y - rayon / 2;
		positions.at(0).z = center2.z + rayon / 2;

		positions.at(1).x = center2.x - Axes.at(0).x*(rayon + dist / 2);
		positions.at(1).y = center2.y - rayon / 2;
		positions.at(1).z = center2.z - rayon / 2;

		positions.at(2).x = center2.x - Axes.at(0).x*(rayon + dist / 2);
		positions.at(2).y = center2.y + rayon / 2;
		positions.at(2).z = center2.z + rayon / 2;

		positions.at(3).x = center2.x - Axes.at(0).x*(rayon + dist / 2);
		positions.at(3).y = center2.y + rayon / 2;
		positions.at(3).z = center2.z - rayon / 2;
	}
	else if (Axes.at(0).y != 0) {
		positions.at(0).x = center2.x + rayon / 2;
		positions.at(0).y = center2.y - Axes.at(0).y*(rayon + dist / 2);
		positions.at(0).z = center2.z + rayon / 2;

		positions.at(1).x = center2.x + rayon / 2;
		positions.at(1).y = center2.y - Axes.at(0).y*(rayon + dist / 2);
		positions.at(1).z = center2.z - rayon / 2;

		positions.at(2).x = center2.x - rayon / 2;
		positions.at(2).y = center2.y - Axes.at(0).y*(rayon + dist / 2);
		positions.at(2).z = center2.z + rayon / 2;

		positions.at(3).x = center2.x - rayon / 2;
		positions.at(3).y = center2.y - Axes.at(0).y*(rayon + dist / 2);
		positions.at(3).z = center2.z - rayon / 2;
	}
	else if (Axes.at(0).z != 0) {
		positions.at(0).x = center2.x + rayon / 2;
		positions.at(0).y = center2.y - rayon / 2;
		positions.at(0).z = center2.z - Axes.at(0).z*(rayon + dist / 2);

		positions.at(1).x = center2.x + rayon / 2;
		positions.at(1).y = center2.y + rayon / 2;
		positions.at(1).z = center2.z - Axes.at(0).z*(rayon + dist / 2);

		positions.at(2).x = center2.x - rayon / 2;
		positions.at(2).y = center2.y - rayon / 2;
		positions.at(2).z = center2.z - Axes.at(0).z*(rayon + dist / 2);

		positions.at(3).x = center2.x - rayon / 2;
		positions.at(3).y = center2.y + rayon / 2;
		positions.at(3).z = center2.z - Axes.at(0).z*(rayon + dist / 2);
	}
	drawFace(positions);
	vector<Vertex> lastPos = computePositions(positions, Axes);
	drawFace(lastPos);

}
void Modeleur::drawCube2(std::vector<obj::coord> positions, std::vector<obj::coord> Axes)
{
}
void Modeleur::drawFace(vector<Vertex> positions ) {
	glBegin(GL_QUAD_STRIP);
	for (Vertex pos : positions) {
		glVertex3f(pos.x, pos.y, pos.z);
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
void Modeleur::setPgm(PGM3D::Ptr pgm) {
	pgmTraite = pgm;
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
	vector<coord> Axes;
	//dim1
	cout << "DIM1" << endl;
	/*for (DGVF::cluster clust : g->at(1)) {//tous clusters dim 1
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
			}
			//calculer les axes et retirer les éléments traitées du cluster
			for (int cell : list_for_axes) {
				Axes.push_back(computeAxe(cluster.first, cell));
				cluster.second.remove(cell);
			}
			count++;
		}
		drawCube1(cluster.first, Axes);
		//cout << endl;
	}*/
	//dim2
	cout << "DIM2" << endl;
	for (DGVF::cluster clust : g->at(2)) {//tous clusters dim2
		cout << "\t" << clust.first << " : ";
		for (int cell : clust.second) {//afficher dim des cellules
			cout << cell << " ";
		}cout << endl;
	}

	//dim3
	cout << "DIM3" << endl;
	for (DGVF::cluster clust : g->at(3)) {//tous clusters dim3
		cout << "\t" << clust.first << " : ";
		for (int cell : clust.second) {//afficher dim des cellules
			cout << cell << " ";
		}cout << endl;
	}

	ctrl->computeCenter(
		0, ccTraite->getSize(Axe::x)*dist,
		0, ccTraite->getSize(Axe::y)*dist,
		0, ccTraite->getSize(Axe::z)*dist);
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

//****************************************Gestion centre ************************************************
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
coord Modeleur::computeAxe(int first_pos, int next_pos) {
	//conversion position ->coordonnées
	coord axe;
	coord first_coord = ccTraite->pos2coord(first_pos);
	coord next_coord = ccTraite->pos2coord(next_pos);
	if (first_coord.x != next_coord.x) {
		if (first_coord.x < next_coord.x) {
			axe.x = 1;
			axe.x = 0;
			axe.x = 0;
		}
		else if (first_coord.x > next_coord.x) {
			axe.x = -1;
			axe.x = 0;
			axe.x = 0;
		}
	}
	else if (first_coord.y != next_coord.y) {
		if (first_coord.y < next_coord.y) {
			axe.x = 0;
			axe.x = 1;
			axe.x = 0;
		}
		else if (first_coord.y > next_coord.y) {
			axe.x = 0;
			axe.x = -1;
			axe.x = 0;
		}
	}
	else if (first_coord.z != next_coord.z) {
		if (first_coord.z < next_coord.z) {
			axe.x = 0;
			axe.x = 0;
			axe.x = -1;
		}
		else if (first_coord.z > next_coord.z) {
			axe.x = 0;
			axe.x = 0;
			axe.x = -1;
		}
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

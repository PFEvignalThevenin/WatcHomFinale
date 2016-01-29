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
void Modeleur::drawContour(vector<coord> positions_2, vector<coord> positions_tmp) {
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
vector<coord> Modeleur::computePositions(vector<coord> positions, std::vector<coord> Axes) {

	//positions des 4 points de départ
	vector<coord> positions_2 = positions;
	//positions des 4 points après translation
	vector<coord> positions_tmp;
	positions_tmp.resize(4);
	//initialisation
	for (unsigned int i = 0; i < positions_tmp.size(); i++)
		positions_tmp.at(i) = positions_2.at(i) + dist*Axes.at(0);
	
	//permet de compenser la distance en cas de coude 
	vector<int> compense;
	compense.resize(4);
	for (unsigned int i = 0; i < 4; i++)
		compense.at(i) = 0;

	// desinner tous les contours autour du chemin sauf le dernier
	for (int i = 0; i < Axes.size()-1; i++) {
		bool isLine = false;
		//ne pas déssiner de vertices tant qu'on a la même direction mais faire la translation suivant l'axe 
		while (Axes.at(i) == Axes.at(i + 1) && i < Axes.size()-1) {
			for (unsigned int j = 0; j < positions_tmp.size(); j++) {
				positions_tmp.at(j) = positions_tmp.at(j) + dist*Axes.at(i);
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
		positions_tmp.at(i) = positions_2.at(i) + dist*Axes.at(Axes.size()-1);
	
	drawContour(positions_2, positions_tmp);
	//mettre à jour les positions des derniers sommet
	positions_2 = positions_tmp;
	
	return positions_2;
}
void Modeleur::drawCube1(vector<coord> positions, std::vector<coord> Axes) {

	drawFace(positions);
	vector<coord> lastPos = computePositions(positions, Axes);
	drawFace(lastPos);

}
void Modeleur::drawFace(vector<coord> positions) {
	glBegin(GL_QUADS);
	for (coord pos : positions) {
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
	cout << "construction" << endl;
	int cptr = 1;//compte toutes les listes. Il ne doit pas y avoir de doublons
	for (Dim a : {Dim::d0,Dim::d1,Dim::d2,Dim::d3}) {
		//pour chaque dimension
		cout << "DIM : " << to_string(a) << endl;
		listObj = ctrl->getFormes(a);
		for (int i = 0; i < objAffiche->nbrObjects(); i++) {//parcourir les objets
			obj = objAffiche->getObject(i);
			if (obj.getDimension() == a) {//si objet de la dimension a
				glNewList(cptr, GL_COMPILE);	//créer nouvelle liste
				listObj->push_back(cptr);		//conserver identificateur
				for (obj::face f : obj.getFaces()) {//tracer l'objet
					drawFace(f);
				}
				cout << to_string(cptr) << endl;
				glEndList();
				cptr++;
			}
		}
	}
	computeCenter();
}
void Modeleur::initiatePgm() {
	ctrl->resetLists();
	std::vector<GLuint> *listObj = ctrl->getFormes(Dim::d0);
	cout << "construction PGM..." << endl;
	glNewList(1, GL_COMPILE);	//créer nouvelle liste
	listObj->push_back(1);		//conserver identificateur
	drawPgm(*pgmTraite);
	glEndList();
	cout << "construction PGM Done" << endl;
	ctrl->computeCenter(
		0, pgmTraite->getSize(Axe::x)*dist,
		0, pgmTraite->getSize(Axe::y)*dist,
		0, pgmTraite->getSize(Axe::z)*dist);
}
void Modeleur::initiateComplexeCubique(shared_ptr<vector<map<int, int>>> g_inv) {
	ctrl->resetLists();
	//traiter les clusters par dimension
	cout << "TODO : \tfaire l'affichage des clusters dans Modeleur::initiateComplexeCubique" << endl;
	cout << "\tfait :  Dim0" << endl;
	cout << "\ta faire :  Dim 1,2,3" << endl;
	int cptr = 1;//compte toutes les listes. Il ne doit pas y avoir de doublons
	std::vector<GLuint> *listObj;//variable des indices de liste d'une dimension
	//dim0
	listObj = ctrl->getFormes(Dim::d0);//liste de dim0
	for (DGVF::cellBound bind : g_inv->at(0)) {
		glNewList(cptr, GL_COMPILE);	//créer nouvelle liste
		listObj->push_back(cptr);		//conserver identificateur
		drawCube0(coord2Vert(ccTraite->pos2coord(bind.first)));//dessin
		glEndList();//fin definition liste
		cptr++;
	}
	//dim1

	//dim2

	//dim3

	ctrl->computeCenter(
		0, ccTraite->getSize(Axe::x)*dist,
		0, ccTraite->getSize(Axe::y)*dist,
		0, ccTraite->getSize(Axe::z)*dist);
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

void Modeleur::setDistances(float rayon, float longueur, float separation) {
	this->rayon = rayon;
	this->longueur = longueur;
	this->separation = separation;
	dist = rayon + 2 * separation + longueur;
}
Vertex Modeleur::coord2Vert(coord co) {
	Vertex v;
	v.x = co.x*dist;
	v.y = co.y*dist;
	v.z = co.z * dist;
	return v;
}

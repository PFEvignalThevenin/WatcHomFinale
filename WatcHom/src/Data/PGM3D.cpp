#include "Data\PGM3D.hpp"
#include <iostream>

using namespace std;

PGM3D::PGM3D() : StructureCubique<char>(0,0,0)
{
}

PGM3D::~PGM3D()
{
}
void PGM3D::save(std::string const& path) const {
	ofstream file;
	file.open(path);

	//check ouverture
	if (!file.good()) {
		throw FileError(0,"Error in PGM3D save : \n\timpossible to open " + path + ".");
	}
	//nbr magique et dimensions
	file << "P2" << endl;
	for (int i = 0; i < 3; i++) {
		file << size[i]<<" ";
	}file << "255";//le 255 qui sert à rien
	//data
	for (int i = 0; i < nbrCubes(); i++) {
		if (i%size[Axe::x] == 0) {
			file << endl;
		}
		if (isEmpty(i)) {
			file << "0 ";
		}
		else {
			file << "1 ";
		}
	}
}
void PGM3D::load(std::string const& path) {
	ifstream file;
	string work;
	file.open(path.c_str(), ios::in);

	//check ouverture
	if (!file.good()){
		throw FileError(0,"Error in PGM3D load : \n\timpossible to create " + path + ".");
	}

	//check start with P2
	file >> work;
	if (work != "P2") {
		throw FileError(1,"Error in  PGM3D load : \n\tCan only load P2 format. was " + work);
	}
	//Récupérer les dimensions
	int p_size[3];//ordre du fichier : x,y,z
	for (int i = 0; i < 3; i++)
	{
		if (!file.eof()) {
			file >> p_size[i];
		} else { 
			throw FileError(2,"Error in  PGM3D load : \n\tcouldn't read the size."); 
		}
	}
	setDataSize(p_size);//initialiser le tableau des données

	//check 3eme ligne avec 255, mais en fait on s'en fout
	file >> work;

	//récupération des données
	int p_v;
	for (int i = 0; i < size[Axe::z]; i++) {
		for (int j = 0; j < size[Axe::y]; j++) {
			for (int k = 0; k < size[Axe::x]; k++) {
				if (!file.eof()) {
					file >> p_v;
				}
				else {
					throw FileError(3,"Error in  PGM3D load : \n\tcouldn't read the " + to_string(i) + "-th voxel.");
				}
				set(k,j,i, p_v);
			}
		}
	}
	//fin du traitement
	file.close();
}
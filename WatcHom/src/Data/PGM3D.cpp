#include "Data\PGM3D.hpp"
#include <iostream>

using namespace std;

PGM3D::PGM3D() : StructureCubique<char>(0,0,0)
{
}

PGM3D::~PGM3D()
{
}
//******************************************Sauvegarde******************************************************
void PGM3D::save(std::string const& path) const {
	ofstream file;
	file.open(path);

	//check ouverture
	if (!file.good()) {
		throw FileError(0,"Error in PGM3D save : \n\timpossible to open " + path + ".\n");
	}
	//nbr magique et dimensions
	file << "P2" << endl;
	for (int i = 0; i < 3; i++) {
		file << size[i]<<" ";
	}file << "255";//le 255 qui sert � rien
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
//******************************************Chargement******************************************************
void PGM3D::load(std::string const& path) {
	ifstream file;
	string work;
	file.open(path.c_str(), ios::in);

	//check ouverture
	if (!file.good()){
		throw FileError(0,"Error in PGM3D load : \n\timpossible to create " + path + ".\n");
	}

	//check start with P2
	file >> work;
	if (work != "P2") {
		throw FileError(1,"Error in  PGM3D load : \n\tCan only load P2 format. was " + work +"\n");
	}
	//R�cup�rer les dimensions
	int p_size[3];//ordre du fichier : x,y,z
	for (int i = 0; i < 3; i++)
	{
		if (!file.eof()) {
			file >> p_size[i];
		} else { 
			throw FileError(2,"Error in  PGM3D load : \n\tcouldn't read the size.\n"); 
		}
	}
	setDataSize(p_size);//initialiser le tableau des donn�es

	//check 3eme ligne avec 255, mais en fait on s'en fout
	file >> work;

	//r�cup�ration des donn�es
	int p_v;
	int cptr = 0;
	for (int i = 0; i < nbrCubes(); i++) {
		cptr++;
		if (!file.eof()) {
			file >> p_v;
		}
		else {
			throw FileError(3, "Error in  PGM3D load : \n\tcouldn't read the " + to_string(cptr) + "-th voxel." + to_string(p_v) + "\n");
		}
		set(i, (char)p_v);
	}
	//fin du traitement
	file.close();
}
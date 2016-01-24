#include "Interface\WindowOpenGL.hpp"

#include <Data\PGM3D.hpp>
#include <Data\ComplexeCubique.hpp>
#include <Data\Conversion.hpp>
#include "Engine\DGVF.hpp"
#include <ctime>

using namespace std;
using namespace sf;
using namespace sfg;
using namespace obj;
template<typename D>
void printCC(StructureCubique<D> &c) {
	cout << c.getSize(Axe::x) << " " << c.getSize(Axe::y) << " " << c.getSize(Axe::z) << endl;
	for (int i = 0; i < c.getSize(Axe::x); i++) {
		for (int j = 0; j < c.getSize(Axe::y); j++) {
			for (int z = 0; z < c.getSize(Axe::z); z++) {
				cout << c.get(i, j, z) << " ";
			}cout << endl;
		}cout << endl;
	}
}
void printDimCC(ComplexeCubique &c) {
	cout << c.getSize(Axe::x) << " " << c.getSize(Axe::y) << " " << c.getSize(Axe::z) << endl;
	for (int i = 0; i < c.getSize(Axe::x); i++) {
		for (int j = 0; j < c.getSize(Axe::y); j++) {
			for (int z = 0; z < c.getSize(Axe::z); z++) {
				cout << c.dim(i, j, z) << " ";
			}cout << endl;
		}cout << endl;
	}
}
void printBordsCC(ComplexeCubique &c, int pos) {
	cout << "dim : "<<c.dim(pos) << endl;
	for (int i : c.boundary(pos)) {
		cout << i << " dim : " << c.dim(i) << endl;
	}
}
void writeComplex(ComplexeCubique &cbc, string fileName, double r, double s)
{
	clock_t begin = clock();
	int vertices = 0; //numéro du vertice écrit ds le point obj
	vector<double> v(6);

	/* Create the file */
	ofstream file(fileName.c_str(), ios::out | ios::trunc);
	if (!file)
	{
		cout << "Error in write2Obj(" << fileName << "): impossible to create the output text file." << endl;
		return;
	}
	file << "# Obj file created by WatcHom (Aldo Gonzalez-Lorenzo)" << endl;
	if (0 >= s || s >= r || r >= 1)
	{
		std::cout << "Error in fileName: wrong parameters. Choose 0 < s < r < 1" << endl;
		exit(0);
	}
	for (int q = 0; q <= 3; q++)//pour dimensions de 0 a 3-> q
	{
		file << "o " << q << "-cubes" << endl;
		for (int c = 0; c < cbc.nbrCubes(); c++)//parcourir les cubes
		{
			if (cbc.get(c) && cbc.dim(c) == q)//si le cube est de dim q
			{
				obj::coord temp_coord = cbc.pos2coord(c);//lire coords
				std::cout << temp_coord.x << temp_coord.y << temp_coord.z << endl;
				vector<int> coor(3);//et convertir
				coor[0]=temp_coord.x;
				coor[1] = temp_coord.y;
				coor[2] = temp_coord.z;
				//vector<int> coor = cube2coord(c);//récup les coordonnées du cube
				for (int i = 0; i < 3; i++)//pour chaque coordonnée
				{
					if (coor.at(i) % 2 == 0)//si elle est paire?-> voir fonction de calcul des coordos
					{
						v.at(2 * i + 0) = coor.at(i) / 2 - (r - s);
						v.at(2 * i + 1) = coor.at(i) / 2 + (r - s);
					}
					else //sinon : impaire
					{
						v.at(2 * i + 0) = coor.at(i) / 2 + 0 + (r + s);
						v.at(2 * i + 1) = coor.at(i) / 2 + 1 - (r + s);
					}
				}
				file << "v " << v.at(0) << " " << v.at(2) << " " << v.at(4) << endl;    // (-,-,-)
				file << "v " << v.at(1) << " " << v.at(2) << " " << v.at(4) << endl;    // (+,-,-)
				file << "v " << v.at(0) << " " << v.at(3) << " " << v.at(4) << endl;    // (-,+,-)
				file << "v " << v.at(1) << " " << v.at(3) << " " << v.at(4) << endl;    // (+,+,-)
				file << "v " << v.at(0) << " " << v.at(2) << " " << v.at(5) << endl;    // (-,-,+)
				file << "v " << v.at(1) << " " << v.at(2) << " " << v.at(5) << endl;    // (+,-,+)
				file << "v " << v.at(0) << " " << v.at(3) << " " << v.at(5) << endl;    // (-,+,+)
				file << "v " << v.at(1) << " " << v.at(3) << " " << v.at(5) << endl;    // (+,+,+)
				file << "f " << vertices + 2 << " " << vertices + 4 << " " << vertices + 8 << " " << vertices + 6 << endl;
				file << "f " << vertices + 1 << " " << vertices + 5 << " " << vertices + 7 << " " << vertices + 3 << endl;
				file << "f " << vertices + 3 << " " << vertices + 7 << " " << vertices + 8 << " " << vertices + 4 << endl;
				file << "f " << vertices + 1 << " " << vertices + 2 << " " << vertices + 6 << " " << vertices + 5 << endl;
				file << "f " << vertices + 5 << " " << vertices + 6 << " " << vertices + 8 << " " << vertices + 7 << endl;
				file << "f " << vertices + 1 << " " << vertices + 3 << " " << vertices + 4 << " " << vertices + 2 << endl;
				vertices += 8;
			}
		}
	}
	file.close();

	cout << "The obj file has been created in " << double(clock() - begin) / CLOCKS_PER_SEC << " secs." << endl;
}
int main()
{
	int choix = 0;
	if (choix == 0) {
		char x;
		PGM3D pgm;
		Obj obj;
		try {
			clock_t begin = clock();
			//lecture pgm
			pgm.load("fertility3.pgm");
			cout << "lecture pgm en " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
			//conversion pgm vers complexe
			begin = clock();
			shared_ptr<ComplexeCubique> cc = Conversion::PGM3D2ComplexeCubique(pgm);
			cout << "conversion pgm>C en " << double(clock() - begin) / CLOCKS_PER_SEC << endl; 
			//printCC(pgm);
			//printDimCC(*cc.get());
			//writeComplex(*cc,"log/machin_K.obj", 0.1, 0.05);
			//cin >> x;
			//calcul DGVF
			begin = clock();
			DGVF dg(cc.get());

			cout << "Construction DGVF en " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
			//calcul homologie
			begin = clock(); 
			dg.homology("testDGVF",0.1, 0.05);
			cout << "Calcul Homologie en " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
			//lecture obj
			begin = clock();
			obj.load("fertility100_11_V.obj");
			cout << "lecture obj en " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
			//conversion complexe vers obj
			begin = clock();
			cout << "conversion C>Obj en " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
			//conversion pgm vers obj
			begin = clock();
			cout << "conversion pgm>obj en " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
		}
		catch (FileError f) {
			cout << f.what();
		}
		catch (DataError f) {
			cout << f.what();
		}
		catch (exception f) {
			cout << f.what();
		}
		cin >> x;
	}
	else if (choix == 1) {
		WindowOpenGL app_window;
		app_window.run();
	}
	return 0;
}
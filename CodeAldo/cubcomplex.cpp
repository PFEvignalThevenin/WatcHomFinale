#include "cubcomplex.h"

#include <iostream>  // cout
#include <stdlib.h>  // exit()
#include <fstream>	// ifstream
#include <ctime>     // clock_t, clock()

using namespace std;


CubComplex::CubComplex(string fileName)
{
    size.resize(3);
    accSize.resize(3);
    numberCubes = 1;
    
    clock_t begin = clock();
    
    ifstream file;
    file.open(fileName.c_str(), ios::in);
    if(!file.good())
    {
        cout<<"Error in CubComplex(string nameFile): impossible to open " << fileName << "." << endl;
        exit(0);
    }
    
    string ignore;
    file >> ignore; // Ignore the first line with P2
    
    int p_size, size_inputFile = 1;
    //cout << "size = ";
    for(int i = 0; i < 3; i++)
    {
        if(!file.eof()) file >> p_size;
        else {cout << "Error: couldn't read the size." << endl; exit(0);}
        //cout << p_size << " ";
        size.at(i) = p_size;
        accSize.at(i) = numberCubes;
        numberCubes *= (2 * p_size + 1);
        size_inputFile *= p_size;
    }
    file >> ignore; // Ignore the third line with 255
    
    //cout << endl << "Voxels: ";
    
    inputFile.resize(size_inputFile);
    
    int p_v;
    for(int i = 0; i < size_inputFile; ++i)
    {
        if(!file.eof()) file >> p_v;
        else {cout << "Error: couldn't read the " << i << "-th voxel." << endl; exit(0);}
        inputFileinputFile.at(i) = p_v;
        //cout << p_v << " ";
    }
    file.close();
    
    cout << fileName << " read in " << double(clock() - begin) / CLOCKS_PER_SEC << " secs. There are " <<  numberCubes << " cubes" << endl;
}


void CubComplex::makeCubComplex(int p_primal)
{
    clock_t begin = clock();
    
    primal = (p_primal==1)?true:false;	// primal: for every voxels, we activate 27 cubes
	cubes.resize(numberCubes, !primal);
	for(unsigned int i = 0; i < inputFile.size(); i++)
	{
		if(inputFile.at(i) > 0)
		{
			int c, c0, c1, c2;
			c = voxel2cube(i);          cubes.at(c ) = primal;     // (0,0,0)
			c0 = nextCube(c, 1, 0);     cubes.at(c0) = primal;    // (+,0,0)
			c1 = nextCube(c0, 1, 1);    cubes.at(c1) = primal;    // (+,+,0)
			c2 = nextCube(c1, 1, 2);    cubes.at(c2) = primal;    // (+,+,+)
			c2 = nextCube(c1, -1, 2);   cubes.at(c2) = primal;    // (+,+,-)
			c1 = nextCube(c0, -1, 1);   cubes.at(c1) = primal;    // (+,-,0)
			c2 = nextCube(c1, 1, 2);    cubes.at(c2) = primal;    // (+,-,+)
			c2 = nextCube(c1, -1, 2);   cubes.at(c2) = primal;    // (+,-,-)
			c1 = nextCube(c0, 1, 2);    cubes.at(c1) = primal;    // (+,0,+)
			c1 = nextCube(c0, -1, 2);   cubes.at(c1) = primal;    // (+,0,-)
			c0 = nextCube(c, -1, 0);    cubes.at(c0) = primal;    // (-,0,0)
			c1 = nextCube(c0, 1, 1);    cubes.at(c1) = primal;    // (-,+,0)
			c2 = nextCube(c1, 1, 2);    cubes.at(c2) = primal;    // (-,+,+)
			c2 = nextCube(c1, -1, 2);   cubes.at(c2) = primal;    // (-,+,-)
			c1 = nextCube(c0, -1, 1);   cubes.at(c1) = primal;    // (-,-,0)
			c2 = nextCube(c1, 1, 2);    cubes.at(c2) = primal;    // (-,-,+)
			c2 = nextCube(c1, -1, 2);   cubes.at(c2) = primal;    // (-,-,-)
			c1 = nextCube(c0, 1, 2);    cubes.at(c1) = primal;    // (-,0,+)
			c1 = nextCube(c0, -1, 2);   cubes.at(c1) = primal;    // (-,0,-)
			c1 = nextCube(c, 1, 1);     cubes.at(c1) = primal;    // (0,+,0)
			c2 = nextCube(c1, 1, 2);    cubes.at(c2) = primal;    // (0,+,+)
			c2 = nextCube(c1, -1, 2);   cubes.at(c2) = primal;    // (0,+,-)
			c1 = nextCube(c, -1, 1);    cubes.at(c1) = primal;    // (0,-,0)
			c2 = nextCube(c1, 1, 2);    cubes.at(c2) = primal;    // (0,-,+)
			c2 = nextCube(c1, -1, 2);   cubes.at(c2) = primal;    // (0,-,-)
			c1 = nextCube(c, 1, 2);     cubes.at(c1) = primal;    // (0,0,+)
			c1 = nextCube(c, -1, 2);    cubes.at(c1) = primal;    // (0,0,-)

		}
	}
    cout << "Cubical complex computed in " << double(clock() - begin) / CLOCKS_PER_SEC << " secs." << endl;
}


int CubComplex::dim(int c)
{
    int q = 0;
    for(int i = 0; i < 3; i++)
    {
        q += (c % (2*size.at(i) + primal)) % 2;
        c /= 	  (2*size.at(i) + primal);
    }
    return q;
}


vector<int> CubComplex::voxel2coord(int voxel)
{
	vector<int> v(3);

	for(int i = 0; i < 3; i++)
	{
		v.at(i) = 2 * ( voxel % size.at(i) );
        if(primal > 0) v.at(i)++;
		voxel /= size.at(i);
	}
	return v;
}


int CubComplex::coord2cube(const vector<int> *coord)
{
	int c = 0;

	for(int i = 0; i < 3; i++)
	{
		c += coord->at(i) * accSize.at(i);
	}
	return c;
}


int CubComplex::voxel2cube(int voxel)
{
    vector<int> v = voxel2coord(voxel);
    return coord2cube(&v);
}


vector<int> CubComplex::cube2coord(int c)
{
	vector<int> v(3);

	for(int i = 0; i < 3; i++)
	{
        v.at(i) = c % (2*size.at(i) + primal);
		c /= (2*size.at(i) + 1);
	}
	return v;
}


int CubComplex::nextCube(int c, int dir, int axis)
{
	if(axis >= 3)
	{
        cout << "Error: wrong axis value in CubComplex::nextCube(int c, int dir, int axis)" << endl;
		exit(0);
	}

    vector<int> coord = cube2coord(c);
    if( (0 <= coord.at(axis) + dir) && (coord.at(axis) + dir <= 2*size.at(axis)+primal-1) )
	{
		return c + dir * accSize.at(axis);
	}
	return -1;// if we cannot
}


int CubComplex::nextCube(int c, int dir_x, int dir_y, int dir_z)
{
    int c1;
    c1 = nextCube(c, dir_x, 0);
    if(c1 < 0) return -1;
    c1 = nextCube(c1, dir_y, 1);
    if(c1 < 0) return -1;
    c1 = nextCube(c1, dir_z, 2);
    if(c1 < 0) return -1;
    return c1;
}


list<int> CubComplex::boundary(int c)
{
    list<int> l;
    vector<int> coord = cube2coord(c);
    int c2;

    for(int i = 0; i < 3; i++)//pour chaque axe de coordonée
    {
        if(coord.at(i) % 2 == 1)//si elle est impaire -> info dimension::?
        {
			//alors prendre les voisins sur cet axe
            c2 = nextCube(c, +1, i); l.push_back(c2);
            c2 = nextCube(c, -1, i); l.push_back(c2);
        }
    }
    return l;
}

//idem que bondary, mais info dimension opposé 
list<int> CubComplex::coboundary(int c)
{
    list<int> l;
    vector<int> coord = cube2coord(c);
    int c2;

    for(int i = 0; i < 3; i++)
    {
        if(coord.at(i) % 2 == 0)//<- là!!
        {
            c2 = nextCube(c, +1, i); if(c2 >= 0 && cubes.at(c2)) l.push_back(c2);
            c2 = nextCube(c, -1, i); if(c2 >= 0 && cubes.at(c2)) l.push_back(c2);
        }
    }
    return l;
}



void CubComplex::writeComplex(string fileName, double r, double s)
{
    clock_t begin = clock();
    int vertices = 0; //numéro du vertice écrit ds le point obj
    vector<double> v(6);

    /* Create the file */
    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if(!file)
    {
        cout << "Error in write2Obj(" << fileName << "): impossible to create the output text file." << endl;
        return;
    }
    file << "# Obj file created by WatcHom (Aldo Gonzalez-Lorenzo)" << endl;
    if( 0 >= s || s >= r || r >= 1)
    {
        cout << "Error in fileName: wrong parameters. Choose 0 < s < r < 1" << endl;
        exit(0);
    }
    for(int q = 0; q <= 3; q++)//pour dimensions de 0 a 3-> q
    {
        file << "o " << q << "-cubes" << endl;
        for(int c = 0; c < numberCubes; c++)//parcourir les cubes
        {
            if(cubes.at(c) && dim(c) == q)//si le cube est de dim q
            {
                vector<int> coor = cube2coord(c);//récup les coordonnées du cube
                for(int i = 0; i < 3; i++)//pour chaque coordonnée
                {
                    if(coor.at(i) % 2 == 0)//si elle est paire?-> voir fonction de calcul des coordos
                    {
                        v.at(2*i+0) = coor.at(i)/2 - (r-s);
                        v.at(2*i+1) = coor.at(i)/2 + (r-s);
                    }
                    else //sinon : impaire
                    {
                        v.at(2*i+0) = coor.at(i)/2 + 0 + (r+s);
                        v.at(2*i+1) = coor.at(i)/2 + 1 - (r+s);
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
                file << "f " << vertices+2 << " " << vertices+4 << " " << vertices+8 << " " << vertices+6 << endl;
                file << "f " << vertices+1 << " " << vertices+5 << " " << vertices+7 << " " << vertices+3 << endl;
                file << "f " << vertices+3 << " " << vertices+7 << " " << vertices+8 << " " << vertices+4 << endl;
                file << "f " << vertices+1 << " " << vertices+2 << " " << vertices+6 << " " << vertices+5 << endl;
                file << "f " << vertices+5 << " " << vertices+6 << " " << vertices+8 << " " << vertices+7 << endl;
                file << "f " << vertices+1 << " " << vertices+3 << " " << vertices+4 << " " << vertices+2 << endl;
                vertices += 8;
            }
        }
    }
    file.close();

    cout << "The obj file has been created in " << double(clock() - begin) / CLOCKS_PER_SEC << " secs." << endl;
}


int CubComplex::get_numberCubes()
{
    return numberCubes;
}

bool CubComplex::val(int c)
{
    return cubes.at(c);
}

int CubComplex::get_sizeComplex(int i)
{
    return 2*size.at(i)+primal;
}

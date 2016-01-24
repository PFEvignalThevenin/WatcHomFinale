#ifndef CUBCOMPLEX_H
#define CUBCOMPLEX_H

#include <string>
#include <vector>
#include <list>

using namespace std;

class CubComplex
{
public:
    CubComplex(string fileName);  // Read an input .pgm file
    
    void makeCubComplex(int p_primal);      // computes the cubical complex from the image
    
    int dim(int c);                  		// dimension of the cube
    vector<int> voxel2coord(int voxel);
    vector<int> cube2coord(int c);
    int voxel2cube(int voxel);
    int coord2cube(const vector<int> *coord);
    int nextCube(int c, int dir, int axis);
    int nextCube(int c, int dir_x, int dir_y, int dir_z);
    list<int> boundary(int c);
    list<int> coboundary(int c);
    void writeComplex(string fileName, double r, double s);

    /* Getters */
    int get_numberCubes();
    bool val(int c);
    int get_sizeComplex(int i);
    
private:
    int primal;             // 1: primal ; -1: dual
    vector<int> size;       // Size in every direction (taille de la structure du pgm)
    vector<int> accSize;    // (1, size[1], size[1]*size[2])
    vector<int> inputFile;  // Values read from the input .pgm file
    vector<bool> cubes;      // list of the cubes with their value (0: not there, 1: there)
    int numberCubes;        // number of voxels in the input .pgm file
};

#endif // CUBCOMPLEX_H

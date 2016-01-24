// Compile with
//	g++ *.cpp -o ../build-WatcHom/WatcHom

#include <iostream>
#include <sstream>	//istringstream
#include <string>	// stod()

#include "cubcomplex.h"
#include "dgvf.h"

using namespace std;

int main(int argc, char **argv)
{
	if(argc-1 < 1)
    {
        cout << "Error: you gave " << argc-1 << " arguments." << endl;
        cout << "Usage: " << argv[0] << " <input .pgm file> <OPTIONS>" << endl;
        cout << "Options:" << endl;
        cout << "-r, --radius:\t\tradius of the cubes (0.1 by default)." << endl;
        cout << "-s, --separation:\tseparation between the cubes (0.05 by default)." << endl;
        cout << "-p, --primal:\t\t1: primal complex (by default); -1: dual complex." << endl;
        cout << "-wc, --writeComplex:\twrites the complex." << endl;
        cout << "-------" << endl;
        cout << "Example: ./WatcHom volume.pgm -wc" << endl;
        return 0;
    }
    
    istringstream iss;
    double r = 0.1;
    double s = 0.05;
    int primal = 1;
    bool writeComp = false;
    string name = argv[1];
    name.erase(name.end()-4, name.end());
    
    for(int i = 2; i < argc; i++)
    {
		string arg = argv[i];
		if(arg == "-r" || arg == "--radius")
		{
			if (i+1 < argc) {i++; iss.str(argv[i]); iss >> r;}
			else {cout << argv[i] << " option needs a real value." << endl; return 1; }
		}
		else if(arg == "-s" || arg == "--separation")
		{
			if (i+1 < argc) {i++; iss.str(argv[i]); iss >> s;}
			else {cout << argv[i] << " option needs a real value." << endl; return 1; }
		}
		else if(arg == "-p" || arg == "--primal")
		{
			if (i+1 < argc) {i++; iss.str(argv[i]); iss >> primal;}
			else {cout << argv[i] << " option needs a +1/-1 value." << endl; return 1; }
		}
		else if(arg == "-w" || arg == "--writeComplex")
		{
            writeComp = true;
		}
	}
	
    CubComplex K(argv[1]);
    K.makeCubComplex(primal);
    if(writeComp)
        K.writeComplex("log/"+name+"_K.obj", r, s);
    DGVF V(&K);
    V.homology(name, r, s);


    return 0;
}


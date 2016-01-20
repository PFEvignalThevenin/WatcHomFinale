#include "Interface\WindowOpenGL.hpp"

#include <Data\PGM3D.hpp>
#include <Data\ComplexeCubique.hpp>
#include <Data\Conversion.hpp>
#include <ctime>

using namespace std;
using namespace sf;
using namespace sfg;
using namespace obj;

int main()
{
	int choix = 1;
	if (choix == 0) {
		PGM3D pgm;
		Obj obj;
		try {
			clock_t begin = clock();
			//lecture pgm
			pgm.load("fertility100.pgm");
			cout << "lecture pgm en " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
			//conversion pgm vers complexe
			begin = clock();
			shared_ptr<ComplexeCubique> cc = Conversion::PGM3D2ComplexeCubique(pgm);
			cout << "conversion pgm>C en " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
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
		char x;
		cin >> x;
	}
	else if (choix == 1) {
		WindowOpenGL app_window;
		app_window.run();
	}
	return 0;
}
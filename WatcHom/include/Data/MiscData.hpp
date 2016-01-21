#ifndef MISC_DATA_HPP
#define MISC_DATA_HPP

#include <vector>
#include "GL\freeglut.h"

namespace obj {
	/****************************Vertex**********************************/
	struct Vertex {
		float x, y, z;
	};
	/****************************Face************************************/
	typedef std::vector<int> face;
	/*****************************Coord***********************************/
	struct coord {
		int x, y, z;
	};
}
/*****************************Couleur********************************/
struct couleur {
	GLfloat rouge, vert, bleu, alpha;
};
/*******************************Dim**********************************/
enum Dim { d0 = 0, d1 = 1, d2 = 2, d3 = 3 };
#define DIM 4	//because we work on 4 dimensions (from 0 to 3)
/*******************************Axe**********************************/
enum Axe { x = 0, y = 1, z = 2 };
/********************************************************************/
#endif
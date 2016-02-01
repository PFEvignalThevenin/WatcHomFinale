#ifndef MISC_DATA_HPP
#define MISC_DATA_HPP

#include <vector>
#include <string>
#include "GL\freeglut.h"
/*
*Classe d'exception pour les pb de mémoire
*
*/
class DataError : public std::exception
{
public:
	DataError(std::string const& phrase = "Data Error") throw() :m_phrase(phrase) {}

	virtual const char* what() const throw()
	{
		return m_phrase.c_str();
	}

	virtual ~DataError() throw()
	{}

private:
	std::string m_phrase;            //Description de l'erreur
};
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
//fonction de conversion de int vers Axe
Axe int2Axe(int i);
/********************************************************************/
//surcharge d'opérateurs pour Vertex et coord
bool operator==(obj::coord const& a, obj::coord const& b);
obj::Vertex operator*(const float &a, const obj::coord &b);
obj::Vertex operator+(const obj::Vertex &a, const obj::Vertex &b);
#endif

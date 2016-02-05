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
/*******************************Axe**********************************/
enum Axe { x = 0, y = 1, z = 2 };
//fonction de conversion de int vers Axe
Axe int2Axe(int i);
//tourner dans le sens direct
Axe nextAxe(Axe a);
/****************************direction************************************/
typedef std::pair<Axe, bool> dir;
dir normale(dir v1, dir v2);
/*************************************************************************/
namespace obj {
	/****************************Face************************************/
	typedef std::vector<int> face;
	/*****************************Coord***********************************/
	class coord {
	public:
		coord() = default;
		~coord() = default;
		int x = 0, y = 0, z = 0;
		
		void setCoord(int x,int y, int z);
		int& operator[](unsigned int);
		bool operator==(coord const&);
		coord& operator*(const int &);
	};
}
/*****************************Couleur********************************/
struct couleur {
	GLfloat rouge, vert, bleu, alpha;
};
/*******************************Dim**********************************/
enum Dim { d0 = 0, d1 = 1, d2 = 2, d3 = 3 };
#define DIM 4	//because we work on 4 dimensions (from 0 to 3)
/********************************************************************/
#endif

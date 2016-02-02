#ifndef VERTEX_HPP
#define VERTEX_HPP
#include "Data\MiscData.hpp"

namespace obj {
	/****************************Vertex**********************************/
	class Vertex {
	public:
		Vertex() = default;
		~Vertex() = default;
		float x = 0, y = 0, z = 0;

		float& at(unsigned int);
		float& operator[](unsigned int);
		bool operator==(Vertex const&);
		Vertex& operator*(const float &);
		Vertex& operator+(const Vertex &);
		Vertex& translation(dir, float);
	};
}
#endif
#ifndef COMPLEXE_HPP
#define COMPLEXE_HPP

#include <list>

class Complexe
{
public:

private:
	virtual int dim(int pos) = 0;
	virtual std::list<int> boundary(int pos) = 0;
	virtual std::list<int> coboundary(int pos) = 0;
};

#endif

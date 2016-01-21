#ifndef COMPLEXE_HPP
#define COMPLEXE_HPP

#include <list>

/*Classe de Complexe au sens Topographique
 *interface. 
 *l'implémentation dépend de la spécificité du complexe : Cubique ou simple
 *pos est l'index de l'entité
 */

class Complexe
{
public:

private:
	virtual int dim(int pos) = 0;
	virtual std::list<int> boundary(int pos) = 0;
	virtual std::list<int> coboundary(int pos) = 0;
};

#endif

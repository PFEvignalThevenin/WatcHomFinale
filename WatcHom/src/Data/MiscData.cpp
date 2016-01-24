#include "Data\MiscData.hpp"


Axe int2Axe(int i) {
	switch (i) {
	case Axe::x:
		return Axe::x;
		break;
	case Axe::y:
		return Axe::y;
		break;
	case Axe::z:
		return Axe::z;
		break;
	default:
		throw DataError("DataError: int2Axe(int) can't convert " + std::to_string(i));
	}
}
#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include "Data\ComplexeCubique.hpp"
#include "Data\PGM3D.hpp"
#include "Data\Obj.hpp"
#include <memory>

class Conversion
{
public:
	static std::shared_ptr<ComplexeCubique> PGM3D2ComplexeCubique(PGM3D &pgm, bool primal=true);
	static std::shared_ptr<obj::Obj> PGM3D2Obj(PGM3D &pgm);
	static std::shared_ptr<obj::Obj> ComplexeCubique2Obj(ComplexeCubique &pgm);
private:
	inline static coord coordPGM2ComplexeCub(int x, int y, int z);

};

#endif
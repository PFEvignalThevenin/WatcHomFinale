#pragma once
#ifndef PGM3D_HPP
#define PGM3D_HPP

#include "Data\FileSavable.hpp"
#include "Data\StructureCubique.hpp"

class PGM3D : public FileSavable, public StructureCubique<char>
{
public:
	PGM3D();
	~PGM3D();
	//fonctions d'accès fichiers
	virtual void save(std::string const& path) const override;
	virtual void load(std::string const& path) override;
private:
};

#endif
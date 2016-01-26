#pragma once
#ifndef PGM3D_HPP
#define PGM3D_HPP

#include "Data\FileSavable.hpp"
#include "Data\StructureCubique.hpp"
#include <memory>

/* Classe de correspondance avec un fichier .pgm
 *		comprenez bien qu'il s'agit d'un fichier pgm batard pour les données 3D.
 * Structure de fichier : 
 *	- nombre magique P2
 *	-les 3 dimensions
 *	-'255', parce que je le vaux bien
 *	-les voxels sous forme de 0 et de 1 séparés par un espace
 */
class PGM3D : public FileSavable, public StructureCubique<char>
{
public:
	typedef std::shared_ptr<PGM3D> Ptr;
	PGM3D();
	~PGM3D();
	//fonctions d'accès fichiers
	virtual void save(std::string const& path) const override;
	virtual void load(std::string const& path) override;
private:
};

#endif
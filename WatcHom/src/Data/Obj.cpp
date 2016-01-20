
#include "Data\Obj.hpp"
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/postprocess.h>
#include <fstream>
#include <sstream> 
#include <iostream> 

namespace obj {
	using namespace std;

	void Obj::save(std::string const& path) const {
		Assimp::Exporter exporter;
		bool sucess = exporter.Export(scene, ".obj", path);
		if (!sucess) {
			throw FileError(0, "Error in OBJ save : \n\timpossible to create " + path + ".");
		}
	}

	void Obj::load(std::string const& path) {
		Assimp::Importer importer;

		scene = importer.ReadFile(path,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);
		// If the import failed, report it
		if (!scene)
		{
			throw FileError(0, "Error in OBJ load : \n\timpossible to read " + path + ".");
		}
	}
	const aiScene*  Obj::getScene() {
		return scene;
	}
}
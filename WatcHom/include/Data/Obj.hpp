#ifndef Obj_HPP
#define Obj_HPP

#include "Data\FileSavable.hpp"
#include <assimp/scene.h>           // Output data structure
#include <memory>

namespace obj {

	class Obj : public FileSavable
	{
	public:
		typedef std::shared_ptr<obj::Obj> Ptr;
		Obj() = default;
		~Obj() = default;
		virtual void save(std::string const& path) const override;
		virtual void load(std::string const& path) override;

		const aiScene* getScene();
	private:
		const aiScene* scene;
	};
}
#endif
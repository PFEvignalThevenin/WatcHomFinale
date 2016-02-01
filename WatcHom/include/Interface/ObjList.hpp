#pragma once

#include "Engine\Controlleur2.hpp"
#include "SFGUI\Widgets.hpp"

class ObjList : public sfg::Bin
{
public:
	typedef std::shared_ptr<ObjList> Ptr;
	~ObjList() = default;
	static Ptr Create();
	const std::string& GetName() const override;

	//mise à jour de la liste des objs 'objBoxs'
	void majNav();
private:
	ObjList() = default;
	std::vector<sfg::CheckButton::Ptr> objBoxs[4];//liste des objets que l'on peut choisir d'afficher, pour chaque dimension
	sfg::Box::Ptr conteneur[4]; //contient chaque des listes précédentes
	sfg::Button::Ptr but[4];
	sf::Vector2f CalculateRequisition() override;
};

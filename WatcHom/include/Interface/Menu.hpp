#pragma once
#ifndef MENU_HPP
#define MENU_HPP

#include "SFGUI\Widgets.hpp"

class Menu : public sfg::Bin
{
public:
	typedef std::shared_ptr<Menu> Ptr;

	~Menu() = default;
	void bound(std::shared_ptr<Widget> w1, std::shared_ptr<Widget> w2);
	const std::string& GetName() const override;
	static std::shared_ptr<Menu> Create();
	// r�cup�rer un bouton selon son label
	sfg::Button::Ptr getButton(std::string const &label);
private:
	Menu();
	std::shared_ptr<sfg::Table> table;
	sf::Vector2f CalculateRequisition() override;
	void HandleSizeChange()override;
	std::map<std::string, sfg::Button::Ptr> boutons;

	void addButton2MenuList(sfg::Box::Ptr, sfg::Button::Ptr);
};

#endif
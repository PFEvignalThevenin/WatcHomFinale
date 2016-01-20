#pragma once
#ifndef MENU_HPP
#define MENU_HPP

#include "SFGUI\Widgets.hpp"

class Menu : public sfg::Bin
{
public:
	typedef std::shared_ptr<Menu> Ptr;

	Menu();
	~Menu();
	void bound(std::shared_ptr<Widget> w1, std::shared_ptr<Widget> w2);
	const std::string& GetName() const override;
	/** Create box.
	* @param orientation Orientation.
	* @param spacing Spacing = space between widgets.
	* @return Box.
	*/
	static std::shared_ptr<Menu> Create();
	/** récupérer un bouton selon son label
	*/
	sfg::Button::Ptr getButton(std::string const &label);
private:
	std::shared_ptr<sfg::Table> table;
	sf::Vector2f CalculateRequisition() override;
	std::map<std::string, sfg::Button::Ptr> boutons;

	void addButton2MenuList(sfg::Box::Ptr, sfg::Button::Ptr);
};

#endif
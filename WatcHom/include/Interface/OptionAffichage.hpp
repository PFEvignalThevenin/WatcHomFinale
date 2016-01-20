#pragma once
#ifndef WINDOW_OPTION_AFFICHAGE_HPP
#define WINDOW_OPTION_AFFICHAGE_HPP

#include "SFGUI/Bin.hpp"
#include "SFGUI/Box.hpp"
#include "SFGUI/SpinButton.hpp"

class OptionAffichage : public sfg::Bin
{
public:
	typedef std::shared_ptr<OptionAffichage> Ptr;
	OptionAffichage();
	~OptionAffichage();
	static std::shared_ptr<OptionAffichage> Create();
	const std::string& GetName() const;
private:
	void HandleSizeChange() override;
	std::map<std::string, sfg::SpinButton::Ptr> espaces;
protected:
	sfg::Box::Ptr vBox;
	sf::Vector2f CalculateRequisition() override;
	sfg::Box::Ptr BoxTailles();
};

#endif
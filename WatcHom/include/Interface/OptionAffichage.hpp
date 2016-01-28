#pragma once
#ifndef WINDOW_OPTION_AFFICHAGE_HPP
#define WINDOW_OPTION_AFFICHAGE_HPP

#include "SFGUI/Bin.hpp"
#include "SFGUI/Box.hpp"
#include "SFGUI\SpinButton.hpp"
#include "Engine\Controlleur2.hpp"
#include "Interface\ParamAffichageDim.hpp"

class OptionAffichage : public sfg::Bin
{
public:
	typedef std::shared_ptr<OptionAffichage> Ptr;
	OptionAffichage();
	~OptionAffichage() = default;
	static std::shared_ptr<OptionAffichage> Create(int rayon=2, int Longeur=8, int separation=2);
	const std::string& GetName() const override;
	float getRayon();
	float getLongueur();
	float getSeparation();
	//met à jour le controleur
	void affectEspaceChanges();
	//change les valeurs des curseurs ET met à jour le controleur
	void setEspaces(int rayon, int Longeur, int separation);
	//met à jour le controleur
	void affectColorChanges(int dim);
	//change les valeurs des curseurs ET met à jour le controleur
	void setColors(int dim,int r, int g, int b, int a);
private:
	void HandleSizeChange() override;
	std::map<std::string, sfg::SpinButton::Ptr> espaces;
protected:
	//sfg::Box::Ptr vBox;
	std::vector<ParamAffichageDim::Ptr> paramsRGBA;
	sf::Vector2f CalculateRequisition() override;
	sfg::Box::Ptr BoxTailles();
};

#endif
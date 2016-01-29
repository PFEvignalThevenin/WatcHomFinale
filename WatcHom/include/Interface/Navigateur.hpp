#pragma once
#include "SFGUI\Bin.hpp"
#include "SFGUI\Widgets.hpp"
#include <vector>

class Navigateur : public sfg::Bin
{
public:
	typedef std::shared_ptr<Navigateur> Ptr;
	Navigateur();
	~Navigateur() = default;
	static Navigateur::Ptr Create();
	const std::string& GetName() const override;

	void setSaves(bool obj = true, bool morse = true);
	void affichageDims(bool dims[4]);
	void setAutoroll(bool set  = true);
	void affectAutoroll();
	void recentrer();
	void affectSaves();
	void affectAffDim();
private:
	sfg::CheckButton::Ptr saveObj, saveMorse;
	std::vector<sfg::CheckButton::Ptr> checkBoxs;
	sfg::Button::Ptr but_centrer;
	sfg::CheckButton::Ptr autoroll;

	//void HandleSizeChange() override;
protected:
	sf::Vector2f CalculateRequisition() override;
};
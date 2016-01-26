#include "Interface\OptionAffichage.hpp"
#include "Interface\ParamAffichageDim.hpp"

#include "SFGUI\Label.hpp"
#include "SFGUI\SpinButton.hpp"
#include "Engine\Controlleur2.hpp"
#include "SFML\Window.hpp"

using namespace sfg;

OptionAffichage::OptionAffichage() : sfg::Bin()
{
	vBox = Box::Create(Box::Orientation::VERTICAL);
	//ajout des parametrages de couleur pour chaque dimension
	for (int i = 0; i <= 3; i++) {
		vBox->Pack(ParamAffichageDim::Create(i));
	}
	//ajout des choix de tailles
	vBox->Pack(BoxTailles());
	espaces.at("Rayon")->SetValue(2);
	espaces.at("Longueur")->SetValue(8);
	espaces.at("Espace")->SetValue(2);
	//setDistances
}


OptionAffichage::~OptionAffichage()
{
}

/*****************
fonctions pour l'héritage et l'affichage
*****************/

const std::string& OptionAffichage::GetName() const {
	static const std::string name("OptionAffichage");
	return name;
}
OptionAffichage::Ptr OptionAffichage::Create() {
	Ptr ret(new  OptionAffichage());
	ret->Add(ret->vBox);
	return ret;
}
sf::Vector2f OptionAffichage::CalculateRequisition() {
	return GetChild()->GetRequisition();
}

void OptionAffichage::HandleSizeChange() {
	auto child = GetChild();
	if (!child) {
		return;
	}
	sf::FloatRect allocation(GetAllocation());
	allocation.left = 0;
	allocation.top = 0;
	child->SetAllocation(allocation);
}
sfg::Box::Ptr OptionAffichage::BoxTailles() {
	std::string labels[] = { "Rayon","Longueur","Espace" };
	auto ret = Box::Create();
	for (std::string lab : labels) {
		auto entry = SpinButton::Create(0, 10, 1);
		espaces.insert(std::pair<std::string, SpinButton::Ptr>(lab, entry));
		//délégation
		entry->GetSignal(sfg::Widget::OnStateChange).Connect(//mise à jour sur utilisation du scale
			std::bind([=]() {
			Controlleur2::get()->setDistances(getRayon(), getLongueur(), getSeparation());
		}));
		//ajout au groupe
		ret->Pack(Label::Create(lab));
		ret->Pack(entry);
	}
	return ret;
}
float OptionAffichage::getRayon() {
	return espaces.at("Rayon")->GetValue();
}
float OptionAffichage::getLongueur() {
	return espaces.at("Longueur")->GetValue();
}
float OptionAffichage::getSeparation() {
	return espaces.at("Espace")->GetValue();
}
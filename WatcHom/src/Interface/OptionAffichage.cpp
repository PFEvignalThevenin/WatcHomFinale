#include "Interface\OptionAffichage.hpp"

#include "SFGUI\Label.hpp"
#include "SFML\Window.hpp"

using namespace sfg;

OptionAffichage::OptionAffichage() : sfg::Bin()
{
	//creation des parametrages de couleur pour chaque dimension
	for (int i = 0; i <= 3; i++) {
		//vBox->Pack();
		paramsRGBA.push_back(ParamAffichageDim::Create(i));
	}
}

/*****************
fonctions pour l'héritage et l'affichage
*****************/

const std::string& OptionAffichage::GetName() const {
	static const std::string name("OptionAffichage");
	return name;
}
OptionAffichage::Ptr OptionAffichage::Create(int rayon, int Longeur, int separation) {
	Ptr ret(new  OptionAffichage());
	//ajout des parametrages de couleur pour chaque dimension
	Box::Ptr vBox = Box::Create(Box::Orientation::VERTICAL);
	for (int i = 0; i <= 3; i++) {
		vBox->Pack(ret->paramsRGBA.at(i));
	}
	//ajout des choix de tailles
	vBox->Pack(ret->BoxTailles());
	//setDistances
	ret->setEspaces(rayon, Longeur, separation);
	ret->Add(vBox);
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
		auto entry = SpinButton::Create(1, 10, 1);
		espaces.insert(std::pair<std::string, SpinButton::Ptr>(lab, entry));
		//délégation de la mise à jour
		entry->GetSignal(sfg::Widget::OnStateChange).Connect(std::bind(&OptionAffichage::affectEspaceChanges, this));//mise à jour sur utilisation du scale
		//ajout au groupe
		ret->Pack(Label::Create(lab));
		ret->Pack(entry);
	}
	return ret;
}
void OptionAffichage::affectEspaceChanges() {
	Controlleur2::get()->setDistances(getRayon(), getLongueur(), getSeparation());
}
void OptionAffichage::setEspaces(int rayon, int Longeur, int separation) {
	espaces.at("Rayon")->SetValue((float)rayon);
	espaces.at("Longueur")->SetValue((float)Longeur);
	espaces.at("Espace")->SetValue((float)separation);
	espaces.at("Espace")->SetRange(0, 10);//tous les autres ne peuvent avoir 0
	affectEspaceChanges();
}
float OptionAffichage::getRayon() {
	return espaces.at("Rayon")->GetValue() / 10;
}
float OptionAffichage::getLongueur() {
	return espaces.at("Longueur")->GetValue() / 10;
}
float OptionAffichage::getSeparation() {
	return espaces.at("Espace")->GetValue() / 10;
}
void OptionAffichage::affectColorChanges(int dim) {
	paramsRGBA[dim]->affectColorChange();
}
void OptionAffichage::setColors(int dim, int r, int g, int b, int a) {
	paramsRGBA[dim]->setColors( r,  g,  b,  a);
	paramsRGBA[dim]->affectColorChange();
}
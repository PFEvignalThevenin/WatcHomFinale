#include "Interface\ParamAffichageDim.hpp"
#include "Engine\Controlleur2.hpp"
#include "SFGUI\Widgets.hpp"

using namespace sfg;
using namespace std;

ParamAffichageDim::ParamAffichageDim(int dim) : Frame()
{
	dimension = dim;
	hBox = Box::Create();
	initScales();
	for (std::map<string, Scale::Ptr>::iterator it = scalers.begin(); it != scalers.end(); ++it) {
		hBox->Pack(sfg::Label::Create(it->first + " "));
		hBox->Pack(it->second);
	}
	scalers.at("Alpha")->SetValue(255);
}

ParamAffichageDim::~ParamAffichageDim()
{
}

/*****************
Fonctions pour récupérer les valeurs de couleur, alpha et dimension
*****************/

float ParamAffichageDim::getRed() {
	return scalers.at("Rouge")->GetValue();
}
float ParamAffichageDim::getBlue() {
	return scalers.at("Bleu")->GetValue();
}
float ParamAffichageDim::getGreen() {
	return scalers.at("Vert")->GetValue();
}
float ParamAffichageDim::getAlpha() {
	return scalers.at("Alpha")->GetValue();
}
int ParamAffichageDim::getDimension() {
	return dimension;
}

/*****************
Fonction d'initialisation des curseurs des valeurs parametrables
*****************/

void ParamAffichageDim::initScales() {
	string labels[] = { "Rouge", "Vert","Bleu","Alpha" };
	for (string lab : labels) {
		scalers.insert(pair<string, Scale::Ptr>(lab, Scale::Create(0, 255, 1)));
		scalers.at(lab)->SetValue(100);
		//mise à jour sur utilisation du scale
		scalers.at(lab)->GetSignal(sfg::Widget::OnMouseMove).Connect(bind(&ParamAffichageDim::affectColorChange, this));
	}
}

/*****************
fonctions pour l'héritage et l'affichage
*****************/

const std::string& ParamAffichageDim::GetName() const {
	static const std::string name("ParamAffichageDim");
	return name;
}
std::shared_ptr<ParamAffichageDim> ParamAffichageDim::Create(int dim) {
	Ptr ret(new  ParamAffichageDim(dim));
	ret->SetLabel("Dimension " + to_string(dim));
	ret->Add(ret->hBox);
	return ret;
}
void ParamAffichageDim::affectColorChange() {
	Controlleur2::get()->setCouleur(Controlleur2::int2Dim(dimension), getRed(), getGreen(), getBlue(), getAlpha());
}
void ParamAffichageDim::setColors(int r, int g, int b, int a) {
	scalers.at("Rouge")->SetValue((float)r);
	scalers.at("Vert")->SetValue((float)g);
	scalers.at("Bleu")->SetValue((float)b);
	scalers.at("Alpha")->SetValue((float)a);
}
#include "Interface\Navigateur.hpp"
#include "Engine\Controlleur2.hpp"

using namespace std;
using namespace sfg;

Navigateur::Navigateur() : sfg::Bin()
{
	saveObj = CheckButton::Create("Save obj");
	saveMorse = CheckButton::Create("Save Morse");
	saveObj->GetSignal(CheckButton::OnToggle).Connect(bind(&Navigateur::affectSaves, this));
	saveMorse->GetSignal(CheckButton::OnToggle).Connect(bind(&Navigateur::affectSaves, this));
	for (int i = 0; i < 4; i++) {
		checkBoxs.push_back(CheckButton::Create("Dim "+to_string(i)));
		checkBoxs[i]->SetActive(true);
		checkBoxs[i]->GetSignal(CheckButton::OnToggle).Connect(bind(&Navigateur::affectAffDim, this));
	}
	affectAffDim();
	but_centrer = Button::Create("Centrer");
	but_centrer->GetSignal(Button::OnLeftClick).Connect(bind(&Navigateur::recentrer, this));
	butSaveObj = Button::Create("Export obj");//boutons de sauvegarde
	butSaveMorse = Button::Create("Export Morse");
	autoroll = CheckButton::Create("Autoroll");
	autoroll->GetSignal(CheckButton::OnToggle).Connect(bind(&Navigateur::affectAutoroll, this));
	setAutoroll(true);
}
Navigateur::Ptr Navigateur::Create() {
	Navigateur::Ptr ret = Navigateur::Ptr(new Navigateur());
	//conteneurs
	auto hb = Box::Create(Box::Orientation::VERTICAL);
	auto f_saves = Frame::Create("saves");
	auto f_dim = Frame::Create("aff. dims");
	auto f_nav = Frame::Create("navigation");
	//frame saves
	auto hbf1 = Box::Create(Box::Orientation::VERTICAL);
	hbf1->Pack(ret->saveObj);
	hbf1->Pack(ret->saveMorse);
	hbf1->Pack(ret->butSaveObj);
	hbf1->Pack(ret->butSaveMorse);

	f_saves->Add(hbf1);
	//frame dims
	auto hbf2 = Box::Create(Box::Orientation::VERTICAL);
	for (int i = 0; i < 4; i++) {
		hbf2->Pack(ret->checkBoxs[i]);
	}
	f_dim->Add(hbf2);
	//frame nav
	auto hbf3 = Box::Create(Box::Orientation::VERTICAL);
	hbf3->Pack(ret->autoroll);
	hbf3->Pack(ret->but_centrer);
	f_nav->Add(hbf3);
	//ajouter au bin
	hb->Pack(f_saves);
	hb->Pack(f_dim);
	hb->Pack(f_nav);
	ret->Add(hb);
	return ret;
}
void Navigateur::setMainWindow(std::shared_ptr<WindowOpenGL> main_win) {
	this->main_win = main_win;
	butSaveObj->GetSignal(Button::OnLeftClick).Connect(bind(&WindowOpenGL::saveObj, main_win));
	butSaveMorse->GetSignal(Button::OnLeftClick).Connect(bind(&WindowOpenGL::saveMorse, main_win));
}
const std::string& Navigateur::GetName() const {
	static const std::string name("Navigateur");
	return name;
}
sf::Vector2f Navigateur::CalculateRequisition() {
	return GetChild()->GetRequisition();
}
/*void Navigateur::HandleSizeChange() {
	auto child = GetChild();
	if (!child) {
		return;
	}
	sf::FloatRect allocation(GetAllocation());
	allocation.left = 0;
	allocation.top = 0;
	child->SetAllocation(allocation);
}*/
void Navigateur::setSaves(bool obj , bool morse ) {
	saveObj->SetActive(obj);
	saveMorse->SetActive(morse);
	affectSaves();
}
void Navigateur::affichageDims(bool dims[4]) {
	for (int i = 0; i < 4; i++) {
		checkBoxs[i]->SetActive(dims[i]);
	}
	affectAffDim();
}
void Navigateur::setAutoroll(bool set) {
	autoroll->SetActive(set);
	affectAutoroll();
}
void Navigateur::affectAutoroll() {
	Controlleur2::get()->setAutoroll(autoroll->IsActive());
}
void Navigateur::recentrer() {
	Controlleur2::get()->recentrer();
}
void Navigateur::affectSaves() {
	main_win->setSave(saveObj->IsActive(),saveMorse->IsActive());
}
void Navigateur::affectAffDim() {
	Controlleur2::Ptr c = Controlleur2::get();
	for (Dim d : {Dim::d0, Dim::d1, Dim::d2, Dim::d3}) {
		c->setAffichageDim(d, checkBoxs[d]->IsActive());
	}
}
#include "Interface\ClusterList.hpp"
#include "Engine\Controlleur2.hpp"

using namespace std;
using namespace sfg;

ClusterList::ClusterList(ObjList::Ptr ol) : Bin()
{
	lab_tete = Label::Create("Waiting for a Complexe");
	b_collapse = Button::Create("Collapse");
	b_merge = Button::Create("Merge");
	b_retour = Button::Create("Back");
	for (unsigned int i = 0; i < 4; i++) {
		hb_collapseList[i] = Box::Create(Box::Orientation::VERTICAL);	//liste collapses
		but[i] = Button::Create("Dimension " + to_string(i));			//boutons de réduction
		but[i]->GetSignal(Button::OnLeftClick).Connect(bind([=](Box::Ptr b) {//ajout fonction de réduction de la liste
			if (b->IsLocallyVisible()) {
				b->Show(false);
			}
			else {
				b->Show(true);
			}
		}, hb_collapseList[i]));
		but[i]->SetId("butDimCollapse" + to_string(i));
	}
	b_merge->Show(false);
	b_collapse->Show(false);
	b_retour->Show(false);
	b_merge->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ClusterList::ButtonMerge, this));
	b_collapse->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ClusterList::ButtonCollapse, this));
	b_retour->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ClusterList::ButtonBack, this));
	this->objList = ol;
}
ClusterList::Ptr ClusterList::Create(ObjList::Ptr ol) {
	ClusterList::Ptr ret(new ClusterList(ol));
	auto hb = Box::Create(Box::Orientation::VERTICAL);
	hb->Pack(ret->lab_tete);
	for (unsigned int i = 0; i < 4; i++) {
		hb->Pack(ret->but[i]);
		hb->Pack(ret->hb_collapseList[i]);
	}
	hb->Pack(ret->b_collapse);
	hb->Pack(ret->b_merge);
	hb->Pack(ret->b_retour);
	ret->Add(hb);
	return ret;
}
void ClusterList::setMainWindow(std::shared_ptr<WindowOpenGL> main_win) {
	this->main_win = main_win;
}
const std::string& ClusterList::GetName() const {
	static const std::string name("ClusterList");
	return name;
}
void ClusterList::refresh() {
	//la liste des objs change
	objList->majNav();
	int nbrIter = Controlleur2::get()->getNbrIterations();
	//gestion bouton retour arrière
	if (nbrIter < 2) {
		b_retour->Show(false);
	}
	else {
		b_retour->Show(true);
	}
	//réinitialiser
	radios.clear();
	clusters = Controlleur2::get()->getCollapses();
	for (unsigned int i = 0; i < 4; i++) {
		hb_collapseList[i]->RemoveAll();
		sfg::Context::Get().GetEngine().SetProperty("*#butDimCollapse" + to_string(i), "Color", sf::Color(250, 0, 0));
	}
	//cas particulier si début de l'algo
	if (nbrIter == 0) {
		lab_tete->SetText("PGM");
		b_merge->Show(true);
		radios.clear();
		b_collapse->Show(false);
		return;
	}
	//gestion Perfection : label d'entête
	if (Controlleur2::get()->isPerfect()) {
		lab_tete->SetText("Perfect");
		radios.clear();
	}
	else {
		lab_tete->SetText("Clusters to collapse");
	}
	//gestion bouton merge
	if (Controlleur2::get()->isClusterisable()) {
		b_merge->Show(true);
	}else {
		b_merge->Show(false);
	}
	//gestion bouton Collapse
	if (clusters->size()==0) {
		b_collapse->Show(false);
		return;
	}else
		b_collapse->Show(true);
	//liste des clusters
	int q;
	for (std::pair<int, int> bind : *clusters) {
		q = Controlleur2::get()->getDim(bind.first);
		RadioButton::Ptr radBut = RadioButton::Create(to_string(q) + "_" + to_string(bind.first) + " -> " + to_string(q + 1) + "_" + to_string(bind.second));
		radios.push_back(radBut);
		hb_collapseList[q]->Pack(radBut);
		radBut->SetGroup(radios[0]->GetGroup());
	}
	//finalisations : couleur boutons
	for (unsigned int i = 0; i < 4; i++) {
		if(hb_collapseList[i]->GetChildren().size() != 0)
			sfg::Context::Get().GetEngine().SetProperty("*#butDimCollapse" + to_string(i), "Color", sf::Color(200, 200, 200));
	}
}
void ClusterList::ButtonCollapse() {//with a radioButton selected
	for (unsigned int i = 0; i < radios.size(); i++) {//find the active radioButton
		if (radios[i]->IsActive()) {
			Controlleur2::get()->collapse(clusters->at(i).first, clusters->at(i).second);
			main_win->autoSave();
			refresh();
		}
	}
}
void ClusterList::ButtonMerge() {//enabled only if collabsable is true
	Controlleur2::get()->cellClustering();
	main_win->autoSave();
	refresh();
}
void ClusterList::ButtonBack() {
	Controlleur2::get()->retourIterPrecedente();
	refresh();
}
sf::Vector2f ClusterList::CalculateRequisition() {
	return GetChild()->GetRequisition();
}
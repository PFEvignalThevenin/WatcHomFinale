#include "Interface\ClusterList.hpp"
#include "Engine\Controlleur2.hpp"

using namespace std;
using namespace sfg;

ClusterList::ClusterList() : Bin()
{
	hb_collapseList = Box::Create(Box::Orientation::VERTICAL);
	lab_tete = Label::Create("Waiting for a Complexe");
	b_collapse = Button::Create("Collapse");
	b_merge = Button::Create("Merge");
	hb_collapseList = Box::Create(Box::Orientation::VERTICAL);
	b_merge->Show(false);
	b_collapse->Show(false);
	b_merge->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ClusterList::ButtonMerge, this));
	b_collapse->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&ClusterList::ButtonCollapse, this));

}
ClusterList::Ptr ClusterList::Create() {
	ClusterList::Ptr ret(new ClusterList());
	auto hb = Box::Create(Box::Orientation::VERTICAL);
	hb->Pack(ret->lab_tete);
	hb->Pack(ret->hb_collapseList);
	hb->Pack(ret->b_collapse);
	hb->Pack(ret->b_merge);
	ret->Add(hb);
	return ret;
}
const std::string& ClusterList::GetName() const {
	static const std::string name("ClusterList");
	return name;
}
void ClusterList::refresh() {
	if (Controlleur2::get()->isPerfect()) {
		lab_tete->SetText("Perfect");
		hb_collapseList->RemoveAll();
		radios.clear();
	}
	else {
		lab_tete->SetText("Clusters to collapse");
	}
	if (Controlleur2::get()->isClusterisable()) {
		b_merge->Show(true);
	}
	clusters = Controlleur2::get()->getCollapses();
	hb_collapseList->RemoveAll();
	radios.clear();
	if (clusters->size()==0) {
		b_collapse->Show(false);
		return;
	}
	hb_collapseList->RemoveAll();
	radios.clear();
	int q;
	for (std::pair<int, int> bind : *clusters) {
		q = Controlleur2::get()->getDim(bind.first);
		radios.push_back(RadioButton::Create(to_string(q) + "_" + to_string(bind.first) + " -> " + to_string(q + 1) + "_" + to_string(bind.second)));
	}
	hb_collapseList->Pack(radios[0]);
	for (unsigned int i = 1; i < radios.size(); i++) {
		radios[i]->SetGroup(radios[0]->GetGroup()); 
		hb_collapseList->Pack(radios[i]);
	}
	b_collapse->Show(true);
}
void ClusterList::ButtonCollapse() {//with a radioButton selected
	for (unsigned int i = 0; i < radios.size(); i++) {//find the active radioButton
		if (radios[i]->IsActive()) {
			Controlleur2::get()->collapse(clusters->at(i).first, clusters->at(i).second);
		}
	}refresh();
}
void ClusterList::ButtonMerge() {//enabled only if collabsable is true
	Controlleur2::get()->cellClustering();
	b_merge->Show(false);
	refresh();
}
sf::Vector2f ClusterList::CalculateRequisition() {
	return GetChild()->GetRequisition();
}
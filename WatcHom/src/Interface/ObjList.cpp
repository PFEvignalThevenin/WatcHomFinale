#include "Interface\ObjList.hpp"

using namespace sfg;
using namespace std;

ObjList::Ptr ObjList::Create() {
	ObjList::Ptr ret(new ObjList());
	auto hb = Box::Create(Box::Orientation::VERTICAL);
	for (int i = 0; i < 4; i++) {
		ret->conteneur[i] = Box::Create(Box::Orientation::VERTICAL);//créer la box
		ret->but[i] = Button::Create("Dimension " + to_string(i));
		ret->but[i]->GetSignal(Button::OnLeftClick).Connect(bind([=](Box::Ptr b) {
			if (b->IsLocallyVisible()) {
				b->Show(false);
			}
			else {
				b->Show(true);
			}
		},ret->conteneur[i]));
		ret->but[i]->SetId("butDim"+to_string(i));
		hb->Pack(ret->but[i]);	//ajouter nom
		hb->Pack(ret->conteneur[i]);//ajouter box
	}
	ret->Add(hb);
	return ret;
}
const std::string& ObjList::GetName() const {
	static const std::string name("ObjList");
	return name;
}
sf::Vector2f ObjList::CalculateRequisition() {
	return GetChild()->GetRequisition();
}
void ObjList::majNav() {
	vector<GLuint> *objs;
	Controlleur2::Ptr ctrl = Controlleur2::get();
	for (int q = 0; q < 4; q++) {	//pour chaque dimension
		objs = Controlleur2::get()->getFormes(Controlleur2::int2Dim(q));//récupérer la liste des objets
		objBoxs[q].clear();	//enlever ancien contenu
		conteneur[q]->RemoveAll();
		if (objs->size() == 0) {
			//colorier boutton en rouge pour signifier que rien
			sfg::Context::Get().GetEngine().SetProperty("*#butDim"+to_string(q), "Color", sf::Color(250, 0, 0));
		}
		else {
			sfg::Context::Get().GetEngine().SetProperty("*#butDim" + to_string(q), "Color", sf::Color(200,200,200));
			for (GLuint i : *objs) {
				//ajouter à la nouvelle liste de checkButtons
				auto cb = CheckButton::Create(ctrl->getNameList(i));
				objBoxs[q].push_back(cb);
				//ajouter les checkButtons ds Box pour affichage
				conteneur[q]->Pack(cb);
				//linker fonction
				cb->GetSignal(CheckButton::OnToggle).Connect(bind([=]() {
					if (cb->IsActive()) {
						ctrl->setAfficherList(i, false);
					}
					else {
						ctrl->setAfficherList(i, true);
					}
				}));
			}
		}
	}
}
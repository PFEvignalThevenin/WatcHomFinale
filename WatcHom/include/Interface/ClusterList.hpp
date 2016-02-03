#pragma once

#include "SFGUI\Frame.hpp"
#include "SFGUI\Widgets.hpp"
#include "Engine\DGVF.hpp"
#include "Interface\ObjList.hpp"
#include "Interface\WindowOpenGL.hpp"
#include <memory>

class WindowOpenGL;

class ClusterList : public sfg::Bin
{
public:
	typedef std::shared_ptr<ClusterList> Ptr;
	static ClusterList::Ptr Create(ObjList::Ptr ol);
	~ClusterList() = default;
	void setMainWindow(std::shared_ptr<WindowOpenGL> main_win);
	const std::string& GetName() const override;

	void refresh();
	void ButtonCollapse();//with selecting a radioButton
	void ButtonMerge();	//enabled only if collabsable is true
	void ButtonBack();	//bouton pour retour en arrière ds itération de l'algorithme
private:
	ClusterList(ObjList::Ptr ol);
	ObjList::Ptr objList;	//liste des objets à afficher, car prévenir de mise à jour lorsque calcul d'algo
	std::shared_ptr<WindowOpenGL> main_win;//utilisé pour la sauvegarde
	std::shared_ptr<std::vector<DGVF::cellBound>> clusters;
	std::vector<sfg::RadioButton::Ptr> radios;
	sfg::Box::Ptr hb_collapseList[4]; //contient chacune des listes de radios triés par dimension
	sfg::Button::Ptr but[4];	//boutons pour ouvrir et fermet la liste par dimension
	sfg::Button::Ptr b_merge, b_collapse, b_retour;//merge, collapse, retour : en arrière d'une itération
	sfg::Label::Ptr lab_tete;

	sf::Vector2f CalculateRequisition() override;
	
};
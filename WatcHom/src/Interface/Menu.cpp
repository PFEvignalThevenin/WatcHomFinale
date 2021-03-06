#pragma once

#include "Interface\Menu.hpp"

using namespace sfg;
using namespace std;

Menu::Menu() : Bin()
{
	table = Table::Create();
	//Creation de la barre de menu
	//auto menuBar = Table::Create();
	auto menuFichier = Button::Create("Fichier");
	auto menuParametres = Button::Create("Parametres");
	//listes des menus
	auto menuListFichier = Box::Create(Box::Orientation::VERTICAL);
	auto menuListParametres = Box::Create(Box::Orientation::VERTICAL);
	//boutons du menu fichier
	//addButton2MenuList(menuListFichier, Button::Create("Ouvrir"));
	//addButton2MenuList(menuListFichier, Button::Create("Enregistrer"));
	addButton2MenuList(menuListFichier, Button::Create("Ouvrir Obj"));
	addButton2MenuList(menuListFichier, Button::Create("Importer Pgm"));
	addButton2MenuList(menuListFichier, Button::Create("Quitter"));
	//boutons du menu parametres
	addButton2MenuList(menuListParametres, Button::Create("Affichage"));
	addButton2MenuList(menuListParametres, Button::Create("Chemins"));
	//ajouter menus d�roulants � la barre de menus
	table->Attach(menuFichier, sf::Rect<sf::Uint32>(0, 0, 10, 10));
	table->Attach(menuParametres, sf::Rect<sf::Uint32>(10, 0, 10, 10));
	table->Attach(menuListFichier, sf::Rect<sf::Uint32>(0, 10, 10, 10));
	table->Attach(menuListParametres, sf::Rect<sf::Uint32>(10, 10, 10, 10));
	//comportements
	menuListFichier->Show(false);
	menuListParametres->Show(false);
	bound(menuFichier, menuListFichier);
	bound(menuParametres, menuListParametres);
}
/*relie les 2 entit�s afin que la 2e apparaisse lorsque la 1ere est survol�e par la souris
*/
void Menu::bound(std::shared_ptr<Widget> w1, std::shared_ptr<Widget> w2) {
	w1->GetSignal(sfg::Button::OnMouseEnter).Connect(
		std::bind([=]() {
		w2->Show(true);
		})
	);
	w2->GetSignal(sfg::Button::OnMouseLeave).Connect(
		std::bind([=]() {
		w2->Show(false);
		})
	);
}

const std::string& Menu::GetName() const {
	static const std::string name("Menu");
	return name;
}
std::shared_ptr<Menu> Menu::Create() {
	Ptr ret(new  Menu);
	ret->Add(ret->table);
	return ret;
}
sf::Vector2f Menu::CalculateRequisition() {
	return GetChild()->GetRequisition();
}
void Menu::HandleSizeChange() {
	auto child = GetChild();
	if (!child) {
		return;
	}
	sf::FloatRect allocation(GetAllocation());
	allocation.left = 0;
	allocation.top = 0;
	child->SetAllocation(allocation);
}

/* Fonctions de construction de menu */
void Menu::addButton2MenuList(sfg::Box::Ptr box, sfg::Button::Ptr but) {
	box->Pack(but);
	boutons.insert(pair<string, Button::Ptr>(but->GetLabel(), but));
}
sfg::Button::Ptr Menu::getButton(std::string const &label ) {
	return boutons.at(label);
}
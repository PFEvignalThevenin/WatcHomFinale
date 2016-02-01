#include "Interface\WinPaths.hpp"
#include "Engine\Controlleur2.hpp"
#include <sstream>

using namespace std;
using namespace sfg;

WinPaths::WinPaths() : sfg::Window(Window::Style::TOPLEVEL | Window::Style::CLOSE)
{
	objLoadPath = Entry::Create("fertility100_11_V.obj");
	pgmLoadPath = Entry::Create("fertility3.pgm");
	repoSavePath = Entry::Create("./results");
	objSuffixe = Entry::Create("_O_");
	morseSuffixe = Entry::Create("_V_");
	zommMulti = SpinButton::Create(2, 20, 2);;
}
WinPaths::Ptr WinPaths::Create() {
	WinPaths::Ptr ret = WinPaths::Ptr(new WinPaths());
	ret->SetTitle("Gestionnaire des chemins");
	auto table = sfg::Table::Create();
	//chemins
	table->Attach(Label::Create("Fichier Obj à charger"), sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::EXPAND, sfg::Table::FILL);
	table->Attach(ret->objLoadPath, sf::Rect<sf::Uint32>(0, 1, 4, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 0.f));
	table->Attach(Label::Create("Fichier Pgm à charger"), sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::EXPAND, sfg::Table::FILL);
	table->Attach(ret->pgmLoadPath, sf::Rect<sf::Uint32>(0, 3, 4, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 0.f));
	table->Attach(Label::Create("Répertoire de sauvegarde"), sf::Rect<sf::Uint32>(0, 4, 1, 1), sfg::Table::EXPAND, sfg::Table::FILL);
	table->Attach(ret->repoSavePath, sf::Rect<sf::Uint32>(0, 5, 4, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 0.f));
	table->Attach(Label::Create("Suffixe des fichiers Obj"), sf::Rect<sf::Uint32>(0, 6, 1, 1), sfg::Table::EXPAND, sfg::Table::FILL);
	table->Attach(ret->objSuffixe, sf::Rect<sf::Uint32>(0, 7, 4, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 0.f));
	table->Attach(Label::Create("Suffixe des fichiers Morse"), sf::Rect<sf::Uint32>(0, 8, 1, 1), sfg::Table::EXPAND, sfg::Table::FILL);
	table->Attach(ret->morseSuffixe, sf::Rect<sf::Uint32>(0, 9, 4, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 0.f));
	//zoom
	table->Attach(Label::Create("Multiplieur de zoom "), sf::Rect<sf::Uint32>(0, 10, 1, 1),  sfg::Table::EXPAND, sfg::Table::FILL);
	table->Attach(ret->zommMulti, sf::Rect<sf::Uint32>(2, 10, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);
	ret->Add(table);
	//add comportement
	ret->GetSignal(sfg::Window::OnCloseButton).Connect(bind([ret]() {
		ret->Show(false);
	}));
	return ret;
}
int WinPaths::getZoomMultiply() {
	return (int)zommMulti->GetValue();
}
std::string WinPaths::getObjLoad() {
	return objLoadPath->GetText();
}
std::string WinPaths::getPgmLoad() {
	return pgmLoadPath->GetText();
}
std::string WinPaths::getObjSave() {
	string ret = repo() + getObjName() + objSuffixe->GetText() + to_string(Controlleur2::get()->getNbrIterations() + 1) + ".obj";
	return ret;
}
std::string WinPaths::getMorseSave() {
	string ret = repo() + getObjName() + morseSuffixe->GetText() + to_string(Controlleur2::get()->getNbrIterations() + 1) + "";
	return ret;
}
std::string WinPaths::getObjName() {
	istringstream ss(pgmLoadPath->GetText());
	string token;
	while (std::getline(ss, token, '/')) {}
	istringstream ss2(token);
	while (std::getline(ss2, token, '\\')) {}
	//ici : token contient la dernière partie d'un chemin de répertoire : en gros un fichier
	istringstream ss3(token);
	std::getline(ss3, token, '.');//là on enlève l'extention
	return token;
}
#include <windows.h>
std::string WinPaths::repo() {
	std::string ret = repoSavePath->GetText();
	//ajouter un slash final s'il en manque
	if (ret.back() == '/'  || ret.back() == '\\' ) {}
	else {
		ret.push_back('/');
	}
	//créer le répertoire.
	CreateDirectory(ret.c_str(), NULL);
	return ret;
}
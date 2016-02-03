#pragma once

#include "SFGUI\Widgets.hpp"
#include "SFGUI\SFGUI.hpp"
#include <SFML\Graphics.hpp>
#include "Menu.hpp"
#include "OptionAffichage.hpp"
#include "ClusterList.hpp"
#include "WinPaths.hpp"
#include "Navigateur.hpp"

class Navigateur;
class ClusterList;

class WindowOpenGL
{
public:
	typedef std::shared_ptr<WindowOpenGL> Ptr;
	~WindowOpenGL() = default;
	static WindowOpenGL::Ptr Create();

	void run();
	void drawOpenGL(sf::Window &window);
	//paramétrage pour enregistrer le résultat aux formats obj ou morse après une étape de l'algo
	void setSave(bool obj = true, bool morse = false);
	void autoSave();
	bool saveMorse();
	bool saveObj();
private:
	bool quitter = false;
	Menu::Ptr gbl_menu;						//bin du menu
	OptionAffichage::Ptr gbl_OptAffichage;	//bin des options d'affichage
	std::shared_ptr<ClusterList> gbl_clusterList;	//liste des clusters collapsables
	WinPaths::Ptr win_paths;
	std::shared_ptr<Navigateur> gbl_navigateur;
	ObjList::Ptr gbl_objList;
	sfg::Window::Ptr win_menu;		//fenetre du menu
	sfg::Window::Ptr win_optAff;	//fenetre des options d'afffichage
	sfg::Window::Ptr win_NavPanel;	//fenetre des listes de cluster
	sfg::SFGUI sfgui;				//contexte sfgui
	sf::RenderWindow app;
	sfg::ScrolledWindow::Ptr navScroll;
protected:
	WindowOpenGL();
	sfg::Window::Ptr initMenuWindow();				//initialise la fenetre du menu
	sfg::Window::Ptr initOptionAffichageWindow();	//initialise la fenetre des options d'affichage
	sfg::Window::Ptr initNavPanel();		//initialise la fenetre des listes de cluster
	WinPaths::Ptr initPathWindow();		//initialise la fenetre de gstion des chemins de fichiers
	
	void afficherOptionAffichage();
	void afficherOptionChemins();
	void positionnerNavPanel();
	//attention : ne fonctionne pas : freeze l'écran si en-dehors des case de la gestion d'évènements
	std::string getUserString(std::string nameMessage, std::string description);

	//gestion de la sauvegarde
	bool b_saveObj, b_saveMorse;
};
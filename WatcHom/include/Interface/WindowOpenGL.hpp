#ifndef WINDOW_OPENGL_HPP
#define WINDOW_OPENGL_HPP

#include "SFGUI\Widgets.hpp"
#include "SFGUI\SFGUI.hpp"
#include <SFML\Graphics.hpp>
#include "Menu.hpp"
#include "OptionAffichage.hpp"
#include "ClusterList.hpp"

class WindowOpenGL
{
public:
	typedef std::shared_ptr<WindowOpenGL> Ptr;
	WindowOpenGL();
	~WindowOpenGL();

	void run();
	void drawOpenGL(sf::Window &window);
private:
	bool quitter = false;
	Menu::Ptr gbl_menu;						//bin du menu
	OptionAffichage::Ptr gbl_OptAffichage;	//bin des options d'affichage
	ClusterList::Ptr gbl_clusterList;		//liste des clusters collapsables
	sfg::Window::Ptr win_menu;		//fenetre du menu
	sfg::Window::Ptr win_optAff;	//fenetre des options d'afffichage
	sfg::Window::Ptr win_clusterList;//fenetre des listes de cluster
	sfg::SFGUI sfgui;				//context sfgui
	sf::VideoMode *video;
protected:
	sfg::Window::Ptr initMenuWindow();				//initialise la fenetre du menu
	sfg::Window::Ptr initOptionAffichageWindow();	//initialise la fenetre des options d'affichage
	sfg::Window::Ptr initClusterListWindow();		//initialise la fenetre des listes de cluster
	void afficherOptionAffichage();

};

#endif
#include "Interface\WindowOpenGL.hpp"

#include "SFML\Window.hpp"

#include "Engine\Controlleur2.hpp"

using namespace std;
using namespace sfg;
using namespace sf;

WindowOpenGL::WindowOpenGL()
{
	video = new VideoMode(800,600,32);
	win_menu = initMenuWindow();
	win_optAff = initOptionAffichageWindow();
	win_optAff->Show(false);
	//link du bouton "Parametres>Affichages" avec l'apparition de cette fenêtre
	gbl_menu->getButton("Affichage")->GetSignal(sfg::Button::OnMouseLeftPress).Connect(
		std::bind([=]() {
			afficherOptionAffichage();
		}
	));
}

WindowOpenGL::~WindowOpenGL()
{
}

//******************************
//fonction run.
//******************************

void WindowOpenGL::run() {
	//Desktop
	sfg::Desktop desktop;
	desktop.Add(win_menu);
	desktop.Add(win_optAff);
	//SFML
	RenderWindow app(*video, "WatcHom");
	app.setVerticalSyncEnabled(true);	//se synchroniser sur le rafraichissement de la carte
	app.resetGLStates();
	//boucle de rafraichissement
	while (!quitter) {
		Event event;
		while (app.pollEvent(event)) {
			desktop.HandleEvent(event);
			switch (event.type) {
			case Event::Closed :
				quitter = true;
				break;
			}
		}
		desktop.Update(1.0f);
		app.clear();
		drawOpenGL(app);
		sfgui.Display(app);
		app.display();
	}
}

//******************************
//fonctions d'affichage OpenGL
//******************************

void WindowOpenGL::drawOpenGL(sf::Window &window) {
	Controlleur2::get()->drawGL();
}

//******************************
//fonctions d'initialisation
//******************************

sfg::Window::Ptr WindowOpenGL::initMenuWindow() {
	auto window = sfg::Window::Create(0);
	gbl_menu = Menu::Create();
	window->Add(gbl_menu);
	window->SetAllocation(FloatRect(0, 0, 50, 20));
	//linker boutton quitter
	gbl_menu->getButton("Quitter")->GetSignal(sfg::Window::OnMouseLeftPress).Connect(
		std::bind([=]() {
			quitter = true;
	}));
	//linker boutton charger
	gbl_menu->getButton("Ouvrir Obj")->GetSignal(sfg::Window::OnMouseLeftPress).Connect(
		std::bind([=]() {
		Controlleur2::get()->loadObj("fertility100_11_V.obj");
	}));
	return window;
}
sfg::Window::Ptr WindowOpenGL::initOptionAffichageWindow() {
	auto windowOA = sfg::Window::Create(sfg::Window::Style::CLOSE | sfg::Window::Style::TOPLEVEL);
	windowOA->SetTitle("Options d'affichage");
	gbl_OptAffichage = OptionAffichage::Create();
	windowOA->Add(gbl_OptAffichage);
	//fermer fenetre des options d'affichage sur clic de la croix
	windowOA->GetSignal(sfg::Window::OnCloseButton).Connect(
		std::bind([=]() {
		win_optAff->Show(false);
		}
	));
	return windowOA;
}
//******************************
//fonctions de gestion diverses
//******************************

//affiche la fenetre 'OptionsAffichage'
void WindowOpenGL::afficherOptionAffichage() {
	float w = 400, h = 300;
	win_optAff->Show(true);
	win_optAff->SetAllocation(FloatRect((800 - w) / 2, (600 - h) / 2, w, h));
}
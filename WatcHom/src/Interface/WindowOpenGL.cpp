#include "Interface\WindowOpenGL.hpp"

#include "SFML\Window.hpp"

#include "Engine\Controlleur2.hpp"
#include "Interface\DialogueBox.hpp"

using namespace std;
using namespace sfg;
using namespace sf;

WindowOpenGL::WindowOpenGL() : app(sf::VideoMode(800, 600, 32), "WatcHom",sf::Style::Default, ContextSettings(8))
{
	win_menu = initMenuWindow();
	win_optAff = initOptionAffichageWindow();
	win_optAff->Show(false);
	win_paths = initPathWindow();
	win_paths->Show(false);
	win_NavPanel = initNavPanel();
}
WindowOpenGL::Ptr WindowOpenGL::Create() {
	WindowOpenGL::Ptr ret(new WindowOpenGL());
	ret->gbl_clusterList->setMainWindow(ret);
	ret->gbl_navigateur->setMainWindow(ret);
	return ret;
}

//******************************
//fonction run.
//******************************

void WindowOpenGL::run() {
	//Desktop
	sfg::Desktop desktop;
	desktop.Add(win_menu);
	desktop.Add(win_NavPanel);
	desktop.Add(win_optAff);
	desktop.Add(win_paths);
	//SFML
	positionnerNavPanel();
	app.setVerticalSyncEnabled(true);	//se synchroniser sur le rafraichissement de la carte
	app.resetGLStates();
	//boucle de rafraichissement
	static bool rightPressed = false;//gestion clics gauches prolong�s
	static bool midPressed = false;//gestion clics 3 prolong�s
	static int pmx;
	static int pmy;
	while (!quitter) {
		Event event;
		while (app.pollEvent(event)) {
			desktop.HandleEvent(event);
			switch (event.type) {
			case Event::Closed:
				quitter = true;
				break;
			case Event::Resized:
				Controlleur2::get()->setDimFenetre(event.size.width, event.size.height);
				positionnerNavPanel();
				break;
			case Event::MouseWheelMoved:
				if (event.mouseWheel.x > app.getSize().x - win_NavPanel->GetAllocation().width) {//si dsn le menu de navigation
					navScroll->GetVerticalAdjustment()->SetValue(navScroll->GetVerticalAdjustment()->GetValue() - event.mouseWheel.delta * 50.f);
					break;
				}
				Controlleur2::get()->setZoom(Controlleur2::get()->getZoom() - event.mouseWheel.delta*win_paths->getZoomMultiply()* 0.1f);
				break;
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Right) {
					rightPressed = true;
					Controlleur2::get()->startRotation(event.mouseButton.x,event.mouseButton.y);
				}
				else if (event.mouseButton.button == sf::Mouse::Middle) {
					midPressed = true;
					pmx = event.mouseButton.x;
					pmy = event.mouseButton.y;
				}
				break;
			case Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Right) {
					rightPressed = false;
					Controlleur2::get()->stopRotation();
				}
				else if (event.mouseButton.button == sf::Mouse::Middle) {
					midPressed = false;
				}
				break;
			case Event::MouseMoved:
				if (rightPressed) {
					Controlleur2::get()->rotation(event.mouseMove.x, event.mouseMove.y);
				}
				else if (midPressed) {
					Controlleur2::get()->translation((float)(event.mouseMove.x - pmx) / 10, (float)(pmy - event.mouseMove.y) / 10);//inverser axe y
					pmx = event.mouseMove.x;
					pmy = event.mouseMove.y;
				}
				break;
			case Event::KeyPressed:
				if (event.key.code == 68) {//+
					Controlleur2::get()->travelling((float) -win_paths->getZoomMultiply());
				}
				else if (event.key.code == 67) {//-
					Controlleur2::get()->travelling((float) +win_paths->getZoomMultiply());
				}
				break;
			default:
				break;
					
			}
			desktop.Update(1.0f);
			app.clear();
			drawOpenGL(app);
			sfgui.Display(app);
			app.display();
		}
	}
}

//******************************
//fonctions d'affichage OpenGL
//******************************

void WindowOpenGL::drawOpenGL(sf::Window &window) {
	glEnable(GL_DEPTH_TEST);	//SFGUI d�conne lorsque le depth buffer est utilis�
	Controlleur2::get()->drawGL();
	glDisable(GL_DEPTH_TEST);
}

//******************************
//fonctions d'initialisation
//******************************

sfg::Window::Ptr WindowOpenGL::initMenuWindow() {
	auto window = sfg::Window::Create(0);
	gbl_menu = Menu::Create();
	window->Add(gbl_menu);
	window->GetSignal(Widget::OnMouseEnter).Connect(bind([window]() {
		window->SetHierarchyLevel(0);
	}));
	//linker bouton quitter
	gbl_menu->getButton("Quitter")->GetSignal(sfg::Window::OnMouseLeftPress).Connect(
		std::bind([=]() {
			quitter = true;
	}));
	//linker bouton charger
	gbl_menu->getButton("Ouvrir Obj")->GetSignal(sfg::Window::OnMouseLeftPress).Connect(
		std::bind([=]() {
		Controlleur2::get()->loadObj(win_paths->getObjLoad());
		gbl_objList->majNav();
	}));
	//linker bouton pgm
	gbl_menu->getButton("Importer Pgm")->GetSignal(sfg::Window::OnMouseLeftPress).Connect(
		std::bind([=]() {
		if (Controlleur2::get()->loadPgm(win_paths->getPgmLoad()))
			gbl_clusterList->refresh();
	}));
	gbl_menu->getButton("Affichage")->GetSignal(sfg::Button::OnMouseLeftPress).Connect(std::bind(&WindowOpenGL::afficherOptionAffichage, this));
	gbl_menu->getButton("Chemins")->GetSignal(sfg::Button::OnMouseLeftPress).Connect(std::bind(&WindowOpenGL::afficherOptionChemins, this));
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
	gbl_OptAffichage->setColors(0, 255, 0, 0, 255);
	gbl_OptAffichage->setColors(1, 0, 255, 0, 255);
	gbl_OptAffichage->setColors(2, 0, 0, 255, 255);
	gbl_OptAffichage->setColors(3, 100, 0, 100, 255);
	return windowOA;
}
sfg::Window::Ptr WindowOpenGL::initNavPanel() {
	auto window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
	auto note = Notebook::Create();
	gbl_objList = ObjList::Create();
	gbl_clusterList = ClusterList::Create(gbl_objList);
	gbl_navigateur = Navigateur::Create();
	note->AppendPage(gbl_clusterList, Label::Create("Algo"));
	note->AppendPage(gbl_navigateur, Label::Create("Nav"));
	note->AppendPage(gbl_objList, Label::Create("Objs"));
	//window->Add(note);
	window->SetAllocation(FloatRect(200, 100, 50, 20));
	navScroll = sfg::ScrolledWindow::Create();
	navScroll->AddWithViewport(note);
	navScroll->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_NEVER | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
	window->Add(navScroll);
	return window;
}
WinPaths::Ptr WindowOpenGL::initPathWindow() {
	auto ret = WinPaths::Create();
	//position
	float w = 400, h = 300;
	ret->SetAllocation(FloatRect((app.getSize().x - w) / 2, (app.getSize().y - h) / 2, w, h));
	return ret;
}
//******************************
//fonctions de gestion diverses
//******************************

//affiche la fenetre 'OptionsAffichage'
void WindowOpenGL::afficherOptionAffichage() {
	float w = 400, h = 300;
	win_optAff->SetAllocation(FloatRect((app.getSize().x - w) / 2, (app.getSize().y - h) / 2, w, h));
	win_optAff->Show(true);
}
void WindowOpenGL::afficherOptionChemins() {
	float w = 400, h = 300;
	win_paths->SetAllocation(FloatRect((app.getSize().x - w) / 2, (app.getSize().y - h) / 2, w, h));
	win_paths->Show(true);
}
void WindowOpenGL::positionnerNavPanel() {
	sf::Vector2u appSize = app.getSize();
	//sf::Vector2f size(LARGEUR_NAV, (float)appSize.y);
	sf::Vector2f size(win_NavPanel->GetChild()->GetRequisition().x+20, (float)appSize.y);
	win_NavPanel->SetRequisition(size);
	win_NavPanel->SetAllocation(sf::FloatRect(appSize.x-size.x, 0, size.x, size.y));
}
//demander un message � l'utilisateur
std::string WindowOpenGL::getUserString(std::string nameMessage, std::string description) {
	//attention : ne fonctionne pas : freeze l'�cran si en-dehors des case de la gestion d'�v�nements
	return DialogueBox::GetString(app, sfgui, nameMessage, description);
}

//****************************Fonctions de sauvegarde*********************************
void WindowOpenGL::setSave(bool obj, bool morse) {
	b_saveObj = obj;
	b_saveMorse = morse;
}
void WindowOpenGL::autoSave() {
	if (b_saveObj) {
		saveObj();
	}if (b_saveMorse) {
		saveMorse();
	}
}
bool WindowOpenGL::saveMorse() {
	return Controlleur2::get()->saveMorse(win_paths->getMorseSave());
}
bool WindowOpenGL::saveObj() {
	return Controlleur2::get()->saveObj(win_paths->getObjSave());
}
#include "Interface\DialogueBox.hpp"
#include <SFML\Graphics.hpp>

using namespace std;
using namespace sfg;
using namespace sf;

DialogueBox::DialogueBox() : sfg::Window(Style::TOPLEVEL){}
/*fonction affichant une fenetre de dialogue et prenant le controle des évènements jusqu'à validation
 * @param app : la fenêtre d'affichage
 */
std::string DialogueBox::GetString(sf::RenderWindow &app, sfg::SFGUI &sfgui, std::string title, std::string message) {
	DialogueBox::Ptr dialogue = DialogueBox::Ptr(new DialogueBox());
	dialogue->SetTitle(title);
	dialogue->SetAllocation(sf::FloatRect(200,100,200,50));
	dialogue->SetZOrder(0);
	
	auto hb = Box::Create(Box::Orientation::VERTICAL);
	auto mess = Label::Create(message);
	auto resp = Entry::Create();
	auto butVal = Button::Create("Validate");
	auto butAnnul = Button::Create("Annuler");
	hb->Pack(mess);
	hb->Pack(resp);
	hb->Pack(butVal);
	hb->Pack(butAnnul);
	dialogue->Add(hb);
	//positionnement
	sf::Vector2u size = app.getSize();
	sf::Vector2f diagSize(300, 50);
	dialogue->SetRequisition(diagSize);
	dialogue->SetAllocation(sf::FloatRect((size.x-diagSize.x)/2, (size.y - diagSize.y)/3, diagSize.x, diagSize.y));
	//gestion événements bouttons
	bool quitter = false;
	butVal->GetSignal(sfg::Button::OnLeftClick).Connect(bind([&quitter,resp]() {
		if (resp->GetText().getSize() != 0) {
			quitter = true;
		}
	}));
	butAnnul->GetSignal(sfg::Button::OnLeftClick).Connect(bind([&quitter, resp]() {
		quitter = true;
		resp->SetText("");
	}));
	//new Desktop
	sfg::Desktop p_desk;
	p_desk.Add(dialogue);
	//boucle de rafraichissement
	while (!quitter) {
		Event event; 
		while (app.pollEvent(event)) {
			p_desk.HandleEvent(event);
			switch (event.type) {

			}
		}
		p_desk.Update(1.0f);
		app.clear();
		sfgui.Display(app);
		app.display();
	}
	return resp->GetText();
}
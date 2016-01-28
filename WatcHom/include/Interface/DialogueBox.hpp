#ifndef DIALOGUE_BOX_HPP
#define DIALOGUE_BOX_HPP

#include "SFGUI\Widgets.hpp"
#include "SFGUI\SFGUI.hpp"
#include "SFML\Window.hpp"
#include <string>

class DialogueBox : public sfg::Window
{
public:
	typedef std::shared_ptr<DialogueBox> Ptr;
	~DialogueBox()=default;
	static std::string GetString(sf::RenderWindow &app,sfg::SFGUI &sfgui, std::string title, std::string message);

private:
	DialogueBox();
	sfg::Entry::Ptr reponse;
};

#endif
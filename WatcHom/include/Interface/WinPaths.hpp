#pragma once

#include "SFGUI\Widgets.hpp"
#include "SFGUI\Window.hpp"
#include <string>

class WinPaths : public sfg::Window
{
public:
	typedef std::shared_ptr<WinPaths> Ptr;
	WinPaths();
	~WinPaths() = default;

	static WinPaths::Ptr Create();

	int getZoomMultiply();
	std::string getObjLoad();
	std::string getPgmLoad();
	std::string getObjSave();
	std::string getMorseSave();

private:
	sfg::Entry::Ptr objLoadPath, pgmLoadPath;
	sfg::Entry::Ptr repoSavePath;
	sfg::Entry::Ptr objSuffixe, morseSuffixe;
	sfg::SpinButton::Ptr zommMulti;

	//get the pgm file and extract the extension and rep path
	std::string WinPaths::getObjName();
	//vérifie que chemin ver répertoire. Le créé si n'existe pas, et rajoute un '/' final si necessaire
	std::string repo();
};

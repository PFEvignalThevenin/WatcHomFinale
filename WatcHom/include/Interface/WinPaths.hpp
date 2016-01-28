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

	void resetInc();
	void inc();

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
	int increment = 0;
};

#ifndef FILE_SAVABLE_HPP
#define FILE_SAVABLE_HPP

#include <string>
#include <exception>
#include <fstream>

class FileSavable
{
public:
	virtual void save(std::string const& path) const = 0;
	virtual void load(std::string const& path) = 0;
private:

protected:
};


/*
 *Classe d'exception :
 *Erreurs lancées par FileSavable
*/
class FileError : public std::exception
{
public:
	FileError(int code, std::string const& phrase = "File Error") throw() :m_phrase(phrase) {}

	virtual const char* what() const throw()
	{
		return m_phrase.c_str();
	}

	virtual ~FileError() throw() {}
	int getCode() { return code; }
private:
	int code;
	std::string m_phrase;            //Description de l'erreur
};

#endif
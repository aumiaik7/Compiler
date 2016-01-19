#ifndef TOKEN_H
#define TOKEN_H

#include <fstream>
#include <string>
#include "symbol.h" 

using namespace std;

class Token
{

	private:
	Symbol sym;
	string value;
	string fileName;
	
	public:
	
	void setSymbol(Symbol symb);
	Symbol getSymbol();
	void setValue(string value);
	string getValue();
	void setFile(string file);
	string getFile();
	
};

#endif

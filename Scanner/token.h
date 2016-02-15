#ifndef TOKEN_H
#define TOKEN_H

#include <fstream>
#include <string>
#include "symbol.h" 

using namespace std;

class Token
{

	private:
	//structure for storing attribute value and lexeme	
	struct attVal
	{
		attVal(int v, string l):value(v),lexeme(l){}
		int value;//
		string lexeme;
	};
	//used in getter functions
	attVal sValue;
	Symbol symbolName;
	
	
	public:
	//default constructor
	Token();
	//constructor for setting attributes for token
	Token(Symbol sym, int val, string lex );
	//getter functions
	Symbol getSymbol();
	int getValue(); 
	string getLexeme(); 
};

#endif

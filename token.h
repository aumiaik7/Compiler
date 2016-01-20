#ifndef TOKEN_H
#define TOKEN_H

#include <fstream>
#include <string>
#include "symbol.h" 

using namespace std;

class Token
{

	private:
	struct attVal
	{
		attVal(int v, string l):value(v),lexeme(l){}
		int value;//
		string lexeme;
	};
	attVal sValue;
	Symbol symbolName;
	
	
	public:
	Token();
	Token(Symbol sym, int val, string lex );
	Symbol getSymbol();
	int getValue() {return sValue.value;};
	string getLexeme() {return sValue.lexeme;}
};

#endif

#include "token.h"

Token::Token()
:symbolName(NONAME), sValue(attVal(-1,string("noname")))
{
}

Token::Token(Symbol s, int v, string l )
:symbolName(s),sValue(attVal(v,l))
{
}

Symbol Token::getSymbol()
{
	return symbolName;
}




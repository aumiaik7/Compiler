#include "token.h"

//default constructor creates NONAME token
Token::Token()
:symbolName(NONAME), sValue(attVal(-1,string("noname")))
{
}
//constructor for creating specific token object
Token::Token(Symbol s, int v, string l )
:symbolName(s),sValue(attVal(v,l))
{
}
//return token symbol
Symbol Token::getSymbol()
{
	return symbolName;
}
//return token value for numeral (-1 for other types of tokens) 
int Token::getValue()
{
	return sValue.value;
}
//return lexeme for id/keyword (empty string for other types of tokens)
string Token::getLexeme()
{
	return sValue.lexeme;
}




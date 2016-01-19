#include "token.h"


void Token::setSymbol(Symbol symb)
{
	sym = symb;
}

Symbol Token::getSymbol()
{
	return sym;
}

void Token::setFile(string file)
{
	fileName = file;
}

string Token::getFile()
{
	return fileName;
}

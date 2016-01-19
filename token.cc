#include "token.h"

void Token::setFile(string file)
{
	fileName = file;
}

string Token::getFile()
{
	return fileName;
}

void Token::setSymbol(Symbol symb)
{
	sym = symb;
}

Symbol Token::getSymbol()
{
	return sym;
}

void Token::setValue(string val)
{
	value = val;
}

string Token::getValue()
{
	return value;
}


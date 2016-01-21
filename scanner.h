#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "token.h"
#include "symboltable.h"
#include <vector>
using namespace std;

class Scanner
{

	private:
	ifstream &srcFile;
	Symboltable &symTablePtr;
	char ch,laCh;//char and look-ahead char
	bool isWhitespace(char lach);
	bool isSpecial(char lach);
	bool isNumEnd(char lach);	
	public:
	//Scanner();
	Scanner(ifstream &in,Symboltable st);
	Token nextToken();
	
		
};

#endif

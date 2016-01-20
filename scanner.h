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
	//Token token, nextTok;
	//int lineNo;
	//int colNo;
	//string value;
	public:
	//Scanner();
	Scanner(ifstream &in,Symboltable st);
	Token nextToken();
	
		
};

#endif

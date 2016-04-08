#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "token.h"
#include "symboltable.h"

using namespace std;

class Scanner
{

	private:
	ifstream &srcFile;//source file input stream
	Symboltable &symTablePtr;//Symbol table object
	char ch,laCh;//char and look-ahead char
	int idType;
	bool isWhitespace(char lach);//helps to detect white spaces
	bool isSpecial(char lach);//helps to detect special symbols
	bool isNumEnd(char lach);//helps to detect end of numerals
	int isValidNum(string s);//helps to detect wheather the numeral is good or bad	
	public:
	Scanner(ifstream &in,Symboltable &st);//constructor
	Token nextToken();//token generator
	void printSymTable();//print symbol table on terminal
	
		
};

#endif

#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "token.h"

using namespace std;

class Scanner
{

	private:
	ifstream srcFile;
	Token token, nextTok;
	int lineNo;
	int colNo;	
	string keywords[];
	public:
	Scanner(Token tok);
	void start();
	Token nextToken();
		
};

#endif

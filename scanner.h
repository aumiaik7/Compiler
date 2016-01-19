#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "token.h"
#include <vector>
using namespace std;

class Scanner
{

	private:
	ifstream srcFile;
	Token token, nextTok;
	int lineNo;
	int colNo;
	string value;
	public:
	Scanner(Token tok);
	void start();
	Token nextToken();
	
		
};

#endif

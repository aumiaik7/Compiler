#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "token.h"
#include "scanner.h"


// This is the maximum number of errors before the compiler bails out.
#define MAXERRORS 20
#define TERMINALS 49	
enum errorkind {ScanE, ParseE, ScopeE, TypeE};
using namespace std;

class Administration
{

	private:
	//input stream of source file
	//ifstream &srcFile;
	//out stream of output file	
	//ofstream &outFile;
	//Scanner object reference
	//Scanner &scanner;
	int lineNo;
	// report error only if correct line is true; prevents multiple/redundant error/line
	bool correctline;
	//count the number of errors.
	int errorCount;
	//get token from scan() function	
	Token nextTok;
	
	
	public:
	//flag for printing symbol table
	bool printSymbolTable;
	// Set up input and output files for scanning
	Administration();
	//Administration(ifstream &in, ofstream &out, Scanner &sc);
	
	~Administration() {}
	// Begin a new line of input
	void NewLine();
	// Error function for the phases
	void error(errorkind , Symbol, int);
	// call scanner from here
	int scan();
	//either valid or invalid token 
	bool validTok(Symbol sym);
	static const string terminals[TERMINALS];
		
};

#endif

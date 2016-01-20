#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include <string>
#include <fstream>
#include <iostream>
#include "token.h"
#include "scanner.h"
#include <vector>

// This is the maximum number of errors before the compiler bails out.
#define MAXERRORS 20
enum errorkind {ScanE, ParseE, ScopeE, TypeE};
using namespace std;

class Administration
{

	private:
	ifstream &srcFile;
	ofstream &outFile;
	
	Scanner &scanner;
	int lineNo;
	// report error only if correct line is true; prevents multiple/redundant error/line
	bool correctline;
	// Count the number of errors.
	int errorCount;
	Token nextTok;
	string value;
	
	public:
	
	// Set up input and output files for scanning
	Administration(ifstream &in, ofstream &out, Scanner &sc);
	~Administration() {}
	// Begin a new line of input
	void NewLine();
	// Error function for the phases
	void error(string text);
	// call scanner from here
	int scan();
	
		
};

#endif
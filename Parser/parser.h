#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "token.h"
#include "scanner.h"
#include "firstfollow.h"


// This is the maximum number of errors before the compiler bails out.
#define MAXERRORS 20
//enum errorkind {ScanE, ParseE, ScopeE, TypeE};
using namespace std;

class Parser
{

	private:
	//input stream of source file
	ifstream &srcFile;
	//out stream of output file	
	ofstream &outFile;
	//Scanner object reference
	Scanner &scanner;
	Firstfollow ff;
	int lineNo;
	// report error only if correct line is true; prevents multiple/redundant error/line
	bool correctline;
	//count the number of errors.
	int errorCount;
	//get token from scan() function	
	Token nextTok;
	//
	bool islookAheadTok;
	
	public:
	//flag for printing symbol table
	bool printSymbolTable;
	// Set up input and output files for scanning
	Parser(ifstream &in, ofstream &out, Scanner &sc);
	~Parser() {}
	//functions for implementing CFG
	void program();
	void block();
	void definitionPart();
	void definition();
	void statementPart();
	
	void constantDefinition();
	void variableDefinition();
	void procedureDefinition();

	void typeSymbol();
	void variableList();
	
	void statement();
	void readStatement();
	void writeStatement();
	void assignmentStatement();
	void ifStatement();
	void doStatement();
	void guardedCommandList();
	void guardedCommand();
	void expressionList();
	void variableAccessList();
	void variableAccess();
	void expression();
	void primaryExpression();
	void relationalOperator();
	void primaryOperator();
	void simpleExpression();
	void term();
	void addopTerm();
	void addingOperator();
	void multiplyingOperator();
	void factor();
	

	void constant();
	bool match(Symbol);
	// Begin a new line of input
	void NewLine();
	// Error function for the phases
	void error(string text);
	// call scanner from here
	int scan();
	//keep track whether token is current token or look ahead token
	void lookAheadToken();

	
		
};

#endif
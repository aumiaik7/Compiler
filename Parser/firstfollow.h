#ifndef FIRSTFOLLOW_H
#define FIRSTFOLLOW_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "token.h"



// This is the maximum number of errors before the compiler bails out.

//enum errorkind {ScanE, ParseE, ScopeE, TypeE};
using namespace std;

class Firstfollow
{

	private:
	
	
	public:
	//functions for checking wheather the look ahead token is in first of any non terminal 
	//First of definition	
	vector<Symbol> firstOfDefinition();
	//First of constantDefinition
	vector<Symbol> firstOfConstDef();
	//First of variableDefinition
	vector<Symbol> firstOfVariDef(); 
	//First of procedureDefinition	
	vector<Symbol> firstOfProcDef(); 
	//First of variableList	
	vector<Symbol> firstOfVariList();
	//First of statementPart	
	vector<Symbol> firstOfStatePart(); 
	//First of statement	
	vector<Symbol> firstOfStatement();
	//First of emptyStatement	
	vector<Symbol> firstOfEmptySt();
	//First of readStatement	
	vector<Symbol> firstOfReadSt();
	//First of writeStatement
	vector<Symbol> firstOfWriteSt();
	//First of procedureStatement
	vector<Symbol> firstOfProcSt();
	//First of assignmentStatement
	vector<Symbol> firstOfAssignSt();
	//First of variableAccessList
	vector<Symbol> firstOfVAList();	
	//First of indexSelector
	vector<Symbol> firstOfIndexSel();
	//First of primaryOperator
	vector<Symbol> firstOfPrimOp();
	//First of term
	vector<Symbol> firstOfTerm();
	//First of factor
	vector<Symbol> firstOfFactor();
	//First of constant
	vector<Symbol> firstOfConstant();
	//First of relationalOperator
	vector<Symbol> firstOfRelOp();
	//First of multiplyingOperator
	vector<Symbol> firstOfMultOp();
	//First of addingOperator
	vector<Symbol> firstOfAddOp();
	//First of ifStatement
	vector<Symbol> firstOfIfSt();
	//First of doStatement
	vector<Symbol> firstOfDoSt();
	//First if expressionList
	vector<Symbol> firstOfExpList();
	//First if guarderCommandList
	vector<Symbol> firstOfGCList();



	//functions for checking wheather the look ahead token is in follow of any non terminal 
	//follow of definitionPart
	vector<Symbol> followOfDefPart();
	//follow of statementPart	
	vector<Symbol> followOfStatePart();
	//follow of guardedCommand
	vector<Symbol> followOfGuardedCommand();
	//follow of variableAccessList
	vector<Symbol> followOfVaList();	
	//follow of variableAccessList
	vector<Symbol> followOfExpression();
		
		
		
};

vector<Symbol> operator+(vector<Symbol> , vector<Symbol> );
vector<Symbol> operator-(vector<Symbol> , Symbol );	
#endif

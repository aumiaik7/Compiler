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
	bool firstOfDefinition(Token tok);
	//First of constantDefinition
	bool firstOfConstDef(Token tok);
	//First of variableDefinition
	bool firstOfVariDef(Token tok); 
	//First of procedureDefinition	
	bool firstOfProcDef(Token tok); 
	//First of variableList	
	bool firstOfVariList(Token tok);
	//First of statementPart	
	bool firstOfStatePart(Token tok); 
	//First of statement	
	bool firstOfStatement(Token tok);
	//First of emptyStatement	
	bool firstOfEmptySt(Token tok);
	//First of readStatement	
	bool firstOfReadSt(Token tok);
	//First of writeStatement
	bool firstOfWriteSt(Token tok);
	//First of procedureStatement
	bool firstOfProcSt(Token tok);
	//First of assignmentStatement
	bool firstOfAssignSt(Token tok);
	//First of variableAccessList
	bool firstOfVAList(Token tok);	
	//First of indexSelector
	bool firstOfIndexSel(Token tok);
	//First of primaryOperator
	bool firstOfPrimOp(Token tok);
	//First of term
	bool firstOfTerm(Token tok);
	//First of factor
	bool firstOfFactor(Token tok);
	//First of constant
	bool firstOfConstant(Token tok);
	//First of relationalOperator
	bool firstOfRelOp(Token tok);
	//First of multiplyingOperator
	bool firstOfMultOp(Token tok);
	//First of addingOperator
	bool firstOfAddOp(Token tok);
	//First of ifStatement
	bool firstOfIfSt(Token tok);
	//First of doStatement
	bool firstOfDoSt(Token tok);
	//First if expressionList
	bool firstOfExpList(Token tok);



	//functions for checking wheather the look ahead token is in follow of any non terminal 
	//follow of definitionPart
	bool followOfDefPart(Token tok);
	//follow of statementPart	
	bool followOfStatePart(Token tok);
	//follow of guardedCommand
	bool followOfGuardedCommand(Token tok);
		
		
		
};

#endif

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
	//Symbol sym;	
	
	public:
	//functions for checking wheather the look ahead token is in first of any non terminal 
	//First of Definition	
	bool firstOfDefinition(Token tok);
	//First of ConstantDefinition
	bool firstOfConstDef(Token tok);
	//First of VariableDefinition
	bool firstOfVariDef(Token tok); 
	//First of ProcedureDefinition	
	bool firstOfProcDef(Token tok); 
	//First of VariableList	
	bool firstOfVariList(Token tok);
	//First of StatementPart	
	bool firstOfStatePart(Token tok); 
	//First of Statement	
	bool firstOfStatement(Token tok);
	//First of EmptyStatement	
	bool firstOfEmptySt(Token tok);
	//First of ReadStatement	
	bool firstOfReadSt(Token tok);
	//First of WriteStatement
	bool firstOfWriteSt(Token tok);
	//First of WriteStatement
	bool firstOfProcSt(Token tok);
	//First of AssignmentStatement
	bool firstOfAssignSt(Token tok);
	//First of VariableAccessList
	bool firstOfVAList(Token tok);	
	//First of IndexSelector
	bool firstOfIndexSel(Token tok);
	//First of PrimaryOperator
	bool firstOfPrimOp(Token tok);
	//First of Term
	bool firstOfTerm(Token tok);
	//First of Factor
	bool firstOfFactor(Token tok);
	//First of Constant
	bool firstOfConstant(Token tok);
	//First of RelationalOperator
	bool firstOfRelOp(Token tok);
	//First of MultiplyingOperator
	bool firstOfMultOp(Token tok);
	//First of AddingOperator
	bool firstOfAddOp(Token tok);
	//First of IfStatement
	bool firstOfIfSt(Token tok);
	//First of DoStatement
	bool firstOfDoSt(Token tok);
	//First if ExpressionList
	bool firstOfExpList(Token tok);

	





	


		


	//functions for checking wheather the look ahead token is in follow of any non terminal 
	bool followOfDefPart(Token tok);
	bool followOfStatePart(Token tok);
	bool followOfGuardedCommand(Token tok);
		
		
		
};

#endif

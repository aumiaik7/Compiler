#ifndef FIRSTFOLLOW_H
#define FIRSTFOLLOW_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "symbol.h"



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
	bool firstOfDefinition(Symbol sym);
	//First of ConstantDefinition
	bool firstOfConstDef(Symbol sym);
	//First of VariableDefinition
	bool firstOfVariDef(Symbol sym); 
	//First of ProcedureDefinition	
	bool firstOfProcDef(Symbol sym); 
	//First of VariableList	
	bool firstOfVariList(Symbol sym); 
	//First of Statement	
	bool firstOfStatement(Symbol sym);
	//First of EmptyStatement	
	bool firstOfEmptySt(Symbol sym);
	//First of ReadStatement	
	bool firstOfReadSt(Symbol sym);
	//First of WriteStatement
	bool firstOfWriteSt(Symbol sym);
	//First of WriteStatement
	bool firstOfProcSt(Symbol sym);
	//First of AssignmentStatement
	bool firstOfAssignSt(Symbol sym);
	//First of VariableAccessList
	bool firstOfVAList(Symbol sym);	
	//First of IndexSelector
	bool firstOfIndexSel(Symbol sym);
	//First of PrimaryOperator
	bool firstOfPrimOp(Symbol sym);
	//First of Term
	bool firstOfTerm(Symbol sym);
	//First of Factor
	bool firstOfFactor(Symbol sym);
	//First of Constant
	bool firstOfConstant(Symbol sym);
	//First of RelationalOperator
	bool firstOfRelOp(Symbol sym);
	//First of MultiplyingOperator
	bool firstOfMultOp(Symbol sym);
	//First of AddingOperator
	bool firstOfAddOp(Symbol sym);




	


		


	//functions for checking wheather the look ahead token is in follow of any non terminal 
	bool followOfDefPart(Symbol sym);
	bool followOfStatePart(Symbol sym);
		
		
		
};

#endif

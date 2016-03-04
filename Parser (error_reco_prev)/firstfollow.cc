#include "firstfollow.h"

//functions for checking wheather the look ahead token is in first of any non terminal 
//First of definition
bool Firstfollow::firstOfDefinition(Token tok)
{
	bool inConstDef = firstOfConstDef(tok);
	bool inVariDef = firstOfVariDef(tok);
	bool inProcDef = firstOfProcDef(tok);
	
	return  inConstDef || inVariDef || inProcDef;		
}

//First of constantDefinition
bool Firstfollow::firstOfConstDef(Token tok)
{
	if(tok.getSymbol() == CONST)
		return true;
	else
		return false;	
}

//First of variableDefinition
bool Firstfollow::firstOfVariDef(Token tok)
{
	if(tok.getSymbol() == INT || tok.getSymbol() == BOOL)
		return true;
	else
		return false;	
}

//First of procedureDefinition	
bool Firstfollow::firstOfProcDef(Token tok)
{
	if(tok.getSymbol() == PROC)
		return true;	
	else
		return false;
}

//First of variableList	
bool Firstfollow::firstOfVariList(Token tok)
{
	if(tok.getSymbol() == ID && tok.getIDtype() == 1)
		return true;
	else
		return false;	
}

//First of statementPart
bool Firstfollow::firstOfStatePart(Token tok)
{
	
}

//First of statement
bool Firstfollow::firstOfStatement(Token tok)
{
	bool inEmptySt = firstOfEmptySt(tok);
	bool inReadSt = firstOfReadSt(tok);
	bool inWriteSt = firstOfWriteSt(tok);
	bool inAssignSt = firstOfAssignSt(tok);
	bool inProcSt = firstOfProcSt(tok);
	bool inIfSt = firstOfIfSt(tok);
	bool inDoSt = firstOfDoSt(tok);
	
	return  inEmptySt || inReadSt || inWriteSt || inAssignSt || inProcSt || inIfSt || inDoSt ;		
	
}

//First of emptyStatement
bool Firstfollow::firstOfEmptySt(Token tok)
{
	if(tok.getSymbol() == SKIP)
		return true;
	else
		return false;		
}

//First of readStatement
bool Firstfollow::firstOfReadSt(Token tok)
{
	if(tok.getSymbol() == READ)
		return true;
	else
		return false;		
}

//First of writeStatement
bool Firstfollow::firstOfWriteSt(Token tok)
{
	if(tok.getSymbol() == WRITE)
		return true;
	else
		return false;		
}

//First of procedureStatement
bool Firstfollow::firstOfProcSt(Token tok)
{
	if(tok.getSymbol() == CALL)
		return true;
	else
		return false;	
}

//First of assignmentStatement
bool Firstfollow::firstOfAssignSt(Token tok)
{
	bool inVAList = firstOfVAList(tok);
	
	return 	inVAList;
}

//First of variableAccessList
bool Firstfollow::firstOfIfSt(Token tok)
{
	if(tok.getSymbol() == IF)
		return true;
	else
		return false;
}

//First of doStatement
bool Firstfollow::firstOfDoSt(Token tok)
{
	if(tok.getSymbol() == DO)
		return true;
	else
		return false;	
}

//First of variableAccessList
bool Firstfollow::firstOfVAList(Token tok)
{
	if(tok.getSymbol() == ID && tok.getIDtype() == 1)
		return true;
	else
		return false;	
}

//First of indexSelector
bool Firstfollow::firstOfIndexSel(Token tok)
{
	if(tok.getSymbol() == LEFTBRACKET)
		return true;
	else
		return false;	
}

//First of primaryOperator
bool Firstfollow::firstOfPrimOp(Token tok)
{
	if(tok.getSymbol() == AND || tok.getSymbol() == OR)
		return true;
	else
		return false;	
}

//First of term
bool Firstfollow::firstOfTerm(Token tok)
{
	bool inFactor = firstOfFactor(tok);
	
	return inFactor;
}

//First of factor
bool Firstfollow::firstOfFactor(Token tok)
{
	bool isName = false;
	bool inConst = firstOfConstant(tok);
	bool isLeftp = false;
	bool isTilt = false;
	if(tok.getSymbol() == ID)
		isName = true;
	else if(tok.getSymbol() == LEFTP)
		isLeftp = true;
	else if(tok.getSymbol() == NOT)
		isTilt = true;

	return isName || inConst || isLeftp || isTilt;

}

//First of constant
bool Firstfollow::firstOfConstant(Token tok)
{
	if(tok.getSymbol() == NUMERAL || tok.getSymbol() == TRUE || tok.getSymbol() == FALSE || (tok.getSymbol() == ID && 			tok.getIDtype() == 2))
		return true;
	else
		return false;
}

//First of relationalOperator
bool Firstfollow::firstOfRelOp(Token tok)
{
	if(tok.getSymbol() == LESST || tok.getSymbol() == EQUAL || tok.getSymbol() == GREATERT || tok.getSymbol() == LTE || 			tok.getSymbol() == GTE)
		return true;
	else
		return false;
}

//First of multiplyingOperator
bool Firstfollow::firstOfMultOp(Token tok)
{
	if(tok.getSymbol() == TIMES || tok.getSymbol() == DIV || tok.getSymbol() == MOD)
		return true;
	else
		return false;
}

//First of addingOperator
bool Firstfollow::firstOfAddOp(Token tok)
{
	if(tok.getSymbol() == PLUS || tok.getSymbol() == MINUS)
		return true;
	else
		return false;
}

//First if expressionList
bool Firstfollow::firstOfExpList(Token tok)
{
	if(tok.getSymbol() == MINUS || firstOfTerm(tok))
		return true;
	else
		return false;	
}


//follow//functions for checking wheather the look ahead token is in follow of any non terminal 
//follow of definitionPart
bool Firstfollow::followOfDefPart(Token tok)
{
	bool inStatement = firstOfStatement(tok);
	bool isEnd = false;
	
	
	if(tok.getSymbol() == END)
		isEnd = true;
	

	return inStatement || isEnd ;
}

//follow of statementPart
bool Firstfollow::followOfStatePart(Token tok)
{
	bool inGc = followOfGuardedCommand(tok);
	bool isEnd = false;
	
	if(tok.getSymbol() == END)
		isEnd = true;

	return inGc || isEnd ;
}

//follow of guardedCommand
bool Firstfollow::followOfGuardedCommand(Token tok)
{
	if(tok.getSymbol() == FI || tok.getSymbol() == OD || tok.getSymbol() == GC1)
		return true;
	else
		return false;
}

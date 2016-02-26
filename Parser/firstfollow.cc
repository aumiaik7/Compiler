#include "firstfollow.h"


/*bool Firstfollow::firstOfDefPart(Token tok)
{
	bool inDefinition = firstOfDefinition(tok);

	return 	inDefinition;
}
*/
bool Firstfollow::firstOfDefinition(Token tok)
{
	bool inConstDef = firstOfConstDef(tok);
	bool inVariDef = firstOfVariDef(tok);
	bool inProcDef = firstOfProcDef(tok);
	
	return  inConstDef || inVariDef || inProcDef;		
	
}

bool Firstfollow::firstOfConstDef(Token tok)
{
	if(tok.getSymbol() == CONST)
		return true;
	else
		return false;	
}


bool Firstfollow::firstOfVariDef(Token tok)
{
	if(tok.getSymbol() == INT || tok.getSymbol() == BOOL)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfProcDef(Token tok)
{
	if(tok.getSymbol() == PROC)
		return true;	
	else
		return false;
}

bool Firstfollow::firstOfVariList(Token tok)
{
	if(tok.getSymbol() == ID && tok.getIDtype() == 1)
		return true;
	else
		return false;	
}


bool Firstfollow::firstOfStatePart(Token tok)
{
	
}
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
bool Firstfollow::firstOfEmptySt(Token tok)
{
	if(tok.getSymbol() == SKIP)
		return true;
	else
		return false;		
}


bool Firstfollow::firstOfReadSt(Token tok)
{
	if(tok.getSymbol() == READ)
		return true;
	else
		return false;		
}

bool Firstfollow::firstOfWriteSt(Token tok)
{
	if(tok.getSymbol() == WRITE)
		return true;
	else
		return false;		
}
bool Firstfollow::firstOfProcSt(Token tok)
{
	if(tok.getSymbol() == CALL)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfAssignSt(Token tok)
{
	bool inVAList = firstOfVAList(tok);
	
	return 	inVAList;
}

bool Firstfollow::firstOfIfSt(Token tok)
{
	if(tok.getSymbol() == IF)
		return true;
	else
		return false;
}

bool Firstfollow::firstOfDoSt(Token tok)
{
	if(tok.getSymbol() == DO)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfVAList(Token tok)
{
	if(tok.getSymbol() == ID && tok.getIDtype() == 1)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfIndexSel(Token tok)
{
	if(tok.getSymbol() == LEFTBRACKET)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfPrimOp(Token tok)
{
	if(tok.getSymbol() == AND || tok.getSymbol() == OR)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfTerm(Token tok)
{
	bool inFactor = firstOfFactor(tok);
	
	return inFactor;
}

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


bool Firstfollow::firstOfConstant(Token tok)
{
	if(tok.getSymbol() == NUMERAL || tok.getSymbol() == TRUE || tok.getSymbol() == FALSE || (tok.getSymbol() == ID && tok.getIDtype() == 2))
		return true;
	else
		return false;
}

bool Firstfollow::firstOfRelOp(Token tok)
{
	if(tok.getSymbol() == LESST || tok.getSymbol() == EQUAL || tok.getSymbol() == GREATERT || tok.getSymbol() == LTE || tok.getSymbol() == GTE)
		return true;
	else
		return false;
}

bool Firstfollow::firstOfMultOp(Token tok)
{
	if(tok.getSymbol() == TIMES || tok.getSymbol() == DIV || tok.getSymbol() == MOD)
		return true;
	else
		return false;
}

bool Firstfollow::firstOfAddOp(Token tok)
{
	if(tok.getSymbol() == PLUS || tok.getSymbol() == MINUS)
		return true;
	else
		return false;
}




//follow
bool Firstfollow::followOfDefPart(Token tok)
{
	bool inStatement = firstOfEmptySt(tok);
	bool isEnd = false;
	
	
	if(tok.getSymbol() == END)
		isEnd = true;
	

	return inStatement || isEnd ;
}


bool Firstfollow::followOfStatePart(Token tok)
{
	if(tok.getSymbol() == END)
		return true;
	else
		return false;
}
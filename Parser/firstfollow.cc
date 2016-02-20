#include "firstfollow.h"


/*bool Firstfollow::firstOfDefPart(Symbol sym)
{
	bool inDefinition = firstOfDefinition(sym);

	return 	inDefinition;
}
*/
bool Firstfollow::firstOfDefinition(Symbol sym)
{
	bool inConstDef = firstOfConstDef(sym);
	bool inVariDef = firstOfVariDef(sym);
	bool inProcDef = firstOfProcDef(sym);
	
	return  inConstDef || inVariDef || inProcDef;		
	
}

bool Firstfollow::firstOfConstDef(Symbol sym)
{
	if(sym == CONST)
		return true;
	else
		return false;	
}


bool Firstfollow::firstOfVariDef(Symbol sym)
{
	if(sym == INT || sym == BOOL)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfProcDef(Symbol sym)
{
	if(sym == PROC)
		return true;	
	else
		return false;
}

bool Firstfollow::firstOfVariList(Symbol sym)
{
	if(sym == ID)
		return true;
	else
		return false;	
}


bool Firstfollow::firstOfStatePart(Symbol sym)
{
	
}
bool Firstfollow::firstOfStatement(Symbol sym)
{
	bool inEmptySt = firstOfEmptySt(sym);
	bool inReadSt = firstOfReadSt(sym);
	bool inWriteSt = firstOfWriteSt(sym);
	bool inAssignSt = firstOfAssignSt(sym);
	bool inProcSt = firstOfProcSt(sym);
	bool inIfSt = firstOfIfSt(sym);
	bool inDoSt = firstOfDoSt(sym);
	
	
	return  inEmptySt || inReadSt || inWriteSt || inAssignSt || inProcSt || inIfSt || inDoSt ;		
	
}
bool Firstfollow::firstOfEmptySt(Symbol sym)
{
	if(sym == SKIP)
		return true;
	else
		return false;		
}


bool Firstfollow::firstOfReadSt(Symbol sym)
{
	if(sym == READ)
		return true;
	else
		return false;		
}

bool Firstfollow::firstOfWriteSt(Symbol sym)
{
	if(sym == WRITE)
		return true;
	else
		return false;		
}
bool Firstfollow::firstOfProcSt(Symbol sym)
{
	if(sym == CALL)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfAssignSt(Symbol sym)
{
	bool inVAList = firstOfVAList(sym);
	
	return 	inVAList;
}

bool Firstfollow::firstOfIfSt(Symbol sym)
{
	if(sym == IF)
		return true;
	else
		return false;
}

bool Firstfollow::firstOfDoSt(Symbol sym)
{
	if(sym == DO)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfVAList(Symbol sym)
{
	if(sym == ID)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfIndexSel(Symbol sym)
{
	if(sym == LEFTBRACKET)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfPrimOp(Symbol sym)
{
	if(sym == AND || sym == OR)
		return true;
	else
		return false;	
}

bool Firstfollow::firstOfTerm(Symbol sym)
{
	bool inFactor = firstOfFactor(sym);
	
	return inFactor;
}

bool Firstfollow::firstOfFactor(Symbol sym)
{
	bool isName = false;
	bool inConst = firstOfConstant(sym);
	bool isLeftp = false;
	bool isTilt = false;
	if(sym == ID)
		isName = true;
	else if(sym == LEFTP)
		isLeftp = true;
	else if(sym == NOT)
		isTilt = true;

	return isName || inConst || isLeftp || isTilt;

	
}


bool Firstfollow::firstOfConstant(Symbol sym)
{
	if(sym == NUMERAL || sym == TRUE || sym == FALSE || sym == ID)
		return true;
	else
		return false;
}

bool Firstfollow::firstOfRelOp(Symbol sym)
{
	if(sym == LESST || sym == EQUAL || sym == GREATERT || sym == LTE || sym == GTE)
		return true;
	else
		return false;
}

bool Firstfollow::firstOfMultOp(Symbol sym)
{
	if(sym == TIMES || sym == DIV || sym == MOD)
		return true;
	else
		return false;
}

bool Firstfollow::firstOfAddOp(Symbol sym)
{
	if(sym == PLUS || sym == MINUS)
		return true;
	else
		return false;
}




//follow
bool Firstfollow::followOfDefPart(Symbol sym)
{
	bool inStatement = firstOfEmptySt(sym);
	bool isEnd = false;
	
	if(sym == END)
		isEnd = true;

	return inStatement || isEnd;
}


bool Firstfollow::followOfStatePart(Symbol sym)
{
	if(sym == END)
		return true;
	else
		return false;
}

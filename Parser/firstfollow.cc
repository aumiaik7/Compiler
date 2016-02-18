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


bool Firstfollow::firstOfStatement(Symbol sym)
{
	bool inEmptySt = firstOfEmptySt(sym);
	//bool inReadSt = firstOfReadSt(sym);
	//bool inWriteSt = firstOfWriteSt(sym);
	bool inAssignSt = firstOfAssignSt(sym);
	bool inProcSt = firstOfProcSt(sym);
	//bool inIfSt = firstOfIfSt(sym);
	//bool inDoSt = firstOfDoSt(sym);
	
	return  inEmptySt || inAssignSt || inProcSt ;		
	
}
bool Firstfollow::firstOfEmptySt(Symbol sym)
{
	if(sym == SKIP)
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

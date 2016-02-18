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

#include "blocktable.h"

BlockTable::BlockTable()
:myBlock(MAXBLOCKS,table(MAXDEFINITIONS)),blockLevel(0),def(0)
{
	
	for(int i = 0; i < MAXBLOCKS ; i++)
		for(int j = 0; j < MAXDEFINITIONS; j++)
			myBlock[i][j].id = -1;
}

bool BlockTable::search(int idToLook)
{
	for(int i = 0; i <= def; i++)
	{
		if(myBlock[blockLevel][i].id == idToLook)
			return true;
		else if(myBlock[blockLevel][i].id == -1)
			return false;
	}
	return false;
}

bool BlockTable::define(int nID, PL_Kind nKind, PL_Type nType, int nSize, int nValue)
{
	if(!search(nID))
	{
		myBlock[blockLevel][def].id = nID;
		myBlock[blockLevel][def].kind = nKind;
		myBlock[blockLevel][def].type = nType;
		myBlock[blockLevel][def].value = nValue;

		def++;

		return true;
	}
	else
		return false;

}
TableEntry BlockTable::find(int idToLook, bool& error)
{

}

#include "blocktable.h"

BlockTable::BlockTable()
:myBlock(MAXBLOCKS,table(MAXDEFINITIONS)),blockLevel(-1),def(0)
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
		myBlock[blockLevel][def].size = nSize;
		myBlock[blockLevel][def].value = nValue;

		def++;

		return true;
	}
	else
		return false;

}

TableEntry BlockTable::find(int idToLook, bool &err)
{
	for(int i = 0; i <= blockLevel; i++)
		for(int j = 0; j < MAXDEFINITIONS; j++)
		{
			if(myBlock[i][j].id == -1)
				break;
			else if(myBlock[i][j].id == idToLook)
			{
				err = false;
				return myBlock[i][j];
			}
		}

	err = true;
	TableEntry dummyEntry;
	dummyEntry.id = -1;
	//dummyEntry.kind = UNDEFINED;
	return dummyEntry;

}

void BlockTable::newBlock()
{
	def = 0;
	blockLevel++;
	if(blockLevel > 9)
	{
		cerr<<"Symbol table is full. Program exits"<<endl;
		exit(0);
	}
}

void BlockTable::setArraySize(int defPosition, int size)
{
	for(int i = defPosition; i < def; i++)
	{
		myBlock[blockLevel][i].size = size;
	}
}

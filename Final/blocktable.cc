#include "blocktable.h"

BlockTable::BlockTable()
:myBlock(MAXBLOCKS,table(MAXDEFINITIONS)),blockLevel(-1),def(0)
{
	
	for(int i = 0; i < MAXBLOCKS ; i++)
		for(int j = 0; j < MAXDEFINITIONS; j++)
			myBlock[i][j].id = -1;
}

//returns true if the id we are looking for is found
//in the current block. false otherwise
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

//returns true if the current block doesn't contain
//an object with our current id. Otherwise it returns
//false indicating ambiguous name i.e there are more
//than one definition for the specific id
bool BlockTable::define(int nID, PL_Kind nKind, PL_Type nType, int nSize, int nValue)
{
	if(!search(nID))
	{
		//define attributes
		myBlock[blockLevel][def].id = nID;
		myBlock[blockLevel][def].kind = nKind;
		myBlock[blockLevel][def].type = nType;
		myBlock[blockLevel][def].size = nSize;
		myBlock[blockLevel][def].value = nValue;
		myBlock[blockLevel][def].rbl = blockLevel;
		myBlock[blockLevel][def].disp = def + 3;

		def++;

		return true;
	}
	else
		return false;

}

//error is false if the id we are looking for is in
//the block table. Search all blocks in the block table
//error is true if not found in the blocktable
TableEntry BlockTable::find(int idToLook, bool &err)
{
	for(int i = blockLevel; i >= 0; i--)
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

	//not found
	err = true;
	TableEntry dummyEntry;
	dummyEntry.id = -1;
	return dummyEntry;

}
//new block in stack
bool BlockTable::newBlock()
{
	def = 0;
	blockLevel++;
	if(blockLevel > 9)
	{
		return false;
		//cerr<<"Symbol table is full. Program exits"<<endl;
		//exit(0);
	}
	return true;
}
//setSize for Arrays
void BlockTable::setArraySize(int defPosition, int size)
{
	for(int i = defPosition; i < def; i++)
	{
		myBlock[blockLevel][i].size = size;
	}
}

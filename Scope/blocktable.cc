#include "blockTable.h"

BlockTable::BlockTable()
:myBlock(MAXBLOCKS,table(MAXDEFINITIONS))
{
	
	TableEntry entry;
	entry.id = 1;
	entry.type = INTEGRAL;

	myBlock[0].push_back(entry);
}

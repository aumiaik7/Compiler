#include "blocktable.h"

BlockTable::BlockTable()
:myBlock(MAXBLOCKS,table(MAXDEFINITIONS))
{
	
	myBlock[0][0].id = 1;
	myBlock[0][0].type = INTEGRAL;
	cout<<myBlock[0][0].id<<endl;
}


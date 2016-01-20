#include "symboltable.h"

Symboltable::Symboltable()
:occupied(0), htable(SYMTABLESIZE)
{
}

bool Symboltable::full()
{
	return occupied == SYMTABLESIZE;
}

int Symboltable::getOC()
{
	return occupied;
}

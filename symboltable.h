#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <vector>
#include <iostream>
#include "token.h"
#include "symbol.h"
#define SYMTABLESIZE 307

using namespace std;

class Symboltable
{

	private:
	vector<Token> htable;
	int occupied;// number of cells occupied
	int hashfn(string lex); // hash function
	int posFlag;
	
	
	public:
	// create a table with SYMTABLESIZE 307 entries.
	Symboltable();
	// destructor
	~Symboltable(){}
	// return the position of the lexeme in the table if found, -1 if not.
	int search(string lex);
	// Insert a token with lexeme s in the symbol table; if already in the table return its
	// location (index); otherwise create a token of appropriate type; insert the token and
	// return its location; modified in the later phases
	int insert(string s);
	// Return true if the table is full; false otherwise
	bool full();
	// Return the number of occupied cells (used in load factor calculartion.
	int getOC();
};

#endif



#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H

#include <vector>
#include <iostream>
#include <string>
#include "type.h"
#define MAXBLOCKS 10
#define MAXDEFINITIONS 20

using namespace std;

typedef struct
{
	int id;
	PL_Kind kind;
	PL_Type type;
	int size;
	int value;
}TableEntry;

class BlockTable
{

	private:
	//keeps track of the current block
	int blockLevel;
	int def;
	//vector of block table	
	typedef vector<TableEntry> table;
	typedef vector<table> block;
	bool error;
	block myBlock;
	//typedef vector< vector<TableEntry> > blockTable(MAXBLOCKS,vector<TableEntry>(MAXDEFINITIONS));
	

	public:
	//default constructor
	BlockTable();
	~BlockTable(){};
	//return true if the id we are looking for is found
	//in the current block. false otherwise
	bool search(int);
	//returns true if the current block doesn't contain 
	//an object with our current id. Otherwise it returns
	//false indicating ambiguous name i.e there are more 
	//than one definition for the specific id 
	bool define(int, PL_Kind, PL_Type, int, int);
	//error is false if the id we are looking for is in
	//the block table. Search all blocks in the block table
	//error is true if not found in the blocktable
	TableEntry find(int, bool&);

		
};

#endif



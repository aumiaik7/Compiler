#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H

#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include "type.h"
#define MAXBLOCKS 10
#define MAXDEFINITIONS 30

using namespace std;

typedef struct
{
	int id;
	PL_Kind kind;
	PL_Type type;
	int size;
	int value;
	int rbl; //relative block level
	int disp; // displacement
}TableEntry;

class BlockTable
{

	private:
	//keeps track of the current block
	int blockLevel;
	//vector of block table	
	typedef vector<TableEntry> table;
	typedef vector<table> block;
	//vector of vector
	block myBlock;
	

	public:
	//default constructor
	BlockTable();
	~BlockTable(){};
	//error is false if a name is found in block table
	//true otherwise
	bool error;
	//returns true if the id we are looking for is found
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
	TableEntry find(int, bool &);
	//new block
	bool newBlock();
	//setSize for Arrays
	void setArraySize(int,int);
	//index for name definitions
	int def;
	//set start label
	void setStartLabel(int);
		
};

#endif



#ifndef FIRSTFOLLOW_H
#define FIRSTFOLLOW_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "symbol.h"



// This is the maximum number of errors before the compiler bails out.

//enum errorkind {ScanE, ParseE, ScopeE, TypeE};
using namespace std;

class Firstfollow
{

	private:
	//Symbol sym;	
	
	public:
	//functions for checking wheather the look ahead token is in first of any non terminal 
	bool firstOfDefinition(Symbol sym);
	bool firstOfConstDef(Symbol sym);
	bool firstOfVariDef(Symbol sym); 
	bool firstOfProcDef(Symbol sym); 
	bool firstOfVariList(Symbol sym); 
		
		
};

#endif

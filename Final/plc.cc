#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "symbol.h"	
#include "scanner.h"
#include "administration.h"
#include "parser.h"
#include "symboltable.h"
#include "blocktable.h"

int main(int argc, char *argv[]) {
    
	//source file object	
	ifstream srcFile;
	//output file object
	ofstream outFile;
	//print symbol table upon user input
	bool printSymTab = false;
	
	//must give at list 2 arguments while executing program .
	if(argc < 2)
	{
		cout<<"Please specify the file name as argument"<<endl;
		exit(0);
	}
	
	//symbol table object
	Symboltable st;
	//Scanner object
	Scanner scanner(srcFile,st);
	
	srcFile.open(argv[1]);
	outFile.open("Code");
	BlockTable bTable;
	//Parser calss object	
	Parser parser(srcFile, outFile, scanner, bTable);
	//Strat Parsing  
	parser.program(ENDOFFILE);	
	
	srcFile.close();

}

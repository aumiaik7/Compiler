#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include "symbol.h"	
#include "scanner.h"
#include "administration.h"
#include "parser.h"
#include "symboltable.h"
#include "blocktable.h"
#include "Assembler.h"
#include "interp.h"


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
		cerr<<"Please specify the file name as argument"<<endl;
		return 1;
	}
	
	//symbol table object
	Symboltable st;
	//Scanner object
	Scanner scanner(srcFile,st);
	
	srcFile.open(argv[1]);
	if (!srcFile)
	{
		 cerr << "Input file " << argv[1] << " could not be opened" << endl;
		 return 1;
	}
	//outFile.open("Code");

	char intermediate[1024];
	strcpy(intermediate,"PLFile_");
	strcat(intermediate, argv[1]);
	strcat(intermediate, ".asm");
	ofstream assemblyOutfile;

	assemblyOutfile.open(intermediate, ios::out);
	 if (!assemblyOutfile)
	 {
		 cerr << "Intermediate file " << intermediate<< " could not be written"
		      << endl;
		 return 1;
	 }
	BlockTable bTable;
	//Parser calss object	
	Parser parser(srcFile, assemblyOutfile, scanner, bTable);
	//Strat Parsing  
	int errorCount = parser.parse();
	
	srcFile.close();
	assemblyOutfile.close();

	//no errors so perfor first and second pass in Assembler and execute machine
	//code in interpreter
	if (errorCount == 0)
	{
		ifstream assemblyInfile;
		assemblyInfile.open(intermediate, ios::in);
		if (!assemblyInfile)
		{
		    cerr << "Intermediate file " << intermediate<< " could not be read"<< endl;
		    return 1;
		}
		//machine code with resolved forward references are always stored in this file
		ofstream outputfile("machineCode");
		if (!outputfile)
		{
		    cerr << "Output file " << argv[3] << " could not be opened"<< endl;
		    return 1;
		}

		Assembler assembler(assemblyInfile, outputfile);
		assembler.firstPass();
		assemblyInfile.seekg(0);
		assembler.secondPass();
		assemblyInfile.close();
		outputfile.close();
		Interpreter interpreter("machineCode", false);
	}


	//srcFile.close();

}

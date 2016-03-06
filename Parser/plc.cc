#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "symbol.h"	
#include "scanner.h"
#include "administration.h"
#include "parser.h"
#include "symboltable.h"

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
	
	//open source file with object
	//srcFile.open(argv[1]);
	//srcFile.open("test6");
	//if(!srcFile.is_open())
	//{
	//	cout << "Error opening Source file" << endl;
	//	exit(0);
	//}
	//open output file with object
	//outFile.open("outFile");
	//outFile.open("ParserOutFile");
	//if(!outFile.is_open())
	//{
	//	cout << "Error opening output file" << endl;
	//	exit(0);
	//}
	//symbol table object
	Symboltable st;
	//Scanner object
	Scanner scanner(srcFile,st);
	//Administrator Object (Starts the compiler)
	//Administration compiler(srcFile, outFile, scanner);

	
	//enable print Symboltable when user gives "SymbolTable" as third argument	
	/*if(argc == 3 && string(argv[2]) == "SymbolTable")
	{
				
		compiler.printSymbolTable = true;
	}
	*/
	//scanning start here
	//int status = compiler.scan();

//	if(status == 0)
	//{
		//cout<<"\n<<<Scanning Result>>>\nSuccessfully sccaned. Token stored in outFile"<<endl;
		//Parser Object (Starts the Parser)
		srcFile.open(argv[1]);
		outFile.open("ParserOutFile");
		Parser parser(srcFile, outFile, scanner);
		parser.program(ENDOFFILE);	
		
	//}
	//else
//	{
//		cout<<"\n<<<Scanning Result>>>\nError occured!"<<endl;
//		cout<<"Program contains lexical error. Please correct those to perform syntax analysis"<<endl;
//	}



				
    	
}

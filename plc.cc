#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "scanner.h"
#include "administration.h"
#include "symboltable.h"

int main(int argc, char *argv[]) {
    
	ifstream srcFile;
	ofstream outFile;
	bool printSymTab = false;
	
	char ch;
	
	if(argc < 2)
	{
		cout<<"Please specify the file name as argument"<<endl;
		exit(0);
	}
	
	srcFile.open(argv[1]);
	if(!srcFile.is_open())
	{
		cout << "Error opening Source file" << endl;
		exit(0);
	}
	outFile.open("output.txt");
	if(!outFile.is_open())
	{
		cout << "Error opening output file" << endl;
		exit(0);
	}
	
	Symboltable st;
	//Scanner object
	Scanner scanner(srcFile,st);
	//Administrator Object (Starts the compiler)
	Administration compiler(srcFile, outFile, scanner);
	if(argc == 3 && string(argv[2]) == "SymbolTable")
	{
				
		compiler.printSymbolTable = true;
	}
	
	
	int status = compiler.scan();

	if(status == 0)
		cout<<"Successfully sccaned"<<endl;
	else
		cout<<"Error occured. Check output.txt file"<<endl;

	
	
		
			
    	
}

#include <iostream>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include "scanner.h"
#include "administration.h"
#include "symboltable.h"

int main(int argc, char *argv[]) {
    
	ifstream srcFile;
	ofstream outFile;
	char ch;
	
	if(argc != 2)
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

	int status = compiler.scan();

	
		
			
    	
}

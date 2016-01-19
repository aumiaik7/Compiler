#include <iostream>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include "token.h"
#include "scanner.h"


int main(int argc, char *argv[]) {
    
	ifstream srcFile;
	char ch;
	
	if(argc != 2)
	{
		cout<<"Please specify the file name as argument"<<endl;
		exit(0);
	}

	
	Token token;
	token.setFile(argv[1]);
	Scanner scanner(token);
	scanner.start();  
	
	/*while((ch = token.srcFile.get()) != EOF)
	{
		cout << ch;
	}
	
	srcFile.close();

	*/
		
			
    	
}

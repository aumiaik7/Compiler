#include <iostream>
#include <fstream>
#include <cstdlib>
#include "token.h"


int main(int argc, char *argv[]) {
    
	ifstream srcFile;
	
	if(argc != 2)
	{
		cout<<"Please specify the file name as argument"<<endl;
		exit(0);
	}

	srcFile.open(argv[1]);
	if(!srcFile.is_open())
	{
		cout << "Error opening file" << endl;
	
	}
		
			
    	/*switch (argc) {
		case 1: // No parameters, use stdin
		        // printf("NO argument provided\n");
		        filePtr = stdin;
		        break;

		case 2: // One parameter, use .lan file supplied	
		        if ( (filePtr = fopen(strcat(argv[1], ".lan"), "r")) == NULL ) {
		                printf("Cannot open input file.\n");
		                exit(1);
		        }
		        break;

		default:
		        printf("Syntax: scanner [file] (.lan is implicit)\n");
		        exit(0);
    	}*/
}

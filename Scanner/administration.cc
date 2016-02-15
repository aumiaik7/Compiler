#include "administration.h"

//constructor
Administration::Administration(ifstream &in, ofstream &out, Scanner &sc)
:srcFile(in),outFile(out),scanner(sc),printSymbolTable(false)
{
}

int Administration::scan()
{
		
	//initializing line no and error count		
	lineNo = 1;
	errorCount = 0;
	//continues to scan until reaches max error or end of file
	outFile<<"Line "<<lineNo<<":";
	while(1)
	{
		//exract token from source file untill the scanner reaches end of file
		nextTok = scanner.nextToken();
		//stop when the symbol is ENDOFFILE 
		if(nextTok.getSymbol() == ENDOFFILE)
		{
			outFile<<"<"<<nextTok.getSymbol()<< ","<<nextTok.getValue()<<","<<nextTok.getLexeme()<<"> "; 
			break;
		}
		//increment newline counter
		else if(nextTok.getSymbol() == NEWLINE)
			NewLine();
		//gets valid token
		else if(validTok(nextTok.getSymbol()))
		{
			outFile<<"<"<<nextTok.getSymbol()<< ","<<nextTok.getValue()<<","<<nextTok.getLexeme()<<"> "; 
		}
		//invalid  token		
		else
		{
			//bail out condition
			if(errorCount >= MAXERRORS)
			{
				cout<<"Too many errors. Bailing out!!\n";
				outFile<<"Too many errors. Bailed out!!";
				srcFile.close();
				outFile.close();
				exit(0);
			}
			//shows corresponding error message
			switch(nextTok.getSymbol())
			{
				
				case NONAME:
						
						break;	
				case BADNAME:	
						outFile<<"ERROR: Bad name occured at line "<<lineNo;
						errorCount++;
						break;
				case BADNUMERAL:
						outFile<<"ERROR: Bad numeral detected at line "<<lineNo;
						errorCount++;
						break;
				case BADSYMBOL:
						outFile<<"ERROR: Bad symbol found at line "<<lineNo;
						errorCount++;
						break;
				default:
					outFile<<"ERROR: Bad character found at line "<<lineNo;
						
			}	
		}
	}
	//if user wants then shows the symbol table on terminal
	if(printSymbolTable)	
		scanner.printSymTable();
	srcFile.close();
	outFile.close();

	return errorCount;
}
//new line counter
void Administration::NewLine()
{
	lineNo++;
	outFile<<"\n"<<"Line "<<lineNo<<":";	
	
}

//checks wheather the token is valid or not. if valid then returns true
bool Administration::validTok(Symbol sym)
{
	if(sym == NONAME || sym == BADNAME || sym == BADNUMERAL || sym == BADSYMBOL)
		return false;
	else 
		return true;
}	
//write error messages to output file	
void Administration::error(string text)
{
	
}	


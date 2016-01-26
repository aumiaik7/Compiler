#include "administration.h"

Administration::Administration(ifstream &in, ofstream &out, Scanner &sc)
:srcFile(in),outFile(out),scanner(sc),printSymbolTable(false)
{
}

int Administration::scan()
{
		
	//initializing line no		
	lineNo = 1;
	errorCount = 0;
	//continues to scan until reaches max error or end of file
	outFile<<"Line "<<lineNo<<":";
	while(1)
	{
		//value = "";
		nextTok = scanner.nextToken();
		if(nextTok.getSymbol() == ENDOFFILE)
			break;
		//when gets line increments the line counter
		else if(nextTok.getSymbol() == NEWLINE)
			NewLine();
		//gets valid token
		else if(validTok(nextTok.getSymbol()))
		{
			outFile<<"<"<<nextTok.getSymbol()<< ","<<nextTok.getValue()<<","<<nextTok.getLexeme()<<"> "; 
		}
		else
		{
			if(errorCount > MAXERRORS)
			{
				cout<<"Too many errors. Bailing out!!";
				outFile<<"Too many errors. Bailed out!!";
				exit(0);
			}
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
	if(printSymbolTable)	
		scanner.printSymTable();
	srcFile.close();
	outFile.close();

	return errorCount;
}

void Administration::NewLine()
{
	lineNo++;
	outFile<<"\n"<<"Line "<<lineNo<<":";	
	/*if(nextTok.getSymbol() == BADSYMBOL)
	{
		outFile.put("Error");
	}*/
}

//
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


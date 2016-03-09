#include "administration.h"


//const string Administration::terminals[TERMINALS] = { "id","num","bad num","bad name", "bad symbol", "badchar", "newline", "no name", "end of file", ".", ",", ";", "[", "]", "&", "|", "~", "<", "=", ">", "<=", ">=", "+", "-", "*", "/", "\\", ")", "(", ":=", "[]", "->", "begin", "end", "const", "array", "integer", "Boolean", "proc", "skip", "read", "write", "call", "if", "do", "fi", "od", "false", "true"};
//constructor
/*Administration::Administration(ifstream &in, ofstream &out, Scanner &sc)
:srcFile(in),outFile(out),scanner(sc),printSymbolTable(false)
{
}*/
Administration::Administration()
{
}
int Administration::scan()
{
		
	/*//initializing line no and error count		
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
				cerr<<"Too many errors. Bailing out!!\n";
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
						cerr<<"ERROR: Bad name occured at line "<<lineNo;
						errorCount++;
						break;
				case BADNUMERAL:
						cerr<<"ERROR: Bad numeral detected at line "<<lineNo;
						errorCount++;
						break;
				case BADSYMBOL:
						cerr<<"ERROR: Bad symbol found at line "<<lineNo;
						errorCount++;
						break;
				default:
					cerr<<"ERROR: Bad character found at line "<<lineNo;
						
			}	
		}
	}
	//if user wants then shows the symbol table on terminal
	if(printSymbolTable)	
		scanner.printSymTable();
	srcFile.close();
	outFile.close();
*/
	return errorCount;
}
//new line counter
void Administration::NewLine()
{
	lineNo++;
	//outFile<<"\n"<<"Line "<<lineNo<<":";	
	
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
void Administration::error(errorkind error,Symbol sym,int flag)
{
	switch(error)
	{
		case ScanE:

		break;
		case ParseE:
		if(flag == 1)
		{
			//cerr<<"Syntax Error: Missing "<<terminals[sym-256]<<" at line "<<lineNo<<endl;
		}
		break;
		

	}	
}	


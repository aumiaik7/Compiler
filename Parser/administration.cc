#include "administration.h"


const string Administration::terminals[TERMINALS] = { "id","num","bad num","bad name", "bad symbol", "badchar", "newline", "no name", "end of file", ".", ",", ";", "[", "]", "&", "|", "~", "<", "=", ">", "<=", ">=", "+", "-", "*", "/", "\\", ")", "(", ":=", "[]", "->", "begin", "end", "const", "array", "integer", "Boolean", "proc", "skip", "read", "write", "call", "if", "do", "fi", "od", "false", "true"};
//constructor
/*Administration::Administration(ifstream &in, ofstream &out, Scanner &sc)
:srcFile(in),outFile(out),scanner(sc),printSymbolTable(false)
{
}*/
Administration::Administration()
:lineNo(1)
{
}
int Administration::scan()
{
	//this method was used in previous phase to grab token in loop
	//but now parser grabs token from scanner so it is commented out

	
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
}

//error counter
void Administration::ErrorCount()
{	
	errorCount++;
	if(errorCount >= MAXERRORS)
	{
		cerr<<"Too many errors. Bailing out!!\n";
		exit(0);
	}
	
}

//checks wheather the token is valid or not. if valid then returns true
bool Administration::validTok(Symbol sym)
{
	if(sym == NONAME || sym == BADNAME || sym == BADNUMERAL || sym == BADSYMBOL)
		return false;
	else 
		return true;
}	

//Lexical and syntactical error reporting	
void Administration::error(errorkind error,Symbol sym,int flag)
{
	ErrorCount();	
	switch(error)
	{
		//lexical error
		case ScanE:
			cerr<<"Lexical Error: '"<<terminals[sym-256]<<"' occured at line "<<lineNo<<endl;	
		break;
		//parsing error
		case ParseE:	
		//this portions handles error in variableName, constantName and procedureName 
		if(flag < 4)
		{		
			string type[3] = {"variable", "constant", "procedure"};
			cerr<<"Syntax Error: Expecting name of type '"<<type[flag - 1]<<"' at line "<<lineNo<<endl;
		}
		//matching error
		else if(flag == 4)
		{
			string before = "at";
			if (sym == SEMICOLON)
				before = "on/before";	
			cerr<<"Syntax Error: Missing '"<<terminals[sym-256]<<"' "<<before<< " line "<<lineNo<<endl;
		}
		//found token that was not supposed to be there 
		else if(flag == 5)
		{
			string before = "at";
			if (sym == SEMICOLON)
				before = "on/before";	
			cerr<<"Syntax Error: Illegal token '"<<terminals[sym-256]<<"' at line "<<lineNo<<endl;
		}
		
		break;
	}	
}	


//Completion message
void Administration::done()
{
	cout<<endl<<"<<<Parsing Done>>>"<<endl;
	cout<<"The methods called for nonterminals are showed in ParseOutFile "<<endl;
	cout<<"Total no of lines Parsed: "<<lineNo<<endl;
}


#include "scanner.h"

//constructor
/*Scanner::Scanner()
{
}*/
Scanner::Scanner(ifstream &in, Symboltable st)
:srcFile(in), symTablePtr(st)
{
}




//opens the file provided in command line argument 

Token Scanner::nextToken()
{
	
	
	ch = srcFile.get();
	Token tok;
	if(ch == '~')
	{
		Token tk(ENDOFFILE,-1,"");
		return tk;
	}
	
	//this portion ignores comments in source file
	if(ch == '$')
	{			
				
		while((ch=srcFile.get()) != '\n' )
		{
		}
		
		
	}
	else if (isalpha(ch))
	{
		do
		{		
			ch = srcFile.get();
			if(ch == '\n')
			{	
				break;
			}
			/*else if(isSeparated())
			{
				token.setSymbol(ID);
				token.set
			}*/
			else if(!isalpha(ch) || !isdigit(ch) || ch != '_')
			{
				//cout << "Invalid Identifier at line " << lineNo;		
			}
		}
		while(1);
			
	}
	else if(ch == '\n')
	{
	//lineNo++;
	}
	else
		cout << ch;
	
	
	

	return tok;
}



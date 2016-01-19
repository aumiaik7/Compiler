#include "scanner.h"

//constructor
Scanner::Scanner(Token tk)
{
	token = tk;
}

//opens the file provided in command line argument 
void Scanner::start()
{
	//keywords = {"start", "finish", "then", "if", "repeat", "var", "int", "float", "do", "read", "print", "void", "return", "dummy", "program"};
	string wordSymbols[] = {"begin", "end", "const", "array", "integer", "Boolean", "proc", "skip", "read", "write", "call", "if", "do","fi","od", "false","true"};
 	
	string fileName = token.getFile(); 
	srcFile.open(token.getFile().c_str());
	if(!srcFile.is_open())
	{
		cout << "Error opening file" << endl;
		exit(0);
	
	}
	
	lineNo = 1;
	colNo = 1;
	while(1)
	{
		value = "";
		nextTok = nextToken();
		if(nextTok.getSymbol() == END)
			break;
	}
	cout<<lineNo;
	srcFile.close();
}
Token Scanner::nextToken()
{
	char ch, nextCh;
	
	ch = srcFile.get();

	if(ch == '~')
		token.setSymbol(END);
	
	//this portion ignores comments in source file
	if(ch == '$')
	{			
				
		while((ch=srcFile.get()) != '\n' )
		{
		}
		lineNo++;
		
	}
	else if (isalpha(ch))
	{
		do
		{		
			ch = srcFile.get();
			if(ch == '\n')
			{	
				lineNo++;		
				break;
			}
			else if(isSeparated())
			{
				token.setSymbol(ID);
				token.set
			}
			else if(!isalpha(ch) || !isdigit(ch) || ch != '_')
			{
				//cout << "Invalid Identifier at line " << lineNo;		
			}
		}
		while(1);
			
	}
	else if(ch == '\n')
		lineNo++;
	else
		cout << ch;
	
	
	

	return token;
}



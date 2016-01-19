#include "scanner.h"

//constructor
Scanner::Scanner(Token tk)
{
	token = tk;
}

//opens the file provided in command line argument 
void Scanner::start()
{
	keywords = {"start", "finish", "then", "if", "repeat", "var", "int", "float", "do", "read", "print", "void", "return", "dummy", "program"};	
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
	if(ch == '/')
	{	
		ch = srcFile.get();	
		//ignores single line comments
		if(ch == '/')
		{
			while((ch=srcFile.get()) != '\n' )
			{
			}
			lineNo++;
		}
		//ignores multiple line comments
		else if(ch == '*')
		{
				
			do
			{	
				
				ch =  srcFile.get();
				if(ch == '*')
				{	
					if((ch = srcFile.get()) == '/')					
						break;
						
				}
				else if(ch == '\n')
					lineNo++;
				
				//else cout << ch;

				
			}while(1);
			
		}
	}
	else if (isalpha(ch))
	{
		do
		{		
			ch = srcFile.get();
		}
		while(ch == '_' || isdigit(ch));
			
	}
	else if(ch == '\n')
		lineNo++;
	else
		cout << ch;
	
	
	

	return token;
}



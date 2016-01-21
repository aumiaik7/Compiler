#include "scanner.h"

//constructor
Scanner::Scanner(ifstream &in, Symboltable st)
:srcFile(in), symTablePtr(st)
{
}





//Gets token
Token Scanner::nextToken()
{
	
	
	ch = srcFile.get();
	Token tok;
	string lex = "";
	int value = 0;
	if(ch == EOF)
	{
		Token tk(ENDOFFILE,-1,"");
		return tk;
	}
	
	//this portion ignores comments in source file
	else if(ch == '$')
	{			
		laCh = srcFile.peek();		
		while(srcFile.peek() != '\n' )
		{
			srcFile.get();
		}
			
	}
	else if (isalpha(ch))
	{
		lex = ch;
		do
		{	
				
			laCh = srcFile.peek();
			if(isalnum(laCh) || laCh == '_')
			{
				//cout << laCh;
				lex += laCh;
				srcFile.get();				
			}			
			else if(isWhitespace(laCh) || (isSpecial(laCh) && laCh != '~'))
			{	
				Token tk(ID,-1,lex);
				return tk;
			}
			
			else
			{
				Token tk(BADNAME,-1,"");
				while(srcFile.peek() != '\n')
				{
					srcFile.get();				
				}
				return tk;
			}
			
		}
		while(1);
			
	}
	else if(isdigit(ch))
	{
		value = ch - '0';
		do
		{
			laCh = srcFile.peek();
			
			if(isdigit(laCh))
			{
				
				value = (10*value) + (srcFile.get() - '0'); 
			}
			else if(isWhitespace(laCh) || isNumEnd(laCh))
			{	
				Token tk(NUMERAL,value,"");
				return tk;
			}
			else
			{
				Token tk(BADNUMERAL,-1,"");
				while(srcFile.peek() != '\n')
				{
					srcFile.get();				
				}
				return tk;
			}

		}while(1);
	}	
	else if(ch == '\n')
	{
		Token tk(NEWLINE,-1,"");
		return tk;
	}
	else
		cout << ch;
	
	
	

	return tok;
}


//Supporting functions
bool Scanner::isWhitespace(char lach)
{
	if(lach == ' ' || lach == '\t' || lach == '\n')
	{	
		if(lach == ' ' || lach == '\t')
			srcFile.get();	
		return true;
	}
	else 
		return false;
}

bool Scanner::isSpecial(char lach)
{
	if(lach == '.' || lach == ',' || lach == ';' || lach == '[' || lach == ']' || lach == '&' || lach == '|'
 || lach == '~' || lach == '<' || lach == '=' || lach == '>' || lach == '+' || lach == '-' || lach == '*' || lach == '/'
 || lach == '\\' || lach == '(' || lach == ')' || lach == ':' || lach == '$' )
		return true;
	else 
		return false;
}

bool Scanner::isNumEnd(char lach)
{
	if(lach == ',' || lach == ';' || lach == ']' || lach == '<' || lach == '=' || lach == '>' || lach == '+' || lach == '-' || lach == '*' || lach == '/' || lach == '\\' || lach == ')' ||  lach == '$' )
		return true;
	else 
		return false;
}

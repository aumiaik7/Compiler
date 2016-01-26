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
		Token tk(NONAME,-1,lex);
			return tk;
			
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
				int keyOrId = symTablePtr.insert(lex);			
				if(keyOrId == 1)
				{
					Token tk(ID,-1,lex);
					return tk;
				}
				else if (keyOrId == 2)
				{
					//symbol table full 
					Token tk;
					return tk;
				}
				else
				{
					Token tk((Symbol)keyOrId,-1,lex);
					return tk;
				}	
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
	else if(isSpecial(ch))
	{
		if(ch == '.')
		{
			Token tk(DOT,-1,"");
			return tk;
		}
		else if(ch == ',')
		{
			Token tk(COMMA,-1,"");
			return tk;
		}
		else if(ch == ';')
		{
			Token tk(SEMICOLON,-1,"");
			return tk;
		}
		else if(ch == '[')
		{
			if(srcFile.peek() == ']')
			{
				srcFile.get();
				Token tk(GC1,-1,"");
				return tk;
			}
			else
			{
				Token tk(LEFTBRACKET,-1,"");
				return tk;
			}
		}
		else if(ch == ']')
		{
			Token tk(RIGHTBRACKET,-1,"");
			return tk;
		}
		else if(ch == '&')
		{
			Token tk(AND,-1,"");
			return tk;
		}
		else if(ch == '|')
		{
			Token tk(OR,-1,"");
			return tk;
		}
		else if(ch == '~')
		{
			Token tk(NOT,-1,"");
			return tk;
		}
		else if(ch == ']')
		{
			Token tk(RIGHTBRACKET,-1,"");
			return tk;
		}
		else if(ch == '<')
		{
			if(srcFile.peek() == '=')
			{
				srcFile.get();
				Token tk(LTE,-1,"");
				return tk;
			}
			else
			{
				Token tk(LESST,-1,"");
				return tk;
			}
		}
		else if(ch == '>')
		{
			if(srcFile.peek() == '=')
			{
				srcFile.get();
				Token tk(GTE,-1,"");
				return tk;
			}
			else
			{
				Token tk(GREATERT,-1,"");
				return tk;
			}
		}
		else if(ch == '=')
		{
			Token tk(EQUAL,-1,"");
			return tk;
		}
		else if(ch == '+')
		{
			Token tk(PLUS,-1,"");
			return tk;
		}	
		else if(ch == '-')
		{
			if(srcFile.peek() == '>')
			{
				srcFile.get();
				Token tk(GC2,-1,"");
				return tk;
			}
			else
			{
				Token tk(MINUS,-1,"");
				return tk;
			}
		}
		else if(ch == '*')
		{
			Token tk(TIMES,-1,"");
			return tk;
		}
		else if(ch == '/')
		{
			Token tk(DIV,-1,"");
			return tk;
		}
		else if(ch == '\\')
		{
			Token tk(MOD,-1,"");
			return tk;
		}
		else if(ch == '(')
		{
			Token tk(LEFTP,-1,"");
			return tk;
		}
		else if(ch == ')')
		{
			Token tk(RIGHTP,-1,"");
			return tk;
		}
		else if(ch == ':')
		{
			if(srcFile.peek() == '=')
			{
				srcFile.get();
				Token tk(ASSIGN,-1,"");
				return tk;
			}
			else
			{
				Token tk(BADSYMBOL,-1,"");
				return tk;
			}
		}	
			
				
	}
	else if(isWhitespace(ch))
	{
		return tok;
	}
	
	else
	{
		Token tk(BADSYMBOL,-1,"");
		while(srcFile.peek() != '\n')
		{
			srcFile.get();				
		}
		return tk;
	}
	
	
	

	return tok;
}


//Supporting functions
bool Scanner::isWhitespace(char lach)
{
	if(lach == ' ' || lach == '\t' || lach == '\n')
	{	
		while(srcFile.peek() == ' ' || srcFile.peek() == '\t')
		{
			srcFile.get();				
		}		
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

void Scanner::printSymTable()
{
	symTablePtr.print();
}		

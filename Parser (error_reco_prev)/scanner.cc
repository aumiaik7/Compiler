#include "scanner.h"

//constructor
Scanner::Scanner(ifstream &in, Symboltable st)
:srcFile(in), symTablePtr(st),idType(1)
{
	
}

//token generator
Token Scanner::nextToken()
{	
	//read a character from source file
	ch = srcFile.get();
	//temorary token object. returns either valid or invalid token
	Token tok;
	//lexeme for identifier/keywords
	string lex = "";
	//value for numerals
	int value = 0;
	//end of file
	if(ch == EOF)
	{
		Token tk(ENDOFFILE,-1,"");
		return tk;
	}
	
	//ignore comments in source file
	else if(ch == '$')
	{	
		//look at next character of source file without reading it		
		laCh = srcFile.peek();	
		//continue to read chars until the next char is new line		
		while(srcFile.peek() != '\n' )
		{
			srcFile.get();
		}
		//NONAME token returned
		Token tk(NONAME,-1,lex);
			return tk;
			
	}
	//finite machine for detecting identifier or keyword	
	if (isalpha(ch))
	{
		lex = ch;
		do
		{	
				
			laCh = srcFile.peek();
			//next char is also a valid char			
			if(isalnum(laCh) || laCh == '_')
			{
				//cout << laCh;
				lex += laCh;
				srcFile.get();				
			}	
			//vald id/keyword detected		
			else if(isWhitespace(laCh) || isSpecial(laCh))
			{	
				//check wheather its id/keyword from symbol table
				int keyOrId = symTablePtr.insert(lex,idType);		
				//it's an ID	
				if(keyOrId <= 3)
				{
					Token tk(ID,-1,lex);
					tk.setIDtype(keyOrId);
					idType = 1;	
					return tk;
				}
				//symbol table full program exits
				else if (keyOrId == 4)
				{
					cout<<"Symbol table is full. Program exits"<<endl;
					exit(0);				
				}
				//it's a keyword
				else
				{
					//next identifier is of type constant
					if(keyOrId == 290)
						idType = 2; 
					else if(keyOrId == 294)
						idType = 3;
					else
						idType = 1;
					Token tk((Symbol)keyOrId,-1,"");
					return tk;
				}	
			}
			//invalid identifier 
			else
			{
				Token tk(BADNAME,-1,"");
				// as error detected scanner continues to read chars untill it reaches end of line
				while(srcFile.peek() != '\n')
				{
					srcFile.get();				
				}
				
				return tk;
			}
			
		}
		while(1);
			
	}
	//finite machine for detecting numeral
	else if(isdigit(ch))
	{
		
		lex = ch;
		do
		{
			laCh = srcFile.peek();
			//next char is also a valid digit
			if(isdigit(laCh))
			{
				
				//value = (10*value) + (srcFile.get() - '0'); 
				lex += srcFile.get();
			}
			//end of numeral detected
			else if(isWhitespace(laCh) || isNumEnd(laCh))
			{	
				//check wheather the numeral is valid or not
				value = isValidNum(lex);				
				//valid numeral
				if(value != -1)				
				{
					Token tk(NUMERAL,value,"");
					return tk;
				}
				//Bad numeral
				else
				{
					Token tk(BADNUMERAL,-1,"");
					// as error detected scanner continues to read chars untill it reaches end of line
					while(srcFile.peek() != '\n')
					{
						srcFile.get();				
					}
					return tk;		
				}
			}
			else
			{
				//Bad numeral detected
				Token tk(BADNUMERAL,-1,"");
				while(srcFile.peek() != '\n')
				{
					srcFile.get();				
				}
				return tk;
			}

		}while(1);
	}
	//new line char detected
	else if(ch == '\n')
	{
		Token tk(NEWLINE,-1,"");
		return tk;
	}

 	//finite machine for detecting special symbols
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
		//either array bracket or guarded command  
		else if(ch == '[')
		{
			//if next char is ']' then it's a guarded command
			if(srcFile.peek() == ']')
			{
				srcFile.get();
				Token tk(GC1,-1,"");
				return tk;
			}
			//it's a left bracket for array subscript
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
		// less than or less than equal to detector	
		else if(ch == '<')
		{
			//next char is '=' so it's a lte
			if(srcFile.peek() == '=')
			{
				srcFile.get();
				Token tk(LTE,-1,"");
				return tk;
			}
			//only less than symbol
			else
			{
				Token tk(LESST,-1,"");
				return tk;
			}
		}
		// greater than or greater than equal to detector	
		else if(ch == '>')
		{
			//next char is '=' so it's a gte
			if(srcFile.peek() == '=')
			{
				srcFile.get();
				Token tk(GTE,-1,"");
				return tk;
			}
			//only greater than symbol
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
		//either minus or guarded command  	
		else if(ch == '-')
		{
			//if next char is '>' then it's a guarded command	
			if(srcFile.peek() == '>')
			{
				srcFile.get();
				Token tk(GC2,-1,"");
				return tk;
			}
			//it's a minus symbol
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
		//check wheather the next symbol is '='
		else if(ch == ':')
		{
			//if next char is '=' so it's a assignment symbol
			if(srcFile.peek() == '=')
			{
				srcFile.get();
				Token tk(ASSIGN,-1,"");
				return tk;
			}
			//otherwise bad symbol
			else
			{
				Token tk(BADSYMBOL,-1,"");
				return tk;
			}
		}	
			
				
	}
	//return noname 
	else if(isWhitespace(ch))
	{
		return tok;
	}
	//otherwise return bad symbol
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
//helps to detect white spaces
bool Scanner::isWhitespace(char lach)
{
	if(lach == ' ' || lach == '\t' || lach == '\n')
	{	
		//read whitespaces from file. But does not read new line because there is a seperate portion in the scanner for reading newline as it count the lines in source file
		while(srcFile.peek() == ' ' || srcFile.peek() == '\t')
		{
			srcFile.get();				
		}		
		return true;
	}
	else 
		return false;
}
//helps to detect special symbols
bool Scanner::isSpecial(char lach)
{
	if(lach == '.' || lach == ',' || lach == ';' || lach == '[' || lach == ']' || lach == '&' || lach == '|'
 || lach == '~' || lach == '<' || lach == '=' || lach == '>' || lach == '+' || lach == '-' || lach == '*' || lach == '/'
 || lach == '\\' || lach == '(' || lach == ')' || lach == ':' || lach == '$' )
		return true;
	else 
		return false;
}

//helps to detect end of numerals as end of numeral is different form end of id
bool Scanner::isNumEnd(char lach)
{
	if(lach == ',' || lach == ';' || lach == ']' || lach == '<' || lach == '=' || lach == '>' || lach == '+' || lach == '-' || lach == '*' || lach == '/' || lach == '\\' || lach == ')' ||  lach == '$' )
		return true;
	else 
		return false;
}

//check wheather the numeral is good or bad numeral
int Scanner::isValidNum(string s)
{
	int value = 0;	
	if(s.length() > 10)
		return -1;
	else
	{
		for(int i = 0; i < s.length(); i++)
		{
			value = (10*value) + (s[i] - '0'); 
		}
		if(value>=0 && value<=2147483647)
			return value;
		else
			return -1;  	
	}
		
}	

//show symbol table in terminal
void Scanner::printSymTable()
{
	symTablePtr.print();
}	

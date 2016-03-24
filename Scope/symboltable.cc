#include "symboltable.h"

const string Symboltable::keywords[KEYWORDS] = {"begin", "end", "const", "array", "integer", "Boolean", "proc", "skip", "read", "write", "call", "if", "do", "fi", "od", "false", "true"};

Symboltable::Symboltable()
:occupied(1)
{
	//Symbol for begin is 288
	int sym = 288; 
	
	vector<Token>().swap(htable);
	//initiazlizinf hash table
	htable.reserve(SYMTABLESIZE);
	for( int i = 0; i < SYMTABLESIZE; ++i )
            htable.push_back(Token(NONAME,-1,"noname"));
	
	//all keywords are preloaded to symbol table inside this constructor
	for(int i=0; i < 17; i++)
	{
		Symbol castEnum = (Symbol)sym;
		Token tok(castEnum,-1,keywords[i]);
		insert(tok);
		sym +=1;
	}
	
	
}
//return true if table is full otherwise false;
bool Symboltable::full()
{
	return occupied == SYMTABLESIZE;
}
//return number of occupiend cells in symbol table
int Symboltable::getOC()
{
	return occupied;
}

//insert keyword/id in symbol table
Token Symboltable::insert(string s,int type)
{
		
	//symbol table is full 
	if(full())
	{
		cerr<<"Symbol table is full. Program exits"<<endl;
		exit(0);
	}
	int isOccupied = search(s);
		
	//if(s == keywords[1])
	//	return 289;
	//position is empty keyword can be inserted 
	if(isOccupied == -1)
	{	
		Token tk(ID,position,s);
		tk.setIDtype(type);			
		htable.at(position) = tk;
		//tk = htable.at(position);
		occupied++;
		return tk;
	}

	//position is occupied finds the next empty position and insert
	else
	{
		while(1)
		{
						
			Token tempTk;
			tempTk = htable.at(position); 	
						
			//263 is NONAME symbol so position is empty insert here
			if(tempTk.getSymbol() == 263)
			{	
				
				Token tk(ID,position,s);
				tk.setIDtype(type);
				htable.at(position) = tk;
				//tk = htable.at(position);
				occupied++;
				return tk;
				
			}
			//go to the next position			
			else
			{
							
				int sym = tempTk.getSymbol();
				
				if(sym >= 288  && s == keywords[sym - 288] ) //encoding scheme for matching lexeme with keyword
				{
					return tempTk; // for keyword
				}				
				else if( s == tempTk.getLexeme())
					return tempTk; // same identifier already stored
				
				//last position so reset
				else
				{
					if(position > 306)
					{
						position = 0;
					}
			
					else
						position++;				
				
				}
			}
		}
	}
	
}

//preload symbol table with keywords
void Symboltable::insert(Token tok)
{
	
	//checks the position is occupied or not
	int isOccupied = search(tok.getLexeme());
	Token tk(tok.getSymbol(),-1,"");
	
	//position is empty keyword can be inserted 
	if(isOccupied == -1)
	{	
		htable.at(position) = tk;
		tk = htable.at(position);
		occupied++;
	}
	//position is occupied finds the next empty position and insert
	else
	{
		position++;
		while(1)
		{
						
			Token tempTk;
			tempTk = htable.at(position); 	
				
			
			//263 is NONAME symbol so position is empty insert here
			if(tempTk.getSymbol() == 263)
			{	
				htable.at(position) = tk;
				tk = htable.at(position);
				occupied++;
				break;	
				
			}
			//go to the next position			
			else
			{
				//last position so reset
				if(position > 306)
				{
					position = 0;
				}
			
				else
					position++;				
				
				
			}
		}
	}
	
}
//search function
int Symboltable::search(string lex)
{
	position = hashfn(lex);
	Token tk;
		
	tk = htable.at(position); 
	
	if(tk.getSymbol() ==263)
		return -1;
	else
		return position;					
	
	
}

//hash function
int Symboltable::hashfn(string lex)
{
	int asciiValue = 0;
	for(int i = 0; i < lex.length(); i++)
	{
		asciiValue += int(lex.at(i)); 	
	}	
	
	return asciiValue%307;
	
}

//print symbol table
void Symboltable::print()
{

	cout<<endl<<"<<Symbol Table>>"<<endl;	
	for(int i=0; i < htable.size(); i++)
	{
		Token tempTk;
		tempTk = htable.at(i); 	
				
		//263 is NONAME symbol
		if(tempTk.getSymbol() != 263)
		{
			cout<<"Symbol ID: "<<tempTk.getSymbol() <<" Lexeme: "<<tempTk.getLexeme()<<" Position: "<<i<<endl;
		}
	}
}

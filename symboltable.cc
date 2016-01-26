#include "symboltable.h"
//keywords[KEYWORDS] = {"begin", "end", "const", "array", "integer", "Boolean", "proc", "skip", "read", "write", "call", "if", "do", "fi", "od", "false", "true"};
Symboltable::Symboltable()
:occupied(1), htable(SYMTABLESIZE),keywords({"begin", "end", "const", "array", "integer", "Boolean", "proc", "skip", "read", "write", "call", "if", "do", "fi", "od", "false", "true"})
{
	int sym = 288;
	
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

int Symboltable::getOC()
{
	return occupied;
}

//insert keyword/id in symbol table
int Symboltable::insert(string s)
{
	//symbol table is full 
	if(full())
		return 2;	
	int isOccupied = search(s);
		
	//position is empty keyword can be inserted 
	if(isOccupied == -1)
	{	
		Token tk(ID,-1,s);		
		htable.at(position) = tk;
		tk = htable.at(position);
		occupied++;
		//cout <<" inserted: "<<tk.getLexeme()<<" at position: "<<position<<endl<<endl;	
		return 1;			
	}
	//position is occupied finds the next empty position and insert
	else
	{
		while(1)
		{
						
			Token tempTk;
			tempTk = htable.at(position); 	
				
			
			//263 is NONAME symbol
			if(tempTk.getSymbol() == 263)
			{	
				
				Token tk(ID,-1,s);
				htable.at(position) = tk;
				tk = htable.at(position);
				occupied++;
				//cout <<" inserted: "<<tk.getLexeme()<<" at position: "<<position<<endl<<endl;
				return 1;	
				
			}
			//go to the next position			
			else
			{
							
				int sym = tempTk.getSymbol();
				
				if((sym >= 288 && sym <= 304) && s == keywords[sym - 288] ) //encoding scheme for matching lexeme with keyword
				{
					
					return sym; // for keyword
				
				}				
				else if( s == tempTk.getLexeme())

					return 1; // same identifier already stored				
				
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
		
		//htable.insert(htable.begin()+position, tk );
		htable.at(position) = tk;
		tk = htable.at(position);
		occupied++;
		//cout <<" inserted: "<<tok.getLexeme()<<" at position: "<<position<<endl<<endl;
	}
	//position is occupied finds the next empty position and insert
	else
	{
		position++;
		while(1)
		{
						
			Token tempTk;
			tempTk = htable.at(position); 	
				//cout<<tempTk.getSymbol()<<endl;
			
			//263 is NONAME symbol
			if(tempTk.getSymbol() == 263)
			{	
				
				htable.at(position) = tk;
				tk = htable.at(position);
				//cout <<" inserted: "<<tok.getLexeme()<<" at position: "<<position<<" Symbol: "<< tok.getSymbol()<<endl<<endl;
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
	//cout<<"Position for lex: "<<lex<<" is:"<<position<<endl;	
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
				//cout<<tempTk.getSymbol()<<endl;
		//263 is NONAME symbol
		if(tempTk.getSymbol() != 263)
		{
			cout<<"Symbol ID: "<<tempTk.getSymbol() <<" Lexeme: "<<tempTk.getLexeme()<<endl;
		}
	}
}

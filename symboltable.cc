#include "symboltable.h"

Symboltable::Symboltable()
:occupied(0), htable(SYMTABLESIZE)
{
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

int Symboltable::insert(string s)
{
	int position = search(s);
	if(position != -1)
	{
		Token tk(IF,-1,s);
		//htable.insert(htable.begin()+position, tk );
		htable.at(position) = tk;
		tk = htable.at(position);
		cout <<"inserted: "<<tk.getLexeme()<<" at position: "<<position;
	}
	
}
//search function
int Symboltable::search(string lex)
{
	int index = hashfn(lex);
	Token tk;
	tk = htable.at(index); 
	cout<<"Lexeme: "<<tk.getLexeme();
	if(tk.getLexeme() == "noname")
	{	
		//cout<<	
		return index;
	}
	else
		return -1;
	//cout << htable(index);
	
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

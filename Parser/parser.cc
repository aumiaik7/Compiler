#include "parser.h"

//constructor
Parser::Parser(ifstream &in, ofstream &out, Scanner &sc)
:srcFile(in),outFile(out),scanner(sc),islookAheadTok(false)
{
}

void Parser::program()
{
		
	cout<<"Call block()"<<endl;	
	block();	
	//cout<<"Matched ."<<endl;
	match(DOT);
	
}

void Parser::block()
{
	
	//cout<<"Matched begin"<<endl;		
	match(BEGIN);	

	cout<<"Call defPart()"<<endl;
	definitionPart();
	//cout<<"Matched end"<<endl;
	match(END);
	
}

void Parser::definitionPart()
{
	
	
	//cout<<"Matched semicolon"<<endl;
	
	lookAheadToken();	
	if(ff.firstOfDefinition(nextTok.getSymbol()))
	{
		cout<<"Call definition()"<<endl;
		definition();
		match(SEMICOLON);	
		definitionPart();
	}
	/*else if(ff.followOfDefPart())
	{
		//do Nothing	
	}*/	
	
}

void Parser::definition()
{
	
	lookAheadToken();
	if(ff.firstOfConstDef(nextTok.getSymbol()))
	{
		cout<<"Call constantDefinition()"<<endl;		
		constantDefinition();
	}
	else if(ff.firstOfVariDef(nextTok.getSymbol()))
	{
		cout<<"Call variableDefinition()"<<endl;		
		variableDefinition();
	}
	else if(ff.firstOfProcDef(nextTok.getSymbol()))
	{
		cout<<"Call procedureDefinition()"<<endl;	
		procedureDefinition();
	}
}

void Parser::statementPart()
{
	
	lookAheadToken();	
	if(ff.firstOfDefinition(nextTok.getSymbol()))
	{
		cout<<"Call statement()"<<endl;
		definition();
		match(SEMICOLON);	
		definitionPart();
	}
}



void Parser::constantDefinition()
{
	//cout<<"Matched const"<<endl;
	match(CONST);
	//cout<<"Matched ID"<<endl;
	match(ID);
	//cout<<"Matched ="<<endl;	
	match(EQUAL);
	
	
	constant();
	
}

void Parser::variableDefinition()
{
	cout<<"Call typeSymbol()"<<endl;	
	typeSymbol();
	lookAheadToken();
	if(ff.firstOfVariList(nextTok.getSymbol()))
	{
		cout<<"Call varableList()"<<endl;		
		variableList();
	}
	else if(nextTok.getSymbol() == ARRAY)
	{
		match(ARRAY);
		cout<<"Call varableList()"<<endl;		
		variableList();
		match(LEFTBRACKET);
		cout<<"Call constant()"<<endl;
		constant();
		match(RIGHTBRACKET);		
	}
}



void Parser::typeSymbol()
{
	if(match(INT) || match(BOOL))
	{
		//do nothing		
		cout<<"Ignore Matching errors"<<endl;
	}
	else
	{	//error msg
		cout<<"Error in typeSymbol"<<endl;
	}
	
}

void Parser::variableList()
{
	match(ID);
	lookAheadToken();
	if(nextTok.getSymbol()==COMMA)
	{
		match(COMMA);
		cout<<"Call varableList()"<<endl;
		variableList();
	}
}

void Parser::constant()
{
	
	if(match(NUMERAL) || match(TRUE) || match(FALSE) || match(ID))
	{
		//do nothing
		cout<<"Ignore Previous Matching errors"<<endl;		
	}
	
	else
	{	//error msg
		
	}
}

void Parser::procedureDefinition()
{
	match(PROC);
	match(ID);
	block();
}
//function for matching terminal symbols
bool Parser::match(Symbol sym)
{
	if(islookAheadTok)
		islookAheadTok = false;
	else	
		nextTok = scanner.nextToken();

	//grab the token other than newline or noname
	if(nextTok.getSymbol() == NEWLINE || nextTok.getSymbol() == NONAME)
	{		
		
		while(1)
		{	
			if(nextTok.getSymbol() == NEWLINE)
			{
				NewLine();
			}
			else if(nextTok.getSymbol() != NONAME)
				break;
			nextTok = scanner.nextToken();
		}
		
	}
	
	if(nextTok.getSymbol() == sym)
	{
		//ok do nothing;	
		//cout<<"MATCHED "<<nextTok.getSymbol()<<endl;
		return true;
	}
	
	else
	{
		cout<<nextTok.getSymbol()<<" Symbol:"<<nextTok.getLexeme() <<"  Error Message"<<endl;
		//this means a token is not matched a lookahead token is grabbed		
		islookAheadTok = true;
		//cout<<<<endl;
		return false;
	}
}

void Parser::lookAheadToken()
{
	//this checking is done so that parser will not stuck in error. 
	//if any token is already read it will not read another token again.
	//if not read then it will grab a token 	
	if(!islookAheadTok)
	{
		nextTok = scanner.nextToken();	
		islookAheadTok = true;
	}
	
	//grab the token other than newline or noname
	if(nextTok.getSymbol() == NEWLINE || nextTok.getSymbol() == NONAME)
	{		
		
		while(1)
		{	
			if(nextTok.getSymbol() == NEWLINE)
			{
				NewLine();
			}
			else if(nextTok.getSymbol() != NONAME)
				break;
			nextTok = scanner.nextToken();
		}
		
	}
}

//new line counter
void Parser::NewLine()
{
	lineNo++;
	//outFile<<"\n"<<"Line "<<lineNo<<":";	
	
}


//write error messages to output file	
void Parser::error(string text)
{
	
}	


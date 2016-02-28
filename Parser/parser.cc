#include "parser.h"

//constructor
Parser::Parser(ifstream &in, ofstream &out, Scanner &sc)
:srcFile(in),outFile(out),scanner(sc),islookAheadTok(false)
{
}

/*
  Imoplementation of the CFG is self explanatory
  for non terminal ther are corresponding methods
  and terminal symbols are matched using match(Symbol)
  method				
*/

// program = block '.'
void Parser::program()
{
	lineNo = 1;	
	errorCount = 0;
	outFile<<"program()"<<endl;	
	block();	
	
	if(!match(DOT))
	{
		cerr<<"Syntax Error: "<<"'.' (dot) is missing at line "<< lineNo <<endl;
		ErrorCount();	
	}

	//shows total no of lines parsed at the end
	outFile<<"Total no of lines: "<<lineNo<<endl;
	
}

// block = 'begin' definitionPart statementPart 'end'
void Parser::block()
{
	outFile<<"block()"<<endl;
	if(!match(BEGIN))
	{
		cerr<<"Syntax Error: "<<"begin is missing at line "<< lineNo  <<endl;
		ErrorCount();
		syntaxError(1);
	}
			

	lookAheadToken();
	if(ff.firstOfDefinition(nextTok))
	{	
		definitionPart();
		statementPart();
	}
	else if(ff.followOfDefPart(nextTok))
	{
		statementPart();
	}
	else
	{
		//error message
		cerr<<"Syntax Error: "<<"Illegal start of definition/statement at line "<< lineNo <<endl;
		ErrorCount();
	}

	if(!match(END))
	{
		//error message		
		cerr<<"Syntax Error: "<<"end is missing at line "<< lineNo <<endl;
		ErrorCount();
		syntaxError(2);
	}	
	
}

// definitionPart = {definition';'}
void Parser::definitionPart()
{
	
	outFile<<"definitionPart()"<<endl;
	
	lookAheadToken();	
	if(ff.firstOfDefinition(nextTok))
	{
		definition();
		if(!match(SEMICOLON))
		{
			//error message
			cerr<<"Syntax Error: "<<" ';' is missing at line "<< lineNo <<endl;
			ErrorCount();
			syntaxError(3);
		}		
		definitionPart();
	}
		
}

// definition = constantDefinition | variableDefinition | procedureDefinition
void Parser::definition()
{
	outFile<<"definition()"<<endl;
	
	lookAheadToken();
	if(ff.firstOfConstDef(nextTok))
	{
		constantDefinition();
	}
	else if(ff.firstOfVariDef(nextTok))
	{
		variableDefinition();
	}
	else if(ff.firstOfProcDef(nextTok))
	{
		procedureDefinition();
	}
}

// constantDefinition = 'const' constName '='  constant;
void Parser::constantDefinition()
{
	outFile<<"constantDefinition()"<<endl;
	match(CONST);
	
	if(match(ID))
	{
		if(nextTok.getIDtype() != 2)
		{	
			//error message
			cerr<<"Syntax Error: "<<"expecting name of constant type at line "<< lineNo <<endl;
			ErrorCount();
		}
		
	}
	else
	{
			cerr<<"Syntax Error: "<<" constantName is missing at line "<< lineNo <<endl;
			ErrorCount();
			syntaxError(4);
	}
	//outFile<<"Matched ="<<endl;	
	if(!match(EQUAL))
	{
		cerr<<"Syntax Error: "<<" '=' is missing at line "<< lineNo <<endl;
		ErrorCount();
		syntaxError(5);
	}
	
	constant();
	
}

// variableDefinition = typeSymbol variableList | typeSymbol 'array' variableList'['constant']'
void Parser::variableDefinition()
{
	outFile<<"variableDefinition()"<<endl;	
	typeSymbol();
	lookAheadToken();
	if(ff.firstOfVariList(nextTok))
	{
		variableList();
	}
	else if(match(ARRAY))
	{
		variableList();
		if(!match(LEFTBRACKET))
		{
			//error msg
			cerr<<"Syntax Error: "<<" '[' is missing at line "<< lineNo <<endl;
			ErrorCount();
			syntaxError(6);		
		}
		constant();
		if(!match(RIGHTBRACKET))
		{
			//error msg
			cerr<<"Syntax Error: "<<" ']' is missing at line "<< lineNo <<endl;
			ErrorCount();
			syntaxError(5);		
		}		
	}
	else
	{
		
		if(nextTok.getSymbol()!=ID)
		{
			//error msg
			cerr<<"Syntax Error: "<<" Expecting variableName/'array' at line "<< lineNo <<endl;
			ErrorCount();
			syntaxError(5);
		}
		else
		{
			//error msg	
			cerr<<"Syntax Error: "<<"expecting name of variable type at line "<< lineNo <<endl;
			ErrorCount();	
			variableList();	
		}
	}
}

// procedureDefinition = 'proc' procedureName block
void Parser::procedureDefinition()
{
	outFile<<"procedureDefinition()"<<endl;
	match(PROC);
	if(match(ID))
	{
		if(nextTok.getIDtype() != 3)
		{	
			//error message
			cerr<<"Syntax Error: "<<"expecting name of proc type at line "<< lineNo <<endl;
			ErrorCount();	
		}
	}
	else
	{
		//error msg
		cerr<<"Syntax Error: "<<" procedure name is missing at line "<< lineNo <<endl;
		ErrorCount();
		syntaxError(9);
	}
	block();
}

// typeSymbol = 'integer' | 'Boolean'
void Parser::typeSymbol()
{
	outFile<<"typeSymbol()"<<endl;
	if(match(INT) || match(BOOL))
	{
		//do nothing		
	}
	
	
}

// variableList = variableName {','variableName}
void Parser::variableList()
{
	outFile<<"variableList()"<<endl;
	if(match(ID))
	{
		if(nextTok.getIDtype() != 1)
		{	
				
		}
		
	}
	//lookAheadToken();
	if(match(COMMA))
	{
		variableList();
	}
	
	
}

// statementPart = {statement';'}
void Parser::statementPart()
{
	outFile<<"statementPart()"<<endl;
	lookAheadToken();
	if(ff.firstOfStatement(nextTok))
	{
		statement();
		if(!match(SEMICOLON))
		{
			cerr<<"Syntax Error: "<<" ';' is missing at line "<< lineNo <<endl;
			ErrorCount();
			syntaxError(7);	
		}
		statementPart();
	}
	else if(ff.followOfStatePart(nextTok))
	{
		//do nothing
	}
	else
	{
		// error message
		cerr<<"Syntax Error: "<<" Illegal start of statement at line "<< lineNo<<" token "<<nextTok.getSymbol() <<endl;
		ErrorCount();
		syntaxError(7);	
		statementPart();
	}
}

// statement = emptyStatement | readStatement | writeStatement |  assignmentStatement | ifStatement | doStatement 
void Parser::statement()
{
	outFile<<"statement()"<<endl;
	lookAheadToken();	
	if(ff.firstOfEmptySt(nextTok))
	{
		match(SKIP);
	}
	else if(ff.firstOfReadSt(nextTok))
	{
		readStatement();
	}
	else if(ff.firstOfWriteSt(nextTok))
	{
		writeStatement();
	}
	else if(ff.firstOfProcSt(nextTok))
	{
		match(CALL);
		if(match(ID))
		{
			if(nextTok.getIDtype() != 3)
			{	
				//error message
				cerr<<"Syntax Error: "<<"expecting name of proc type at line "<< lineNo <<endl;
				ErrorCount();	
			}
		}
		else
		{
			//error msg
			cerr<<"Syntax Error: "<<" procedure name is missing at line "<< lineNo <<endl;
			ErrorCount();
			syntaxError(10);
		}
	}
	else if(ff.firstOfAssignSt(nextTok))
	{
		assignmentStatement();
	}
	else if(ff.firstOfIfSt(nextTok))
	{
		ifStatement();		
	}
	else if(ff.firstOfDoSt(nextTok))
	{
		doStatement();		
	}
	
}

//

void Parser::readStatement()
{
	outFile<<"readStatement()"<<endl;
	match(READ);
	variableAccessList();
			
}

void Parser::writeStatement()
{
	outFile<<"writeStatement()"<<endl;
	match(WRITE);
	expressionList();
	
		
}
void Parser::doStatement()
{
	outFile<<"doStatement()"<<endl;
	match(DO);
	guardedCommandList();
	match(OD);
}

void Parser::ifStatement()
{
	outFile<<"ifStatement()"<<endl;
	match(IF);
	guardedCommandList();
	if(!match(FI))
	{
		cerr<<"Syntax Error: "<<" 'fi' is missing at line "<< lineNo <<endl;
		ErrorCount();
		syntaxError(7);
	}	
	
}

void Parser::guardedCommandList()
{
	outFile<<"guardedCommandList()"<<endl;
	guardedCommand();
	lookAheadToken();
	if(nextTok.getSymbol() == GC1)
	{
		match(GC1);
		guardedCommandList();
	}
	
}

void Parser::guardedCommand()
{
	outFile<<"guardedCommand()"<<endl;
	expression();
	if(!match(GC2))
	{
		cerr<<"Syntax Error: "<<" '->' is missing at line "<< lineNo <<endl;
		ErrorCount();
		syntaxError(7);
	}
	lookAheadToken();
	statementPart();
}



void Parser::assignmentStatement()
{
	outFile<<"assignmentStatement()"<<endl;
	variableAccessList();
	if(!match(ASSIGN))
	{
		cerr<<"Syntax Error: "<<" ':=' is missing at line "<< lineNo <<endl;
		ErrorCount();
		syntaxError(11);
	}	
	expressionList();
}

void Parser::expressionList()
{
	outFile<<"expressionList()"<<endl;
	expression();
	if(match(COMMA))
	{
		expressionList();
	}
	else
	{
		//outFile<<"Ignore "<<endl;
	}
}

void Parser::variableAccessList()
{
	outFile<<"variableAccessList()"<<endl;
	variableAccess();
	if(match(COMMA))
	{
		variableAccessList();
	}
	else
	{
		//outFile<<"Ignore "<<endl;
	}
	
	
}

void Parser::variableAccess()
{
	outFile<<"variableAccess()"<<endl;	
	if(match(ID))
	{
		if(nextTok.getIDtype() != 1)
		{	
			//error message
			cerr<<"Syntax Error: "<<"expecting name of variable type at line "<< lineNo <<endl;
			ErrorCount();
		}
	}
	else
	{
		cerr<<"Syntax Error: "<<" variable name is missing at line "<< lineNo <<endl;
		ErrorCount();
		syntaxError(10);
	}
	lookAheadToken();
	if(ff.firstOfIndexSel(nextTok))
	{
		match(LEFTBRACKET);
		expression();
		if(!match(RIGHTBRACKET))
		{
			cerr<<"Syntax Error: "<<" ']' is missing at line "<< lineNo <<endl;
			ErrorCount();
			syntaxError(12);
		}
	}
}

void Parser::expression()
{
	outFile<<"expression()"<<endl;	
	primaryExpression();
	
	lookAheadToken();
	if(ff.firstOfPrimOp(nextTok))
	{
		primaryOperator();
		expression();
		
				
	}
	
}

void Parser::primaryExpression()
{

	outFile<<"primaryExpression()"<<endl;	
	simpleExpression();
	
	lookAheadToken();
	if(ff.firstOfRelOp(nextTok))
	{
		relationalOperator();
		simpleExpression();
	}
}	

void Parser::relationalOperator()
{
	outFile<<"relationalOperator()"<<endl;
	if(match(LESST) || match(EQUAL) || match(GREATERT) || match(LTE) || match(GTE))
	{
		//outFile<<"Ignore"<<endl;	
	}
}

void Parser::simpleExpression()
{
	outFile<<"simpleExpression()"<<endl;
	lookAheadToken();
	if(!ff.firstOfTerm(nextTok))
	{
		match(MINUS);	
	}
	term();
	lookAheadToken();	
	if(ff.firstOfAddOp(nextTok))
	{
		addopTerm();
				
	}
	
}

void Parser::addopTerm()
{
	lookAheadToken();
	if(ff.firstOfAddOp(nextTok))
	{
		addingOperator();
		term();
		addopTerm();
	}
	
}

void Parser::addingOperator()
{
	outFile<<"addingOperator()"<<endl;
	if(match(PLUS) || match(MINUS))
	{
		outFile<<"Ignore"<<endl;	
	}
	else
	{
		outFile<<"Error msg"<< endl;
	}
}

void Parser::term()
{
	outFile<<"term()"<<endl;
	outFile<<"Call factor()"<<endl;
	factor();
	lookAheadToken();
	if(ff.firstOfMultOp(nextTok))
	{
		multiplyingOperator();
		term();
	}
}

void Parser::factor()
{
	outFile<<"factor()"<<endl;
	lookAheadToken();
	if(ff.firstOfConstant(nextTok))
	{
		constant();	
	}
	else if(nextTok.getSymbol() == ID)
	{
		variableAccess();	
	}
	else if(nextTok.getSymbol() == LEFTP)
	{
		match(LEFTP);
		expression();	
		if(!match(RIGHTP))
		{
			cerr<<"Syntax Error: "<<" ')' is missing at line "<< lineNo <<endl;
			ErrorCount();
			syntaxError(12);
		}
	}
	else if(nextTok.getSymbol() == NOT)
	{
		match(NOT);
		factor();	
	}
		
}

void Parser::multiplyingOperator()
{
	outFile<<"multiplyingOperator()"<<endl;
	if(match(TIMES) || match(DIV) || match(MOD))
	{
		outFile<<"Ignore"<<endl;	
	}
	else
	{
		outFile<<"Error msg"<< endl;
	}
}

void Parser::primaryOperator()
{
	outFile<<"primaryOperator()"<<endl;
	if(match(AND) || match(OR))
	{
		outFile<<"Ignore"<<endl;
	}
	else
	{
		//error message
	}
}

void Parser::constant()
{
	outFile<<"constant()"<<endl;
	if(match(NUMERAL) || match(TRUE) || match(FALSE) || match(ID))
	{
		//do nothing
		//outFile<<"Ignore Previous Matching errors"<<endl;	
		if(nextTok.getSymbol() == ID && nextTok.getIDtype() != 2)
		{
			cerr<<"Syntax Error: "<<"expecting name of constant type at line "<< lineNo <<endl;
			ErrorCount();	
		}	
	}
	
	else
	{	//error msg
		if(nextTok.getSymbol()!=ID)
		{
			cerr<<"Syntax Error: "<<" Expecting numeral/boolean symbol at line "<< lineNo <<endl;
			ErrorCount();
			syntaxError(8);
		}
		
	}
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
		//outFile<<"MATCHED "<<nextTok.getSymbol()<<endl;
		return true;
	}
	
	else
	{
		outFile<<"Want to match "<<nextTok.getSymbol()<<" Symbol:"<<nextTok.getLexeme() <<" But got "<<sym<<"  Error Message"<<endl;
		//this means a token is not matched a lookahead token is grabbed		
		islookAheadTok = true;
		//outFile<<<<endl;
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

//Syntex Error recovery
void Parser::syntaxError(int code)
{
	switch(code)
	{
		case 1:
			while(1)
			{
				if(ff.firstOfDefinition(nextTok) || ff.firstOfStatement(nextTok) || nextTok.getSymbol() == END 
					|| nextTok.getSymbol() == ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
			break;
		case 2:
			while(1)
			{
				if(nextTok.getSymbol() == SEMICOLON || nextTok.getSymbol() == DOT || ff.firstOfStatement(nextTok) 
					|| nextTok.getSymbol() == ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
			break;
		case 3:
			while(1)
			{
				if(ff.firstOfDefinition(nextTok) || ff.followOfDefPart(nextTok) 
					|| nextTok.getSymbol() == ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
			break;
		case 4:
			while(1)
			{
				if(nextTok.getSymbol() == SEMICOLON || nextTok.getSymbol() == EQUAL || ff.firstOfDefinition(nextTok) 						|| ff.followOfDefPart(nextTok) || nextTok.getSymbol() == ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
			break;
		case 5:
			while(1)
			{
				if(nextTok.getSymbol() == SEMICOLON || ff.firstOfDefinition(nextTok) 						|| ff.followOfDefPart(nextTok) || nextTok.getSymbol() == ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
			break;
			break;
		case 6:
			while(1)
			{
				if(nextTok.getSymbol() == SEMICOLON || ff.firstOfDefinition(nextTok) 						|| ff.firstOfConstant(nextTok) || ff.followOfDefPart(nextTok) || nextTok.getSymbol() == 							ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
			break;
		case 7:
			while(1)
			{
				if(ff.followOfDefPart(nextTok) || ff.followOfStatePart(nextTok) || nextTok.getSymbol() == ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<" Lexeme: "<< nextTok.getLexeme()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
			break;
		case 8:
			while(1)
			{
				if(nextTok.getSymbol() == RIGHTBRACKET || nextTok.getSymbol() == SEMICOLON 
					|| ff.firstOfDefinition(nextTok) || ff.followOfDefPart(nextTok) 
					|| nextTok.getSymbol() == ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
			break;
		case 9:
			while(1)
			{
				if(nextTok.getSymbol() == BEGIN || nextTok.getSymbol() == ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
			break;
		case 10:
			while(1)
			{
				if(nextTok.getSymbol() == SEMICOLON || ff.followOfDefPart(nextTok)
					||  nextTok.getSymbol() == ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
			break;	
		case 11:
			while(1)
			{
				if(nextTok.getSymbol() == SEMICOLON || ff.firstOfExpList(nextTok)
					||  nextTok.getSymbol() == ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
		case 12:
			while(1)
			{
				if(nextTok.getSymbol() == SEMICOLON || nextTok.getSymbol() == GC2 
					||nextTok.getSymbol() == RIGHTBRACKET ||nextTok.getSymbol() == RIGHTP  
					|| ff.firstOfExpList(nextTok) ||  nextTok.getSymbol() == ENDOFFILE)
				{	
					cout<<"Stopped At: "<< nextTok.getSymbol()<<endl;				
					break;
				}
				else
				{
					//islookAheadTok = false;				
					nextTok = scanner.nextToken();
					if(nextTok.getSymbol() == NEWLINE)
					{
						NewLine();
					}
				}
			}
		
		

		
	}
}

void Parser::ErrorCount()
{	
	errorCount++;
}	
//write error messages to output file	
//void Parser::error(string text)
//{
	
//}	


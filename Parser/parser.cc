#include "parser.h"

//constructor
Parser::Parser(ifstream &in, ofstream &out, Scanner &sc)
:srcFile(in),outFile(out),scanner(sc),islookAheadTok(false)
{
}

void Parser::program()
{
	lineNo = 1;	
	errorCount = 0;
	outFile<<"program()"<<endl;	
	block();	
	//outFile<<"Matched ."<<endl;
	if(!match(DOT))
	{
		cerr<<"Syntax Error: "<<"'.' (dot) is missing at line "<< (lineNo - 1) <<endl;
		ErrorCount();	
	}

	outFile<<"Total no of lines: "<<lineNo<<endl;
	
}

void Parser::block()
{
	outFile<<"block()"<<endl;
	//outFile<<"Matched begin"<<endl;		
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
		/*if(ff.firstOfStatement(nextTok))		
		{
			outFile<<"Call statementPart()"<<endl;
			statementPart();
		}
		*/
		//do nothing
	}
	else
	{
		//error message
		cerr<<"Syntax Error: "<<"Illegal start of definition/statement at line "<< lineNo <<endl;
		ErrorCount();
	}

	if(!match(END))
	{
		cerr<<"Syntax Error: "<<"end is missing at line "<< lineNo <<endl;
		ErrorCount();
		syntaxError(2);
	}	
	
	
}

void Parser::definitionPart()
{
	
	outFile<<"definitionPart()"<<endl;
	//outFile<<"Matched semicolon"<<endl;
	
	lookAheadToken();	
	if(ff.firstOfDefinition(nextTok))
	{
		
		definition();
		if(!match(SEMICOLON))
		{
			cerr<<"Syntax Error: "<<" ';' is missing at line "<< lineNo <<endl;
			ErrorCount();
			syntaxError(3);
		}		
		definitionPart();
	}
	
	/*else if(ff.followOfDefPart())
	{
		//do Nothing	
	}*/	
	
}

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





void Parser::constantDefinition()
{
	outFile<<"constantDefinition()"<<endl;
	//outFile<<"Matched const"<<endl;
	match(CONST);
	//outFile<<"Matched ID"<<endl;
	if(match(ID))
	{
		if(nextTok.getIDtype() != 2)
		{	
			//error message
			//cout
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
		match(LEFTBRACKET);
		constant();
		match(RIGHTBRACKET);		
	}
	else
	{

	}
}



void Parser::typeSymbol()
{
	outFile<<"typeSymbol()"<<endl;
	if(match(INT) || match(BOOL))
	{
		//do nothing		
		outFile<<"Ignore Matching errors"<<endl;
	}
	else
	{	//error msg
		outFile<<"Error in typeSymbol"<<endl;
	}
	
}

void Parser::variableList()
{
	outFile<<"variableList()"<<endl;
	if(match(ID))
	{
		if(nextTok.getIDtype() != 1)
		{	
			//error message
			//cout
		}
	}
	//lookAheadToken();
	if(match(COMMA))
	{
		variableList();
	}
	else
	{
		outFile<<"Ignore"<<endl;
	}
	
}

void Parser::statementPart()
{
	outFile<<"statementPart()"<<endl;
	lookAheadToken();	
	if(ff.firstOfStatement(nextTok))
	{
		statement();
		match(SEMICOLON);
		//outFile<<"Hola "<<nextTok.getSymbol()<<endl;	
		statementPart();
	}
	else if(ff.followOfStatePart(nextTok))
	{
		//do nothing
		outFile<<"statementPart() called but 0 statement found"<<endl;
	}
	else
	{
		// error message
	}
}

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
				//cout
			}
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
	else
	{
		//error message	
	}
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
	match(FI);
	
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
	match(GC2);
	lookAheadToken();
	statementPart();
}


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
void Parser::assignmentStatement()
{
	outFile<<"assignmentStatement()"<<endl;
	variableAccessList();
	match(ASSIGN);
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
		outFile<<"Ignore "<<endl;
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
		outFile<<"Ignore "<<endl;
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
			//cout
		}
	}
	lookAheadToken();
	if(ff.firstOfIndexSel(nextTok))
	{
		match(LEFTBRACKET);
		expression();
		match(RIGHTBRACKET);
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
		outFile<<"Ignore"<<endl;	
	}
	else
	{
		outFile<<"Error msg"<< endl;
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
		match(RIGHTP);
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
		outFile<<"Ignore Previous Matching errors"<<endl;		
	}
	
	else
	{	//error msg
		
	}
}

void Parser::procedureDefinition()
{
	outFile<<"procedureDefinition()"<<endl;
	match(PROC);
	if(match(ID))
	{
		if(nextTok.getIDtype() != 3)
		{	
			//error message
			//cout
		}
	}
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
					nextTok = scanner.nextToken();
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
					nextTok = scanner.nextToken();
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
					nextTok = scanner.nextToken();
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
					nextTok = scanner.nextToken();
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
					nextTok = scanner.nextToken();
			}
			break;
	}
}

void Parser::ErrorCount()
{	
	errorCount++;
}	
//write error messages to output file	
void Parser::error(string text)
{
	
}	


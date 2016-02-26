#include "parser.h"

//constructor
Parser::Parser(ifstream &in, ofstream &out, Scanner &sc)
:srcFile(in),outFile(out),scanner(sc),islookAheadTok(false)
{
}

void Parser::program()
{
		
	cout<<"program()"<<endl;	
	block();	
	//cout<<"Matched ."<<endl;
	match(DOT);
	
}

void Parser::block()
{
	cout<<"block()"<<endl;
	//cout<<"Matched begin"<<endl;		
	match(BEGIN);	

	lookAheadToken();
	if(ff.firstOfDefinition(nextTok))
	{	
		
		definitionPart();
		statementPart();
	
	}
	else if(ff.firstOfStatement(nextTok))
	{
		
		statementPart();
		/*if(ff.firstOfStatement(nextTok))		
		{
			cout<<"Call statementPart()"<<endl;
			statementPart();
		}
		*/
		//do nothing
	}
	else
	{
		//error message
	}
	match(END);
	
}

void Parser::definitionPart()
{
	
	cout<<"definitionPart()"<<endl;
	//cout<<"Matched semicolon"<<endl;
	
	lookAheadToken();	
	if(ff.firstOfDefinition(nextTok))
	{
		
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
	cout<<"definition()"<<endl;
	
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
	cout<<"constantDefinition()"<<endl;
	//cout<<"Matched const"<<endl;
	match(CONST);
	//cout<<"Matched ID"<<endl;
	if(match(ID))
	{
		if(nextTok.getIDtype() != 2)
		{	
			//error message
			//cout
		}
	}
	//cout<<"Matched ="<<endl;	
	match(EQUAL);
	
	
	constant();
	
}

void Parser::variableDefinition()
{
	cout<<"variableDefinition()"<<endl;	
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
}



void Parser::typeSymbol()
{
	cout<<"typeSymbol()"<<endl;
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
	cout<<"variableList()"<<endl;
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
		cout<<"Ignore"<<endl;
	}
	
}

void Parser::statementPart()
{
	cout<<"statementPart()"<<endl;
	lookAheadToken();	
	if(ff.firstOfStatement(nextTok))
	{
		statement();
		match(SEMICOLON);
		//cout<<"Hola "<<nextTok.getSymbol()<<endl;	
		statementPart();
	}
	else if(ff.followOfStatePart(nextTok))
	{
		//do nothing
		cout<<"statementPart() called but 0 statement found"<<endl;
	}
	else
	{
		// error message
	}
}

void Parser::statement()
{
	cout<<"statement()"<<endl;
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
	cout<<"doStatement()"<<endl;
	match(DO);
	guardedCommandList();
	match(OD);
}

void Parser::ifStatement()
{
	cout<<"ifStatement()"<<endl;
	match(IF);
	guardedCommandList();
	match(FI);
	
}

void Parser::guardedCommandList()
{
	cout<<"guardedCommandList()"<<endl;
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
	cout<<"guardedCommand()"<<endl;
	expression();
	match(GC2);
	lookAheadToken();
	statementPart();
}


void Parser::readStatement()
{
	cout<<"readStatement()"<<endl;
	match(READ);
	variableAccessList();
			
}

void Parser::writeStatement()
{
	cout<<"writeStatement()"<<endl;
	match(WRITE);
	expressionList();
	
		
}
void Parser::assignmentStatement()
{
	cout<<"assignmentStatement()"<<endl;
	variableAccessList();
	match(ASSIGN);
	expressionList();
}

void Parser::expressionList()
{
	cout<<"expressionList()"<<endl;
	expression();
	if(match(COMMA))
	{
		expressionList();
	}
	else
	{
		cout<<"Ignore "<<endl;
	}
}

void Parser::variableAccessList()
{
	cout<<"variableAccessList()"<<endl;
	variableAccess();
	if(match(COMMA))
	{
		variableAccessList();
	}
	else
	{
		cout<<"Ignore "<<endl;
	}
	
	
}

void Parser::variableAccess()
{
	cout<<"variableAccess()"<<endl;	
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
	cout<<"expression()"<<endl;	
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

	cout<<"primaryExpression()"<<endl;	
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
	cout<<"relationalOperator()"<<endl;
	if(match(LESST) || match(EQUAL) || match(GREATERT) || match(LTE) || match(GTE))
	{
		cout<<"Ignore"<<endl;	
	}
	else
	{
		cout<<"Error msg"<< endl;
	}
}

void Parser::simpleExpression()
{
	cout<<"simpleExpression()"<<endl;
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
	cout<<"addingOperator()"<<endl;
	if(match(PLUS) || match(MINUS))
	{
		cout<<"Ignore"<<endl;	
	}
	else
	{
		cout<<"Error msg"<< endl;
	}
}

void Parser::term()
{
	cout<<"term()"<<endl;
	cout<<"Call factor()"<<endl;
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
	cout<<"factor()"<<endl;
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
	cout<<"multiplyingOperator()"<<endl;
	if(match(TIMES) || match(DIV) || match(MOD))
	{
		cout<<"Ignore"<<endl;	
	}
	else
	{
		cout<<"Error msg"<< endl;
	}
}

void Parser::primaryOperator()
{
	cout<<"primaryOperator()"<<endl;
	if(match(AND) || match(OR))
	{
		cout<<"Ignore"<<endl;
	}
	else
	{
		//error message
	}
}

void Parser::constant()
{
	cout<<"constant()"<<endl;
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
	cout<<"procedureDefinition()"<<endl;
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
		//cout<<"MATCHED "<<nextTok.getSymbol()<<endl;
		return true;
	}
	
	else
	{
		cout<<"Want to match "<<nextTok.getSymbol()<<" Symbol:"<<nextTok.getLexeme() <<" But got "<<sym<<"  Error Message"<<endl;
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


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

	lookAheadToken();
	if(ff.firstOfDefinition(nextTok.getSymbol()))
	{	
		cout<<"Call definitionPart()"<<endl;
		definitionPart();
	
		cout<<"Call statementPart()"<<endl;
		statementPart();
	
	}
	else if(ff.firstOfStatement(nextTok.getSymbol()))
	{
		
		statementPart();
		/*if(ff.firstOfStatement(nextTok.getSymbol()))		
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
	else if(match(ARRAY))
	{
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
	//lookAheadToken();
	if(match(COMMA))
	{
		cout<<"Call varableList()"<<endl;
		variableList();
	}
	else
	{
		cout<<"Ignore"<<endl;
	}
	
}

void Parser::statementPart()
{
	
	lookAheadToken();	
	if(ff.firstOfStatement(nextTok.getSymbol()))
	{
		cout<<"Call statement()"<<endl;
		statement();
		match(SEMICOLON);
		//cout<<"Hola "<<nextTok.getSymbol()<<endl;	
		statementPart();
	}
	else if(ff.followOfStatePart(nextTok.getSymbol()))
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
	lookAheadToken();	
	if(ff.firstOfEmptySt(nextTok.getSymbol()))
	{
		match(SKIP);
	}
	else if(ff.firstOfReadSt(nextTok.getSymbol()))
	{
		cout<<"call readStatement()"<<endl;
		readStatement();
	}
	else if(ff.firstOfWriteSt(nextTok.getSymbol()))
	{
		cout<<"call writeStatement()"<<endl;
		writeStatement();
	}
	else if(ff.firstOfProcSt(nextTok.getSymbol()))
	{
		match(CALL);
		match(ID);
	}
	else if(ff.firstOfAssignSt(nextTok.getSymbol()))
	{
		cout<<"call assignmentStatement()"<<endl;
		assignmentStatement();
	}
	else if(ff.firstOfIfSt(nextTok.getSymbol()))
	{
		cout<<"call ifStatement()"<<endl;
		ifStatement();		
	}
	else if(ff.firstOfDoSt(nextTok.getSymbol()))
	{
		cout<<"call doStatement()"<<endl;
		doStatement();		
	}
	else
	{
		//error message	
	}
}

void Parser::doStatement()
{
	match(DO);
	cout<<"call guardedCommandList()"<<endl;
	guardedCommandList();
	match(OD);
}

void Parser::ifStatement()
{
	match(IF);
	cout<<"call guardedCommandList()"<<endl;
	guardedCommandList();
	match(FI);
	
}

void Parser::guardedCommandList()
{
	cout<<"call guardedCommand()"<<endl;
	guardedCommand();
	lookAheadToken();
	if(nextTok.getSymbol() == GC1)
	{
		match(GC1);
		cout<<"call guardedCommandList()"<<endl;
		guardedCommandList();
	}
}

void Parser::guardedCommand()
{
	cout<<"call expression()"<<endl;
	expression();
	match(GC2);
	lookAheadToken();
	cout<<"call statementPart()"<<endl;
	statementPart();
}


void Parser::readStatement()
{
	match(READ);
	cout<<"call variableAccessList()"<<endl;
	variableAccessList();
	
		
}

void Parser::writeStatement()
{
	match(WRITE);
	cout<<"call expressionList()"<<endl;
	expressionList();
	
		
}
void Parser::assignmentStatement()
{
	cout<<"call variableAccessList()"<<endl;
	variableAccessList();
	match(ASSIGN);
	cout<<"call expressionList()"<<endl;
	expressionList();
}

void Parser::expressionList()
{
	cout<<"call expression()"<<endl;
	expression();
	if(match(COMMA))
	{
		cout<<"Call expressionList()"<<endl;
		expressionList();
	}
	else
	{
		cout<<"Ignore "<<endl;
	}
}

void Parser::variableAccessList()
{
	cout<<"call variableAccess()"<<endl;
	variableAccess();
	if(match(COMMA))
	{
		cout<<"Call variableAccessList()"<<endl;
		variableAccessList();
	}
	else
	{
		cout<<"Ignore "<<endl;
	}
	
	
}

void Parser::variableAccess()
{
	match(ID);
	lookAheadToken();
	if(ff.firstOfIndexSel(nextTok.getSymbol()))
	{
		match(LEFTBRACKET);
		cout<<"Call expression()"<<endl;		
		expression();
		match(RIGHTBRACKET);
	}
}

void Parser::expression()
{
	cout<<"Call primaryExpression()"<<endl;	
	primaryExpression();
	
	lookAheadToken();
	if(ff.firstOfPrimOp(nextTok.getSymbol()))
	{
		cout<<"Call primaryOperator()"<<endl;
		primaryOperator();
		cout<<"Call primaryExpression()"<<endl;
		expression();
				
	}
	
}

void Parser::primaryExpression()
{

	cout<<"Call simpleExpression()"<<endl;	
	simpleExpression();
	
	lookAheadToken();
	if(ff.firstOfRelOp(nextTok.getSymbol()))
	{
		cout<<"Call relationalOperator()"<<endl;	
		relationalOperator();
		cout<<"Call primaryExpression()"<<endl;	
		primaryExpression();
	}
}	

void Parser::relationalOperator()
{
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
	lookAheadToken();
	if(!ff.firstOfTerm(nextTok.getSymbol()))
	{
		match(MINUS);	
	}
	cout<<"Call term()"<<endl;	
	term();
	lookAheadToken();	
	if(ff.firstOfAddOp(nextTok.getSymbol()))
	{
		cout<<"Call addopTerm()"<<endl;	
		addopTerm();
				
	}
	
}

void Parser::addopTerm()
{
	lookAheadToken();
	if(ff.firstOfAddOp(nextTok.getSymbol()))
	{
		cout<<"Call addingOperator()"<<endl;	
		addingOperator();
		cout<<"Call addopTerm()"<<endl;			
		term();
		cout<<"Call term()"<<endl;	
		addopTerm();
	}
	
}

void Parser::addingOperator()
{
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
	cout<<"Call factor()"<<endl;
	factor();
	lookAheadToken();
	if(ff.firstOfMultOp(nextTok.getSymbol()))
	{
		cout<<"Call multiplyingOperator()"<<endl;
		multiplyingOperator();
		cout<<"Call term()"<<endl;
		term();
	}
}

void Parser::factor()
{
	lookAheadToken();
	if(ff.firstOfConstant(nextTok.getSymbol()))
	{
		cout<<"Call constant()"<<endl;
		constant();	
	}
	else if(nextTok.getSymbol() == ID)
	{
		cout<<"Call variableAccess()"<<endl;
		variableAccess();	
	}
	else if(nextTok.getSymbol() == LEFTP)
	{
		match(LEFTP);
		cout<<"Call expression()"<<endl;	
		expression();	
		match(RIGHTP);
	}
	else if(nextTok.getSymbol() == NOT)
	{
		match(NOT);
		cout<<"Call factor()"<<endl;
		factor();	
	}
		
}

void Parser::multiplyingOperator()
{
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


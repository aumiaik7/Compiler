#include "parser.h"

//constructor
Parser::Parser(ifstream &in, ofstream &out, Scanner &sc)
:srcFile(in),outFile(out),scanner(sc),islookAheadTok(false)
{
	st.push_back(ENDOFFILE);
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
	vector<Symbol> newStops;
	//stops + newStops; 
	newStops = st + DOT;
	//cout<<newStops[1]<<endl;
	outFile<<"program()"<<endl;	
	block(newStops);	
	
	match(DOT);
	
	//parsing done
	done();
		
}

// block = 'begin' definitionPart statementPart 'end'
void Parser::block(vector<Symbol> stops)
{
	outFile<<"block()"<<endl;
	match(BEGIN);
	
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

	//this portion matches end and if not matched the shows corresponding error message
	match(END);
}

// definitionPart = {definition';'}
void Parser::definitionPart()
{
	
	outFile<<"definitionPart()"<<endl;
	
	lookAheadToken();	
	if(ff.firstOfDefinition(nextTok))
	{
		definition();
		match(SEMICOLON);
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
		//id of type 2 is of constant type. if not matched the shows the corresponding error message 
		if(nextTok.getIDtype() != 2)
		{	
			//error message
			cerr<<"Syntax Error: "<<"expecting name of constant type at line "<< lineNo <<endl;
			
		}
		
	}
	
		
	match(EQUAL);
	
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
		match(LEFTBRACKET);
		constant();
		match(RIGHTBRACKET);
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
			
		}
	}
	
	block(st);
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
		match(SEMICOLON);
		statementPart();
	}
	else if(ff.followOfStatePart(nextTok))
	{
		//do nothing
	}
	else
	{
		
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
	
}

// readStatement = 'read' variableAccessList
void Parser::readStatement()
{
	outFile<<"readStatement()"<<endl;
	match(READ);
	variableAccessList();
			
}

// variableAccessList = variableAccess{','variableAccess}
void Parser::variableAccessList()
{
	outFile<<"variableAccessList()"<<endl;
	variableAccess();
	if(match(COMMA))
	{
		variableAccessList();
	}
}

// writeStatement = 'write' expressionList  
void Parser::writeStatement()
{
	outFile<<"writeStatement()"<<endl;
	match(WRITE);
	expressionList();
}

// expressionList = expression {','expression}
void Parser::expressionList()
{
	outFile<<"expressionList()"<<endl;
	expression();
	if(match(COMMA))
	{
		expressionList();
	}
	
}

// assignmentStatement = variableAccessList ':=' expressionList
void Parser::assignmentStatement()
{
	outFile<<"assignmentStatement()"<<endl;
	variableAccessList();
	match(ASSIGN);	
	expressionList();
}

// ifStatement = 'if' guardedCommandList 'fi' 
void Parser::ifStatement()
{
	outFile<<"ifStatement()"<<endl;
	match(IF);
	guardedCommandList();
	match(FI);	
}

// doStatement = 'do' guardedCommandList 'od'
void Parser::doStatement()
{
	outFile<<"doStatement()"<<endl;
	match(DO);
	guardedCommandList();
	match(OD);
}

// guardedCommandList = guardedCommand{'[]'guardedCommand}
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

// guardedCommand = expression '->' statementPart
void Parser::guardedCommand()
{
	outFile<<"guardedCommand()"<<endl;
	expression();
	match(GC2);
	lookAheadToken();
	statementPart();
}

// expression = primaryExpression{primaryOperator primaryExpression}	
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

// primaryOperator = '&' | '|'
void Parser::primaryOperator()
{
	outFile<<"primaryOperator()"<<endl;
	if(match(AND) || match(OR))
	{
		
	}
}

// primaryExpression = simpleExpression [relationalOperator simpleExpression]
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

// relationalOperator = '<' | '=' | '>'	
void Parser::relationalOperator()
{
	outFile<<"relationalOperator()"<<endl;
	if(match(LESST) || match(EQUAL) || match(GREATERT) || match(LTE) || match(GTE))
	{
		
	}
}

// simpleExpression = ['-'] term {addingOperator term}
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

// addingOperator = '+' | '-'
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

//helps to execute {addingOperator term}
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

// term = factor {multiplyingOperator factor}
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

// multiplyingOperator = '*' | '/' | '\'
void Parser::multiplyingOperator()
{
	outFile<<"multiplyingOperator()"<<endl;
	if(match(TIMES) || match(DIV) || match(MOD))
	{
		
	}
	
}

//factor = constant | variableAccess | '('expression')' | '~' factor	
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

//variableAccess = variableName [indexSelector]
void Parser::variableAccess()
{
	outFile<<"variableAccess()"<<endl;	
	if(match(ID))
	{
		if(nextTok.getIDtype() != 1)
		{	
			//error message
			
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

// constanr = numeral | booleanSymbol | constantName
void Parser::constant()
{
	outFile<<"constant()"<<endl;
	if(match(NUMERAL) || match(TRUE) || match(FALSE) || match(ID))
	{
		if(nextTok.getSymbol() == ID && nextTok.getIDtype() != 2)
		{
			
		}	
	}
	
	
}


//this function is used to match terminal symbols of CFG
bool Parser::match(Symbol sym)
{
	//look ahead token flag is disable is currently eanble and dont 
	//grab a new token as already grabbed using lookAheadToken() 
	if(islookAheadTok)
		islookAheadTok = false;
	else	
		nextTok = scanner.nextToken();

	//grab the token other than newline or noname
	if(nextTok.getSymbol() == NEWLINE || nextTok.getSymbol() == NONAME || nextTok.getSymbol() == EOF)
	{		
		
		while(1)
		{	
			//newline detected			
			if(nextTok.getSymbol() == NEWLINE)
			{
				NewLine();
			}
			else if(nextTok.getSymbol() != NONAME)
				break;
			nextTok = scanner.nextToken();
		}
		
	}
	
	//it means the token is matched returns true
	if(nextTok.getSymbol() == sym)
	{
		return true;
	}
	
	//not matched returns false
	else
	{
		
		//this means a token is not matched a lookahead token is grabbed		
		islookAheadTok = true;
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
	if(nextTok.getSymbol() == NEWLINE || nextTok.getSymbol() == NONAME || nextTok.getSymbol() == EOF)
	{		
		
		while(1)
		{	
			//newline detected
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
}

//Syntex Error recovery
// when any error occured corresponding error msg is displayed and 
// we try find the next possible symbol (stop symbol) that can occur
// after the error. For different kinds of terminal symbol stop 
// symbols are different. The different sets of stop symbols finding
// is done in this switch cases  


void Parser::ErrorCount()
{	
	errorCount++;
	if(errorCount >= MAXERRORS)
	{
		cerr<<"Too many errors. Bailing out!!\n";
		outFile<<"Too many errors. Bailed out!!";
		srcFile.close();
		outFile.close();
		exit(0);
	}
	
}	
void Parser::done()
{
	cout<<endl<<"<<<Parsing Done>>>"<<endl;
	cout<<"The methods called for nonterminals are showed in ParseOutFile "<<endl;
	outFile<<"Total no of lines: "<<lineNo<<endl;
	srcFile.close();
	outFile.close();
}

int operator+(vector<Symbol>& stops, vector<Symbol> newStops)
{
	cout<<"Ti amo!"<<endl;


	return 0;
	//stops = vector<Symbol>(sym, sy + sizeof(der1) / sizeof(der1[0]) );
}
vector<Symbol> operator+(vector<Symbol> stops, Symbol sym)
{
	stops.push_back(sym);
	cout<<"Ti amor!"<<endl;
	
	return stops;
	//stops = vector<Symbol>(sym, sy + sizeof(der1) / sizeof(der1[0]) );
}
//write error messages to output file	
//void Parser::error(string text)
//{
	
//}	


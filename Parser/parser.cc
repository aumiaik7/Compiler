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


	newStops = st + st + st;
	
	//stops + newStops; 
	//newStops = st + DOT;
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
	if(in(ff.firstOfDefinition()))
	{	
		definitionPart();
		statementPart();
	}
	else if(in(ff.followOfDefPart()))
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
	if(in(ff.firstOfDefinition()))
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
	if(in(ff.firstOfConstDef()))
	{
		constantDefinition();
	}
	else if(in(ff.firstOfVariDef()))
	{
		variableDefinition();
	}
	else if(in(ff.firstOfProcDef()))
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
	if(in(ff.firstOfVariList()))
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
	if(in(ff.firstOfStatement()))
	{
		statement();
		match(SEMICOLON);
		statementPart();
	}
	else if(in(ff.followOfStatePart()))
	{
		//do nothing
	}
	
}

// statement = emptyStatement | readStatement | writeStatement |  assignmentStatement | ifStatement | doStatement 
void Parser::statement()
{
	outFile<<"statement()"<<endl;
	lookAheadToken();	
	if(in(ff.firstOfEmptySt()))
	{
		match(SKIP);
	}
	else if(in(ff.firstOfReadSt()))
	{
		readStatement();
	}
	else if(in(ff.firstOfWriteSt()))
	{
		writeStatement();
	}
	else if(in(ff.firstOfProcSt()))
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
	else if(in(ff.firstOfAssignSt()))
	{
		assignmentStatement();
	}
	else if(in(ff.firstOfIfSt()))
	{
		ifStatement();		
	}
	else if(in(ff.firstOfDoSt()))
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
	if(in(ff.firstOfPrimOp()))
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
	if(in(ff.firstOfRelOp()))
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
	if(!in(ff.firstOfTerm()))
	{
		match(MINUS);	
	}
	term();
	lookAheadToken();	
	if(in(ff.firstOfAddOp()))
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
	if(in(ff.firstOfAddOp()))
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
	if(in(ff.firstOfMultOp()))
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
	if(nextTok.getSymbol() == ID)
	{
		if(nextTok.getIDtype() == 1)
			variableAccess();
		else if(nextTok.getIDtype() == 2)
			constant();
	}
	else if(in(ff.firstOfConstant()))
	{
		constant();	
	}
	/*else if(nextTok.getSymbol() == ID && nextTok.getIDtype() == 1)
	{
		variableAccess();	
	}
	*/
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
	else
	{
		cout<<"Kenu Kenu Kenu"<<endl;
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
	if(in(ff.firstOfIndexSel()))
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
		//outFile<<"Matched" << nextTok.getSymbol()<<endl;
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

bool Parser::in(vector<Symbol> set)
{
	for(int i = 0; i < set.size(); i++)
	{
		if(nextTok.getSymbol() == set.at(i))
			return true;
	}
	
	return false;
}


//+ operator overloading
/*vector<Symbol> operator+(vector<Symbol> set1, vector<Symbol> set2)
{
	vector<Symbol> mergedSet;
	mergedSet.reserve(set1.size() + set2.size());
	mergedSet.insert(mergedSet.end(), set1.begin(), set1.end());
	mergedSet.insert(mergedSet.end(), set2.begin(), set2.end());

	return mergedSet;
}
*/
/*vector<Symbol> operator+(vector<Symbol> stops, Symbol sym)
{
	stops.push_back(sym);
	cout<<"Ti amor!"<<endl;
	
	return stops;
	//stops = vector<Symbol>(sym, sy + sizeof(der1) / sizeof(der1[0]) );
}*/



//write error messages to output file	
//void Parser::error(string text)
//{
	
//}	


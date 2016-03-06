#include "parser.h"

//constructor
Parser::Parser(ifstream &in, ofstream &out, Scanner &sc)
:srcFile(in),outFile(out),scanner(sc),islookAheadTok(false)
{
	agc = false;
	lookAheadToken();
}

/*
  Imoplementation of the CFG is self explanatory
  for non terminal ther are corresponding methods
  and terminal symbols are matched using match(Symbol)
  method				
*/

// program = block '.'
void Parser::program(Symbol sym)
{
	
	lineNo = 1;	
	errorCount = 0;
	
	stopSet.push_back(sym);
	stopSet.push_back(DOT);
	
	outFile<<"program()"<<endl;	
	block(stopSet);	
	
	vector<Symbol>().swap(stopSet);
	

	stopSet.push_back(sym);
	
	match(DOT,stopSet);//, stopSet);

	
	//parsing done
	done();
		
}

// block = 'begin' definitionPart statementPart 'end'
void Parser::block(vector<Symbol> stops)
{
	outFile<<"block()"<<endl;
	match(BEGIN,ff.firstOfDefinition() + ff.firstOfStatement() + stops);// , ff.firstOfDefinition() + ff.firstOfStatement() + stops);
	
	//lookAheadToken();
	if(in(ff.firstOfDefinition()))
	{	
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(END);
		
		definitionPart(ff.firstOfStatement() + stops + stopSet);
		statementPart(stops + stopSet);
		
	}
	else if(in(ff.followOfDefPart()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(END);
		statementPart(stops + stopSet);
	}
	else
	{
		//error message
		cerr<<"Syntax Error: "<<"Illegal start of definition/statement at line "<< lineNo <<endl;
		ErrorCount();
	}

	//this portion matches end and if not matched the shows corresponding error message
	match(END,stops);
}

// definitionPart = {definition';'}
void Parser::definitionPart(vector<Symbol> stops)
{
	
	outFile<<"definitionPart()"<<endl;
	
	//lookAheadToken();
	
	if(in(ff.firstOfDefinition()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(SEMICOLON);	
		definition(stopSet + stops);
		match(SEMICOLON , stops + ff.firstOfDefinition());
		definitionPart(stops);
	}
	else
		syntaxCheck(stops);
	
		
}

// definition = constantDefinition | variableDefinition | procedureDefinition
void Parser::definition(vector<Symbol> stops)
{
	outFile<<"definition()"<<endl;
	
	//lookAheadToken();
	if(in(ff.firstOfConstDef()))
	{
		constantDefinition(stops);
	}
	else if(in(ff.firstOfVariDef()))
	{
		variableDefinition(stops);
	}
	else if(in(ff.firstOfProcDef()))
	{
		procedureDefinition(stops);
	}
}

// constantDefinition = 'const' constName '='  constant;
void Parser::constantDefinition(vector<Symbol> stops)
{
	outFile<<"constantDefinition()"<<endl;
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(EQUAL);
	//stopSet.push_back(ff.firstOfConstant());
	match(CONST , ff.firstOfConstant() + stopSet + stops);
	
	if(lookAheadTok.getSymbol() == ID && lookAheadTok.getIDtype() == 2)	
	{
		match(ID, stopSet + stops);	
	}
	else
	{
		match(ID, stopSet + stops);
		//cerr<<"ID: "<<lookAheadTok.getLexeme()<< " Type" << lookAheadTok.getIDtype() <<endl;
		cerr<<"Syntax Error: "<<"expecting name of constant type at line "<< lineNo <<endl;
	}		
		
	//vector<Symbol>().swap(stopSet);
	//stopSet.push_back(ff.firstOfConstant());
	
	match(EQUAL , ff.firstOfConstant() + stops);
	
	constant(stops);
	
}

// variableDefinition = typeSymbol variableList | typeSymbol 'array' variableList'['constant']'
void Parser::variableDefinition(vector<Symbol> stops)
{
	outFile<<"variableDefinition()"<<endl;	

	
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(ARRAY);
	typeSymbol(stopSet + ff.firstOfVariList() + stops);
	//lookAheadToken();
	if(in(ff.firstOfVariList()))
	{
		variableList(stops);
	}
	else if(lookAheadTok.getSymbol() == ARRAY)
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(LEFTBRACKET);
		stopSet.push_back(RIGHTBRACKET);

		match(ARRAY, stopSet + ff.firstOfVariList() + stops);
		
		
		variableList(stopSet + stops);
		
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(RIGHTBRACKET);
		match(LEFTBRACKET , stopSet + ff.firstOfConstant() + stops);
		
		constant(stopSet + stops);
		match(RIGHTBRACKET , stops);
	}
	
	
}

// procedureDefinition = 'proc' procedureName block
void Parser::procedureDefinition(vector<Symbol> stops)
{
	outFile<<"procedureDefinition()"<<endl;

	vector<Symbol>().swap(stopSet);
	stopSet.push_back(BEGIN);
	stopSet.push_back(ID);
	
	match(PROC, stopSet + stops);

	if(lookAheadTok.getSymbol() == ID && lookAheadTok.getIDtype() == 3)	
	{
		stopSet.pop_back();
		match(ID, stopSet + stops);	
	}
	else
	{
		match(ID, stopSet + stops);
		//outFile<<"ID: "<<lookAheadTok.getLexeme()<< " Type" << lookAheadTok.getIDtype() <<endl;
		cerr<<"Syntax Error: "<<"expecting name of constant type at line "<< lineNo <<endl;
	}
	
	
	block(stops);
}

// typeSymbol = 'integer' | 'Boolean'
void Parser::typeSymbol(vector<Symbol> stops)
{
	outFile<<"typeSymbol()"<<endl;

	if(lookAheadTok.getSymbol() == INT)
	{
		match(INT,stops);		
	}	
	else if(lookAheadTok.getSymbol() == BOOL)
	{
		match(BOOL,stops);
	}
		
}

// variableList = variableName {','variableName}
void Parser::variableList(vector<Symbol> stops)
{
	outFile<<"variableList()"<<endl;
	//outFile<<"Matched" << lookAheadTok.getSymbol()<<" Lexeme "<<lookAheadTok.getLexeme()<<endl;
	
	if(lookAheadTok.getSymbol() == ID && lookAheadTok.getIDtype() == 1)	
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(COMMA);
		match(ID, stopSet + stops);	
	}	
	else
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(COMMA);
		match(ID, stopSet + stops);
		//outFile<<"ID: "<<lookAheadTok.getLexeme()<< " Type" << lookAheadTok.getIDtype() <<endl;
		cerr<<"Syntax Error: "<<"expecting name of constant type at line "<< lineNo <<endl;
	}
	////lookAheadToken();
	if(lookAheadTok.getSymbol() == COMMA)
	{
		match(COMMA, ff.firstOfVAList() + stops);
		variableList(stops);
	}
	else
	{
		syntaxCheck(stops - ID);
	}
	
	
}

// statementPart = {statement';'}
void Parser::statementPart(vector<Symbol> stops)
{
	outFile<<"statementPart()"<<endl;
	//lookAheadToken();
	if(in(ff.firstOfStatement()))
	{	
		vector<Symbol>().swap(stopSet);	
		stopSet.push_back(SEMICOLON);
		
		statement(stopSet + stops);
		match(SEMICOLON , ff.firstOfStatement() + stops);
		statementPart(stops);
	}
	else if(in(ff.followOfStatePart()))
	{
		//do nothing
	}
	else
		syntaxCheck(stops);
	
}

// statement = emptyStatement | readStatement | writeStatement |  assignmentStatement | ifStatement | doStatement 
void Parser::statement(vector<Symbol> stops)
{
	outFile<<"statement()"<<endl;
	//lookAheadToken();	
	if(in(ff.firstOfEmptySt()))
	{
		match(SKIP,stops);
	}
	else if(in(ff.firstOfReadSt()))
	{
		readStatement(stops);if(lookAheadTok.getSymbol() == INT)
	{
		match(INT,stops);		
	}	
	else if(lookAheadTok.getSymbol() == BOOL)
	{
		match(BOOL,stops);
	}
	}
	else if(in(ff.firstOfWriteSt()))
	{
		writeStatement(stops);
	}
	else if(in(ff.firstOfProcSt()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(ID);
		match(CALL, stopSet + stops);
		

		if(lookAheadTok.getSymbol() == ID && lookAheadTok.getIDtype() == 3)	
		{
			match(ID, stops);	
		}
		else
		{
			match(ID, stops);
			//outFile<<"ID: "<<lookAheadTok.getLexeme()<< " Type" << lookAheadTok.getIDtype() <<endl;
			cerr<<"Syntax Error: "<<"expecting name of constant type at line "<< lineNo <<endl;
		}
				
	}
	else if(in(ff.firstOfAssignSt()))
	{
		assignmentStatement(stops);
	}
	else if(in(ff.firstOfIfSt()))
	{
		ifStatement(stops);		
	}
	else if(in(ff.firstOfDoSt()))
	{
		doStatement(stops);		
	}
	
}

// readStatement = 'read' variableAccessList
void Parser::readStatement(vector<Symbol> stops)
{
	outFile<<"readStatement()"<<endl;
	match(READ, ff.firstOfVAList() + stops);
	variableAccessList(stops);
			
}

// variableAccessList = variableAccess{','variableAccess}
void Parser::variableAccessList(vector<Symbol> stops)
{
	outFile<<"variableAccessList()"<<endl;

	vector<Symbol>().swap(stopSet);
	stopSet.push_back(COMMA);
	stopSet.push_back(ID);
	variableAccess(stopSet + stops);

	if(lookAheadTok.getSymbol() == COMMA)
	{
		match(COMMA, ff.firstOfVAList() + stops);
		variableList(stops);
	}
	else
		syntaxCheck(stops - ID);		
}

// writeStatement = 'write' expressionList  
void Parser::writeStatement(vector<Symbol> stops)
{
	outFile<<"writeStatement()"<<endl;
	match(WRITE, ff.firstOfExpList() + stops);
	expressionList(stops);
}

// expressionList = expression {','expression}
void Parser::expressionList(vector<Symbol> stops)
{
	outFile<<"expressionList()"<<endl;
	
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(COMMA);

	expression(stopSet + stops);
	
	if(lookAheadTok.getSymbol() == COMMA)
	{
		match(COMMA, ff.firstOfExpList() + stops);
		expressionList(stops);
	}
	else
		syntaxCheck(stops - ID);
		
}

// assignmentStatement = variableAccessList ':=' expressionList
void Parser::assignmentStatement(vector<Symbol> stops)
{
	outFile<<"assignmentStatement()"<<endl;

	vector<Symbol>().swap(stopSet);
	stopSet.push_back(ASSIGN);
	//agc = true;
	variableAccessList(stopSet + stops);
	match(ASSIGN, ff.firstOfExpList() + stops);
	//agc = false;	
	expressionList(stops);
}

// ifStatement = 'if' guardedCommandList 'fi' 
void Parser::ifStatement(vector<Symbol> stops)
{
	outFile<<"ifStatement()"<<endl;

	vector<Symbol>().swap(stopSet);
	stopSet.push_back(FI);
	
	match(IF, ff.firstOfGCList() + stops);
		
	guardedCommandList(stopSet + stops);
	match(FI,stops);	
}

// doStatement = 'do' guardedCommandList 'od'
void Parser::doStatement(vector<Symbol> stops)
{
	outFile<<"doStatement()"<<endl;
	match(DO, ff.firstOfGCList() + stops);

	vector<Symbol>().swap(stopSet);
	stopSet.push_back(OD);
	
	guardedCommandList(stopSet + stops);
	match(OD,stops);
}

// guardedCommandList = guardedCommand{'[]'guardedCommand}
void Parser::guardedCommandList(vector<Symbol> stops)
{
	outFile<<"guardedCommandList()"<<endl;

	vector<Symbol>().swap(stopSet);
	stopSet.push_back(GC1);
	stopSet.push_back(FI);
	stopSet.push_back(OD);
	
	guardedCommand(stopSet + stops);
	//lookAheadToken();
	if(lookAheadTok.getSymbol() == GC1)
	{
		match(GC1, ff.firstOfGCList() + stops);
		guardedCommandList(stops);
	}
	else
		syntaxCheck(stops - ID);
	
}

// guardedCommand = expression '->' statementPart
void Parser::guardedCommand(vector<Symbol> stops)
{
	outFile<<"guardedCommand()"<<endl;
	
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(GC2);	
	stopSet.push_back(RIGHTBRACKET);
	stopSet.push_back(RIGHTP);
	//agc = true;
	expression(stopSet + ff.firstOfStatement() + stops);
	match(GC2, ff.firstOfStatement() + stops);
	//agc = false;
	//lookAheadToken();
	statementPart(stops);
}

// expression = primaryExpression{primaryOperator primaryExpression}	
void Parser::expression(vector<Symbol> stops)
{
	outFile<<"expression()"<<endl;	
	
	primaryExpression(ff.firstOfPrimOp() + stops);
	
	//lookAheadToken();
	if(in(ff.firstOfPrimOp()))
	{
		primaryOperator(ff.firstOfExpList() + stops);
		expression(stops);
	}
	else
		syntaxCheck(stops - ID);
	/*else if(in(ff.firstOfExpList()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(GC2);	
		stopSet.push_back(RIGHTBRACKET);
		stopSet.push_back(RIGHTP);	
		syntaxCheck(ff.firstOfPrimOp()+stopSet);
	}*/
	//syntaxCheck(stops);
}

// primaryOperator = '&' | '|'
void Parser::primaryOperator(vector<Symbol> stops)
{
	outFile<<"primaryOperator()"<<endl;
		
	if(lookAheadTok.getSymbol() == AND)
	{
		match(AND,stops);		
	}	
	else if(lookAheadTok.getSymbol() == OR)
	{
		match(OR,stops);
	}
	
}

// primaryExpression = simpleExpression [relationalOperator simpleExpression]
void Parser::primaryExpression(vector<Symbol> stops)
{
	outFile<<"primaryExpression()"<<endl;	
	simpleExpression(ff.firstOfRelOp() + stops);
	
	//lookAheadToken();
	if(in(ff.firstOfRelOp()))
	{
		relationalOperator(stops);
		simpleExpression(stops);
	}
	else
		syntaxCheck(stops);
	/*else if (in(ff.firstOfExpList()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(GC2);	
		stopSet.push_back(RIGHTBRACKET);
		stopSet.push_back(RIGHTP);	
		syntaxCheck(ff.firstOfPrimOp()+stopSet);		
	}
	*/
}	

// relationalOperator = '<' | '=' | '>'	
void Parser::relationalOperator(vector<Symbol> stops)
{
	outFile<<"relationalOperator()"<<endl;

	if(lookAheadTok.getSymbol() == LESST)
	{
		match(LESST,stops);		
	}	
	else if(lookAheadTok.getSymbol() == EQUAL)
	{
		match(EQUAL,stops);
	}
	else if(lookAheadTok.getSymbol() == GREATERT)
	{
		match(GREATERT,stops);
	}
	else if(lookAheadTok.getSymbol() == LTE)
	{
		match(LTE,stops);
	}
	else if(lookAheadTok.getSymbol() == GTE)
	{
		match(GTE,stops);
	}

}

// simpleExpression = ['-'] term {addingOperator term}
void Parser::simpleExpression(vector<Symbol> stops)
{
	outFile<<"simpleExpression()"<<endl;
	//lookAheadToken();
	if(!in(ff.firstOfTerm()))
	{
		match(MINUS, ff.firstOfTerm() + stops);	
	}
	term(ff.firstOfAddOp() + stops);
	//lookAheadToken();	
	if(in(ff.firstOfAddOp()))
	{
		addopTerm(stops);
	}
	else
		syntaxCheck(stops - ID);
	
}

// addingOperator = '+' | '-'
void Parser::addingOperator(vector<Symbol> stops)
{
	outFile<<"addingOperator()"<<endl;

	if(lookAheadTok.getSymbol() == PLUS)
	{
		match(PLUS,stops);		
	}	
	else if(lookAheadTok.getSymbol() == MINUS)
	{
		match(MINUS,stops);
	}
	
}

//helps to execute {addingOperator term}
void Parser::addopTerm(vector<Symbol> stops)
{
	//lookAheadToken();
	if(in(ff.firstOfAddOp()))
	{
		addingOperator(stops);
		term(stops);
		addopTerm(stops);
	}
	
}

// term = factor {multiplyingOperator factor}
void Parser::term(vector<Symbol> stops)
{
	outFile<<"term()"<<endl;
	factor(ff.firstOfMultOp() + stops);
	//lookAheadToken();
	if(in(ff.firstOfMultOp()))
	{
		multiplyingOperator(stops);
		term(stops);
	}
	else
		syntaxCheck(stops - ID);
}

// multiplyingOperator = '*' | '/' | '\'
void Parser::multiplyingOperator(vector<Symbol> stops)
{
	outFile<<"multiplyingOperator()"<<endl;

	if(lookAheadTok.getSymbol() == TIMES)
	{
		match(TIMES,stops);		
	}	
	else if(lookAheadTok.getSymbol() == DIV)
	{
		match(DIV,stops);
	}
	else if(lookAheadTok.getSymbol() == MOD)
	{
		match(MOD,stops);
	}
}

//factor = constant | variableAccess | '('expression')' | '~' factor	
void Parser::factor(vector<Symbol> stops)
{
	outFile<<"factor()"<<endl;
	//lookAheadToken();
	if(lookAheadTok.getSymbol() == ID)
	{
		if(lookAheadTok.getIDtype() == 1)
			variableAccess(stops);
		else if(lookAheadTok.getIDtype() == 2)
			constant(stops);
	}
	else if(in(ff.firstOfConstant()))
	{
		constant(stops);	
	}
	else if(lookAheadTok.getSymbol() == LEFTP)
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(RIGHTP);
	
		match(LEFTP, ff.firstOfExpList() + stopSet + stops);

		expression(stopSet + stops);	
		match(RIGHTP,stops);
	}
	else if(lookAheadTok.getSymbol() == NOT)
	{
		match(NOT, ff.firstOfFactor() + stops);
		factor(stops);	
	}
	
		
}

//variableAccess = variableName [indexSelector]
void Parser::variableAccess(vector<Symbol> stops)
{
	outFile<<"variableAccess()"<<endl;	
	

	if(lookAheadTok.getSymbol() == ID && lookAheadTok.getIDtype() == 1)	
	{
		match(ID, ff.firstOfIndexSel() + stops);	
	}	
	else
	{
		match(ID, ff.firstOfIndexSel() + stops);
		//outFile<<"ID: "<<lookAheadTok.getLexeme()<< " Type" << lookAheadTok.getIDtype() <<endl;
		cerr<<"Syntax Error: "<<"expecting name of constant type at line "<< lineNo <<endl;
	}
		
	//lookAheadToken();
	if(in(ff.firstOfIndexSel()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(RIGHTBRACKET);
	
		match(LEFTBRACKET, stopSet + ff.firstOfExpList() + stops);
				
		expression(stopSet + stops);	
		match(RIGHTBRACKET,stops);
	}
	else
		syntaxCheck(stops - ID);
}

// constanr = numeral | booleanSymbol | constantName
void Parser::constant(vector<Symbol> stops)
{
	outFile<<"constant()"<<endl;

	if(lookAheadTok.getSymbol() == NUMERAL)
	{
		match(NUMERAL,stops);		
	}	
	else if(lookAheadTok.getSymbol() == TRUE)
	{
		match(TRUE,stops);
	}
	else if(lookAheadTok.getSymbol() == FALSE)
	{
		match(FALSE,stops);
	}
	else if(lookAheadTok.getSymbol() == ID)
	{
		if(lookAheadTok.getSymbol() == ID && lookAheadTok.getIDtype() == 2)	
		{
			match(ID, stops);	
		}	
		else
		{
			match(ID, stops);
			//outFile<<"ID: "<<lookAheadTok.getLexeme()<< " Type" << lookAheadTok.getIDtype() <<endl;
			cerr<<"Syntax Error: "<<"expecting name of constant type at line "<< lineNo <<endl;
		}
		
	}
	
	
	
}


//this function is used to match terminal symbols of CFG
bool Parser::match(Symbol sym , vector<Symbol> stops)
{
	
	//it means the token is matched returns true
	if(lookAheadTok.getSymbol() == sym)
	{
		//outFile<<"Matched "<<lookAheadTok.getSymbol()<<" Lexeme:"<<lookAheadTok.getLexeme()<<" Line No:"<<lineNo<<endl;

		while(1)
		{
			lookAheadTok = scanner.nextToken();	
							
			//newline detected			
			if(lookAheadTok.getSymbol() == NEWLINE)
			{
				NewLine();
			}
			else if(lookAheadTok.getSymbol() != NONAME)
			{
				//cerr<<"Stopped At" << lookAheadTok.getSymbol()<<" Lexeme "<<lookAheadTok.getLexeme()<<endl;			
				break;
			}	
			
		}	
		return true;
	}
	else
	{
		cerr<<"Didn't match "<<sym<<" Line No:"<<lineNo<<endl;
		syntaxError(stops);
	}

	syntaxCheck(stops);
	
}

void Parser::lookAheadToken()
{
	//This function grabs the first valid token.
	//called from constructor 
				
	while(1)
	{	
		lookAheadTok = scanner.nextToken();
		
		//newline detected
		if(lookAheadTok.getSymbol() == NEWLINE)
		{
			NewLine();
		}
		else if(lookAheadTok.getSymbol() != NONAME)
		{
			break;
		}	
	}
		
	
	
}

//new line counter
void Parser::NewLine()
{
	lineNo++;
	//outFile<<"Line No: "<<lineNo<<endl;
}

//Syntex Error recovery
// when any error occured corresponding error msg is displayed and 
// we try find the next possible symbol (stop symbol) that can occur
// after the error. For different kinds of terminal symbol stop 
// symbols are different. The different sets of stop symbols finding


void Parser::syntaxError(vector<Symbol> stops)
{
	//admin.parseError()
	while(!in(stops))
	{
		lookAheadTok = scanner.nextToken();	
		if(lookAheadTok.getSymbol() == NEWLINE)
		{
			NewLine();
		}
	}
}

void Parser::syntaxCheck(vector<Symbol> stops)
{
	//admin.parseError()

	while(1)
	{	
		
		//newline detected
		if(lookAheadTok.getSymbol() == NEWLINE)
		{
			NewLine();
		}
		else if(lookAheadTok.getSymbol() != NONAME)
		{
			break;
		}	
		lookAheadTok = scanner.nextToken();
	}
	if(!in(stops))
	{
		cerr<<"Problem paise: "<<lookAheadTok.getSymbol()<<" At :"<<lineNo<<endl;
		/*if(agc)		
		{
			
			vector<Symbol>().swap(stopSet);
			stopSet.push_back(ID);			
			syntaxError(stopSet + stops);
		}*/
		//else
			syntaxError(stops);
		
	}
	
	
}

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
		
		if(lookAheadTok.getSymbol() == set.at(i))
		{
			return true;
		}
	}
	
	return false;
}


	


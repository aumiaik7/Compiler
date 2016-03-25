#include "parser.h"

//constructor
Parser::Parser(ifstream &in, ofstream &out, Scanner &sc, BlockTable &bt)
:srcFile(in),outFile(out),scanner(sc),bTable(bt),islookAheadTok(false)
{
	agc = false;
	//First look ahead token	
	lookAheadToken();
}

/*
  Imoplementation of the CFG is self explanatory
  for non terminal ther are corresponding methods
  and terminal symbols are matched using match(Symbol,vector<Symbol>)
  method				
*/

// program = block '.'
void Parser::program(Symbol sym)
{
	//Building stop set using vector		
	stopSet.push_back(sym);
	stopSet.push_back(DOT);
	
	outFile<<"program()"<<endl;
	//non terminal bloc	
	block(stopSet);	
	
	//Building stop set using vector that may appear after dot symbol
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(sym); //adding symbols in stop set
	
	//this portion matches dot and if not matched the shows corresponding error message and finds the next stop symbol
	match(DOT,stopSet);
	
	//parsing done
	admin.done();
		
}

// block = 'begin' definitionPart statementPart 'end'
void Parser::block(vector<Symbol> stops)
{
	outFile<<"block()"<<endl;
	
	//new block in block table
	bTable.newBlock();
	//this portion matches begin and if not matched the shows corresponding error message and finds the next stop symbol
	match(BEGIN,ff.firstOfDefinition() + ff.firstOfStatement() + stops);
	
	//look ahead token is first of definition
	if(in(ff.firstOfDefinition()))
	{	
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(END);
		
		definitionPart(ff.firstOfStatement() + stops + stopSet);
		statementPart(stops + stopSet);
		
	}
	//look ahead token is follow of definition
	else if(in(ff.followOfDefPart()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(END);
		statementPart(stops + stopSet);
	}
	
	//this portion matches end and if not matched the shows corresponding error message and finds the next stop symbol
	match(END,stops);
}

// definitionPart = {definition';'}
void Parser::definitionPart(vector<Symbol> stops)
{
	
	outFile<<"definitionPart()"<<endl;
	
	if(in(ff.firstOfDefinition()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(SEMICOLON);	
		definition(ff.firstOfDefinition() + stopSet + stops);
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
	
	match(CONST , ff.firstOfConstant() + stopSet + stops);
	
	int id = matchName(ID, stopSet + stops);
	
	match(EQUAL , ff.firstOfConstant() + stops);
	
	constant(stops);
	
	if(id != -1)
	{
		bool isDefined = bTable.define(id,CONSTANT,nameType,1,nameValue);
		if(!isDefined)
		{
			//ambiguous name error
			admin.error(ScopeE,lookAheadTok.getSymbol(),5);

		}
	}

}

// variableDefinition = typeSymbol variableList | typeSymbol 'array' variableList'['constant']'
void Parser::variableDefinition(vector<Symbol> stops)
{
	outFile<<"variableDefinition()"<<endl;	
	
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(ARRAY);
	
	typeSymbol(stopSet + ff.firstOfVariList() + stops);
	
	if(in(ff.firstOfVariList()))
	{
		variableList(stops);
	}
	else if(lookAheadTok.getSymbol() == ARRAY)
	{
		int defPosition = bTable.def;
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

		if(nameType == INTEGRAL)
		{
			bTable.setArraySize(defPosition,nameValue);
		}
		else
		{
			//
			admin.error(ScopeE,lookAheadTok.getSymbol(),2);
		}
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

	stopSet.pop_back();//pop back ID from stop set that may appear ID
	
	int id = matchName(ID, stopSet + stops);
	
	block(stops);

	if(id != -1)
	{
		bool isDefined = bTable.define(id,PROCEDURE,INTEGRAL,1,1);
		if(!isDefined)
		{
			//ambiguous name error
			admin.error(ScopeE,lookAheadTok.getSymbol(),5);
		}
	}
}

// typeSymbol = 'integer' | 'Boolean'
void Parser::typeSymbol(vector<Symbol> stops)
{
	outFile<<"typeSymbol()"<<endl;

	if(lookAheadTok.getSymbol() == INT)
	{
		nameType = INTEGRAL;
		match(INT,stops);		
	}	
	else if(lookAheadTok.getSymbol() == BOOL)
	{
		nameType = BOOLEAN;
		match(BOOL,stops);
	}
		
}

// variableList = variableName {','variableName}
void Parser::variableList(vector<Symbol> stops)
{
	outFile<<"variableList()"<<endl;
	
	if(lookAheadTok.getSymbol() == ID )	
	{
		//if(lookAheadTok.getIDtype() != 1)
		//	admin.error(ParseE,lookAheadTok.getSymbol(),1);
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(COMMA);

		int id = matchName(ID, stopSet + stops);

		if(id != -1)
		{
				bool isDefined = bTable.define(id,VAR,nameType,1,-1);
				if(!isDefined)
				{
					//ambiguous name error
					admin.error(ScopeE,lookAheadTok.getSymbol(),5);
				}
		}
	}	
	
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

		if(lookAheadTok.getSymbol() == ID )	
		{
			/*if(lookAheadTok.getIDtype() != 3)
				admin.error(ParseE,lookAheadTok.getSymbol(),3);	
			match(ID, stops);
			*/
			//look for the procedure name in block table
			int id = matchName(ID, stops);
			TableEntry entry =  bTable.find(id,bTable.error);
			//not found
			if(bTable.error)
			{
				admin.error(ScopeE,lookAheadTok.getSymbol(),3);
				/**
				 *
				 *
				 *
				 */
			}
			//found

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
		variableAccessList(stops);
	}
	else if(in(ff.followOfVaList()))
	{
		syntaxCheck(stops);		
	}
	else
	{
		syntaxCheck(stops - ID);
	}			
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
	
	variableAccessList(stopSet + stops);
	match(ASSIGN, ff.firstOfExpList() + stops);
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
	
	if(lookAheadTok.getSymbol() == GC1)
	{
		match(GC1, ff.firstOfGCList() + stops);
		guardedCommandList(stops);
	}
	else if(in(ff.firstOfStatement()))
	{
		syntaxCheck(stops);	
	}
	else
	{
		syntaxCheck(stops - ID);
	}
	
}

// guardedCommand = expression '->' statementPart
void Parser::guardedCommand(vector<Symbol> stops)
{
	outFile<<"guardedCommand()"<<endl;
	
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(GC2);	
	stopSet.push_back(RIGHTBRACKET);
	stopSet.push_back(RIGHTP);
	//This flag is used to resolve some issues that arises conflicts between stop sets 
	agc = true;
	expression(stopSet + ff.firstOfStatement() + stops);
	agc = false;
	
	match(GC2, ff.firstOfStatement() + stops);
	statementPart(stops);
}

// expression = primaryExpression{primaryOperator primaryExpression}	
void Parser::expression(vector<Symbol> stops)
{
	outFile<<"expression()"<<endl;	
	
	primaryExpression(ff.firstOfPrimOp() + stops);
	
	if(in(ff.firstOfPrimOp()))
	{
		primaryOperator(ff.firstOfExpList() + stops);
		expression(stops);
	}
	else if(agc)
	{
		syntaxCheck(stops);
	}
	else
		syntaxCheck(stops - ID);
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
	
	
	if(in(ff.firstOfRelOp()))
	{
		relationalOperator(stops);
		simpleExpression(stops);
	}
	else
		syntaxCheck(stops - ID);
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
	
	if(lookAheadTok.getSymbol() == MINUS)
	{
		match(MINUS, ff.firstOfTerm() + stops);	
	}
	term(ff.firstOfAddOp() + stops);
		
	if(in(ff.firstOfAddOp()))
	{
		addopTerm(stops);
	}
	else if(agc)
	{
		syntaxCheck(stops);
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
	
	if(in(ff.firstOfMultOp()))
	{
		multiplyingOperator(stops);
		term(stops);
	}	
	else if(in(ff.followOfExpression()))
	{
		syntaxCheck(stops);
	}
	else if(agc)
	{
		syntaxCheck(stops);
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
	
	if(lookAheadTok.getSymbol() == ID)
	{
		/*if(lookAheadTok.getIDtype() == 1)
			variableAccess(stops);
		else if(lookAheadTok.getIDtype() == 2)
			constant(stops);*/
		TableEntry entry =  bTable.find(lookAheadTok.getValue(),bTable.error);
		//not found
		if(bTable.error)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),4);
		}
		else if(entry.kind == VAR)
			variableAccess(stops);
		else if (entry.kind == CONSTANT)
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
	else if(in(ff.followOfExpression()))
	{
		admin.error(ParseE,lookAheadTok.getSymbol(),5);
	}
	
}

//variableAccess = variableName [indexSelector]
void Parser::variableAccess(vector<Symbol> stops)
{
	outFile<<"variableAccess()"<<endl;	
	

	//look for the variable Name in block table
	int id = matchName(ID, stops);
	if(id != -1)
	{
		TableEntry entry =  bTable.find(id,bTable.error);
		//not found
		if(bTable.error)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),4);
		}
		else if(entry.kind != VAR)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),1);
		}
		//found
		else
		{
			nameValue = entry.value;
			//nameType = INTEGRAL;
		}
	}
	//if(lookAheadTok.getIDtype() != 1)
	//		admin.error(ParseE,lookAheadTok.getSymbol(),1);
	//	match(ID, ff.firstOfIndexSel() + stops);
	
	if(in(ff.firstOfIndexSel()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(RIGHTBRACKET);
	
		match(LEFTBRACKET, stopSet + ff.firstOfExpList() + stops);
		expression(stopSet + stops);	
		match(RIGHTBRACKET,stops);
	}
	else if(agc)
	{
		syntaxCheck(stops);
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
		nameValue = lookAheadTok.getValue();
		nameType = INTEGRAL;
		match(NUMERAL,stops);

	}	
	else if(lookAheadTok.getSymbol() == TRUE)
	{
		nameValue = 0;
		nameType = BOOLEAN;
		match(TRUE,stops);
	}
	else if(lookAheadTok.getSymbol() == FALSE)
	{
		nameValue = 1;
		nameType = BOOLEAN;
		match(FALSE,stops);
	}
	else if(lookAheadTok.getSymbol() == ID)
	{
		//find value from block table
		//later

			//if(lookAheadTok.getIDtype() != 2)
			//look for the constantName in block table
			int id = matchName(ID, stops);
			if(id != -1)
			{
				TableEntry entry =  bTable.find(id,bTable.error);
				//not found
				if(bTable.error)
				{
					admin.error(ScopeE,lookAheadTok.getSymbol(),4);
				}
				else if(entry.kind != CONSTANT)
				{
					admin.error(ScopeE,lookAheadTok.getSymbol(),2);
				}
				//found
				else
				{
					nameValue = entry.value;
					nameType = INTEGRAL;
				}
			}

	}
	else
	{
		nameValue = -3;
		nameType = UNIVERSAL;
		admin.error(ParseE,lookAheadTok.getSymbol(),6);
	}	
}


//this function is used to match terminal symbols of CFG
bool Parser::match(Symbol sym , vector<Symbol> stops)
{
	
	//it means the token is matched returns true
	if(lookAheadTok.getSymbol() == sym)
	{
		while(1)
		{
			lookAheadTok = scanner.nextToken();	
							
			//newline detected			
			if(lookAheadTok.getSymbol() == NEWLINE)
			{
				admin.NewLine();
			}
			//lexical errors
			else if(lookAheadTok.getSymbol() == BADNAME || lookAheadTok.getSymbol() == BADNUMERAL 
				|| lookAheadTok.getSymbol() == BADSYMBOL)
			{
				admin.error(ScanE,lookAheadTok.getSymbol(),0);
			}
			else if(lookAheadTok.getSymbol() != NONAME)
			{
				break;
			}	
		}	
		return true;
	}
	else
	{
		//report syntax error 
		admin.error(ParseE,sym,4);
		//recover from syntax error
		syntaxError(stops);
	}
	//check whether the look ahead symbol is valid or not
	syntaxCheck(stops);
	
}

//this function is used to match name symbols of CFG
int Parser::matchName(Symbol sym , vector<Symbol> stops)
{
	int temp = -1;
	nameTok = lookAheadTok;
	//it means the token is matched returns true
	if(lookAheadTok.getSymbol() == sym)
	{
		//index position of name symbol in symbol table
		temp = lookAheadTok.getValue();
		while(1)
		{
			lookAheadTok = scanner.nextToken();

			//newline detected
			if(lookAheadTok.getSymbol() == NEWLINE)
			{
				admin.NewLine();
			}
			//lexical errors
			else if(lookAheadTok.getSymbol() == BADNAME || lookAheadTok.getSymbol() == BADNUMERAL
				|| lookAheadTok.getSymbol() == BADSYMBOL)
			{
				admin.error(ScanE,lookAheadTok.getSymbol(),0);
			}
			else if(lookAheadTok.getSymbol() != NONAME)
			{
				break;
			}
		}
		return temp;
	}
	else
	{
		//report syntax error
		admin.error(ParseE,sym,4);
		//recover from syntax error
		syntaxError(stops);
	}
	//check whether the look ahead symbol is valid or not
	syntaxCheck(stops);
	return temp;

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
			admin.NewLine();
		}
		//lexical errors
		else if(lookAheadTok.getSymbol() == BADNAME || lookAheadTok.getSymbol() == BADNUMERAL 
				|| lookAheadTok.getSymbol() == BADSYMBOL)
		{
			admin.error(ScanE,lookAheadTok.getSymbol(),0);
		}	
		else if(lookAheadTok.getSymbol() != NONAME)
		{
			break;
		}	
	}
}


//Syntex Error recovery
// when any error occured we try find the next possible valid
// symbol (stop symbol) that can occur after the error. 

void Parser::syntaxError(vector<Symbol> stops)
{
	//continue to grab token until any stop symbol is found
	while(!in(stops))
	{
		lookAheadTok = scanner.nextToken();	
		//newline detected
		if(lookAheadTok.getSymbol() == NEWLINE)
		{
			admin.NewLine();
		}
		//lexical errors
		else if(lookAheadTok.getSymbol() == BADNAME || lookAheadTok.getSymbol() == BADNUMERAL 
				|| lookAheadTok.getSymbol() == BADSYMBOL)
		{
			admin.error(ScanE,lookAheadTok.getSymbol(),0);
		}
	}
}

//after matching any terminal symbol we check whether our look ahead token is valid or not
void Parser::syntaxCheck(vector<Symbol> stops)
{
	//grab a look ahead token
	while(1)
	{	
		
		//newline detected
		if(lookAheadTok.getSymbol() == NEWLINE)
		{
			admin.NewLine();
		}
		//lexical errors
		else if(lookAheadTok.getSymbol() == BADNAME || lookAheadTok.getSymbol() == BADNUMERAL 
				|| lookAheadTok.getSymbol() == BADSYMBOL)
		{
			admin.error(ScanE,lookAheadTok.getSymbol(),0);
		}
		else if(lookAheadTok.getSymbol() != NONAME)
		{
			break;
		}	
		lookAheadTok = scanner.nextToken();
	}
	//look ahead symbol is not in stop set so sytax error occuered
	if(!in(stops))
	{
		//report syntax error
		admin.error(ParseE,lookAheadTok.getSymbol(),5);	
		//recover from syntax error
		syntaxError(stops);
	}
}


//check whether the look ahead symbol is in the 'set' of symbol
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


	


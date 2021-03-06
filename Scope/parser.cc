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
	
	//matchName is match function for matching name symbols
	int id = matchName(ID, stopSet + stops);
	
	match(EQUAL , ff.firstOfConstant() + stops);
	
	//scope and type check
	int tempValue;//for constant value
	PL_Type tempType;//for type of constant
	constant(tempValue, tempType, stops);
	

	if(id != -1)
	{
		//define the name in block table
		bool isDefined = bTable.define(id,CONSTANT,tempType,1,tempValue);
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
	
	PL_Type tempType = typeSymbol(stopSet + ff.firstOfVariList() + stops);
	
	if(in(ff.firstOfVariList()))
	{
		variableList(tempType, stops);
	}
	else if(lookAheadTok.getSymbol() == ARRAY)
	{
		int defPosition = bTable.def;
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(LEFTBRACKET);
		stopSet.push_back(RIGHTBRACKET);

		match(ARRAY, stopSet + ff.firstOfVariList() + stops);
		
		
		variableList(tempType, stopSet + stops);
		
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(RIGHTBRACKET);
		match(LEFTBRACKET , stopSet + ff.firstOfConstant() + stops);
		
		int tempValue;//for constant value
		PL_Type tempType;//for type of constant
		constant(tempValue, tempType, stopSet + stops);
		match(RIGHTBRACKET , stops);

		//set array size
		if(tempType == INTEGRAL)
		{
			//set array size of the declared variable in block table
			bTable.setArraySize(defPosition,tempValue);
		}
		else
		{
			//array size type must be integral
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
	
		if(id != -1)
	{
		bool isDefined = bTable.define(id,PROCEDURE,INTEGRAL,1,1);
		if(!isDefined)
		{
			//ambiguous name error
			admin.error(ScopeE,lookAheadTok.getSymbol(),5);
		}
	}
	block(stops);


}

// typeSymbol = 'integer' | 'Boolean'
PL_Type Parser::typeSymbol(vector<Symbol> stops)
{
	outFile<<"typeSymbol()"<<endl;

	if(lookAheadTok.getSymbol() == INT)
	{
		//nameType = INTEGRAL;
		match(INT,stops);
		return INTEGRAL;
	}	
	else if(lookAheadTok.getSymbol() == BOOL)
	{
		//nameType = BOOLEAN;
		match(BOOL,stops);
		return BOOLEAN;
	}
		
}

// variableList = variableName {','variableName}
void Parser::variableList(PL_Type tempType,vector<Symbol> stops)
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
				bool isDefined = bTable.define(id, VAR, tempType, 1, -1);
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
		variableList(tempType, stops);
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
			//look for the procedure name in block table
			int id = matchName(ID, stops);
			TableEntry entry =  bTable.find(id,bTable.error);
			//not found
			if(bTable.error)
			{
				admin.error(ScopeE,lookAheadTok.getSymbol(),4);
			}
			//found but kind is not procedure
			else if(entry.kind != PROCEDURE)
			{
				admin.error(ScopeE,lookAheadTok.getSymbol(),3);
			}
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
	vector<PL_Type> typeList;
	vector<PL_Type>().swap(typeList);
	variableAccessList(stops,typeList);
}

// variableAccessList = variableAccess{','variableAccess}
void Parser::variableAccessList(vector<Symbol> stops, vector<PL_Type>& varTypeList)
{
	outFile<<"variableAccessList()"<<endl;

	vector<Symbol>().swap(stopSet);
	stopSet.push_back(COMMA);
	stopSet.push_back(ID);
	
	PL_Type tempType = variableAccess(stopSet + stops);
	varTypeList.push_back(tempType);

	if(lookAheadTok.getSymbol() == COMMA)
	{
		match(COMMA, ff.firstOfVAList() + stops);
		variableAccessList(stops, varTypeList);
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
	vector<PL_Type> typeList;
	vector<PL_Type>().swap(typeList);
	expressionList(stops,typeList);
}

// expressionList = expression {','expression}
void  Parser::expressionList(vector<Symbol> stops, vector<PL_Type>& typeList)
{
	outFile<<"expressionList()"<<endl;
	
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(COMMA);


	PL_Type tempType = expression(stopSet + stops,0);
	typeList.push_back(tempType);
	
	if(lookAheadTok.getSymbol() == COMMA)
	{
		match(COMMA, ff.firstOfExpList() + stops);
		expressionList(stops,typeList);
	}
	else
	{
		syntaxCheck(stops - ID);
	}
		
}

// assignmentStatement = variableAccessList ':=' expressionList
void Parser::assignmentStatement(vector<Symbol> stops)
{
	outFile<<"assignmentStatement()"<<endl;

	vector<Symbol>().swap(stopSet);
	stopSet.push_back(ASSIGN);
	
	vector<PL_Type> varTypeList;
	vector<PL_Type>().swap(varTypeList);
	variableAccessList(stopSet + stops,varTypeList);
	match(ASSIGN, ff.firstOfExpList() + stops);

	vector<PL_Type> typeList;
	vector<PL_Type>().swap(typeList);
	expressionList(stops,typeList);

	//size of variable access list and expression list must be same
	if(varTypeList.size() != typeList.size())
	{
		admin.error(ScopeE,lookAheadTok.getSymbol(),8);
	}
	else
	{
		for(int i = 0; i < varTypeList.size(); i++)
			if(varTypeList.at(i) !=  typeList.at(i))
				admin.error(ScopeE,lookAheadTok.getSymbol(),9);
	}
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
	PL_Type tempType = expression(stopSet + ff.firstOfStatement() + stops,0);
	agc = false;
	
	match(GC2, ff.firstOfStatement() + stops);
	statementPart(stops);
}

// expression = primaryExpression{primaryOperator primaryExpression}	
PL_Type Parser::expression(vector<Symbol> stops,int flag)
{
	outFile<<"expression()"<<endl;	
	
	PL_Type tempType = primaryExpression(ff.firstOfPrimOp() + stops);
	//flag = 1 means this functions is called recursively and the type is boolean
	//as primary operator is present
	if(flag == 1)
	{
		if(tempType != BOOLEAN)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),7);
		}
	}
	if(in(ff.firstOfPrimOp()))
	{
		//as primary operator is present then primary expression is expected to be boolean
		if(tempType != BOOLEAN)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),7);
		}

		primaryOperator(ff.firstOfExpList() + stops);
		expression(stops,1);
		return tempType;
	}
	else if(agc)
	{
		syntaxCheck(stops);
		return tempType;
	}
	else
	{
		syntaxCheck(stops - ID);
		return tempType;
	}
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
PL_Type Parser::primaryExpression(vector<Symbol> stops)
{
	outFile<<"primaryExpression()"<<endl;	
	PL_Type tempType = simpleExpression(ff.firstOfRelOp() + stops);
	
	
	if(in(ff.firstOfRelOp()))
	{
		//relational operator is present so simple expression is expected
		//to be integral
		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
		relationalOperator(stops);
		tempType = simpleExpression(stops);

		//relational operator is present so simple expression is expected
		//to be integral
		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
		return BOOLEAN;
	}
	else
	{
		syntaxCheck(stops - ID);
		return tempType;
	}
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
PL_Type Parser::simpleExpression(vector<Symbol> stops)
{
	outFile<<"simpleExpression()"<<endl;
	
	if(lookAheadTok.getSymbol() == MINUS)
	{
		match(MINUS, ff.firstOfTerm() + stops);	
	}

	PL_Type tempType = term(ff.firstOfAddOp() + stops,0);
		
	if(in(ff.firstOfAddOp()))
	{
		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
		addopTerm(stops);
		return tempType;
	}
	else if(agc)
	{
		syntaxCheck(stops);
		return tempType;
	}
	else
	{
		syntaxCheck(stops - ID);
		return tempType;
	}
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
		PL_Type tempType = term(stops,0);
		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
		addopTerm(stops);
	}
}

// term = factor {multiplyingOperator factor}
PL_Type Parser::term(vector<Symbol> stops,int flag)
{
	outFile<<"term()"<<endl;
	
	PL_Type tempType = factor(ff.firstOfMultOp() + stops);
	//flag = 1 means Multiplying operator is parsed so the
	//type of factor is expected to be integral
	if(flag == 1)
	{
		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
	}
	if(in(ff.firstOfMultOp()))
	{
		//Multiplying operator is present so the
		//type of factor is expected to be integral
		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
		multiplyingOperator(stops);

		PL_Type tempType = term(stops,1);

	}	
	else if(in(ff.followOfExpression()))
	{
		syntaxCheck(stops);
		return tempType;
	}
	else if(agc)
	{
		syntaxCheck(stops);
		return tempType;
	}
	else
	{
		syntaxCheck(stops - ID);
		return tempType;
	}
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
PL_Type Parser::factor(vector<Symbol> stops)
{
	outFile<<"factor()"<<endl;
	
	if(lookAheadTok.getSymbol() == ID)
	{

		TableEntry entry =  bTable.find(lookAheadTok.getValue(),bTable.error);

		if (entry.kind == CONSTANT)
		{
			int tempValue;
			PL_Type tempType;
			constant(tempValue, tempType, stops);
			return tempType;
		}
		else
		{
			PL_Type tempType = variableAccess(stops);
			return tempType;
		}



	}
	else if(in(ff.firstOfConstant()))
	{
		int tempValue;
		PL_Type tempType;
		constant(tempValue, tempType, stops);
		return tempType;
	}
	else if(lookAheadTok.getSymbol() == LEFTP)
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(RIGHTP);
	
		match(LEFTP, ff.firstOfExpList() + stopSet + stops);
		PL_Type tempType = expression(stopSet + stops,0);
		match(RIGHTP,stops);

		return tempType;
	}
	else if(lookAheadTok.getSymbol() == NOT)
	{
		match(NOT, ff.firstOfFactor() + stops);
		factor(stops);	
	}
	else if(in(ff.followOfExpression()))
	{
		admin.error(ParseE,lookAheadTok.getSymbol(),5);
		return UNIVERSAL;
	}
	
}

//variableAccess = variableName [indexSelector]
PL_Type Parser::variableAccess(vector<Symbol> stops)
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
		//found but kind is not variable
		else if(entry.kind != VAR)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),1);
		}

		if(in(ff.firstOfIndexSel()))
		{
			if(entry.type != INTEGRAL)
			{
				admin.error(ScopeE,lookAheadTok.getSymbol(),6);
			}
			vector<Symbol>().swap(stopSet);
			stopSet.push_back(RIGHTBRACKET);

			match(LEFTBRACKET, stopSet + ff.firstOfExpList() + stops);
			PL_Type tempType = expression(stopSet + stops,0);
			match(RIGHTBRACKET,stops);

			return tempType;
		}
		else if(agc)
		{
			syntaxCheck(stops);
			return entry.type;
		}
		else
		{
			syntaxCheck(stops - ID);
			return entry.type;
		}
	}
}

// constanr = numeral | booleanSymbol | constantName
void Parser::constant(int& tempValue, PL_Type& tempType, vector<Symbol> stops)
{
	outFile<<"constant()"<<endl;

	if(lookAheadTok.getSymbol() == NUMERAL)
	{
		tempValue = lookAheadTok.getValue();
		tempType = INTEGRAL;
		match(NUMERAL,stops);

	}	
	else if(lookAheadTok.getSymbol() == TRUE)
	{
		tempValue = 0;
		tempType = BOOLEAN;
		match(TRUE,stops);
	}
	else if(lookAheadTok.getSymbol() == FALSE)
	{
		tempValue = 1;
		tempType = BOOLEAN;
		match(FALSE,stops);
	}
	else if(lookAheadTok.getSymbol() == ID)
	{
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
			//fount but kind is not constant
			else if(entry.kind != CONSTANT)
			{
				admin.error(ScopeE,lookAheadTok.getSymbol(),2);
			}
			//found
			else
			{
				tempValue = entry.value;
				tempType = INTEGRAL;
			}
		}

	}
	//constant not found
	else
	{
		tempValue = -1;
		tempType = UNIVERSAL;
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

	//it means the token is matched returns position from symbol table
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
//when any error occured we try find the next possible valid
//symbol (stop symbol) that can occur after the error.

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




	


#include "administration.h"

Administration::Administration(ifstream &in, ofstream &out, Scanner &sc)
:srcFile(in),outFile(out),scanner(sc)
{
	
	//srcFile = in;
	//outFile = out;
	//scanner = sc;
}

int Administration::scan()
{
		
			
	lineNo = 1;
	while(1)
	{
		value = "";
		nextTok = scanner.nextToken();
		if(nextTok.getSymbol() == ENDOFFILE)
			break;
		else if(nextTok.getSymbol() == NEWLINE)
			NewLine();
	}
	cout<<lineNo;
	srcFile.close();

	return 0;
}

void Administration::NewLine()
{
	lineNo++;
	/*if(nextTok.getSymbol() == BADSYMBOL)
	{
		outFile.put("Error");
	}*/
}

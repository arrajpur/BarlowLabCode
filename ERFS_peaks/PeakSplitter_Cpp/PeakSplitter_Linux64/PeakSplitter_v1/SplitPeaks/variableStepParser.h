#pragma once
#include "Parser.h"
#include "wigPosition.h"

class VariableStepParser :
	public Parser
{
private:
	int m_space;
	int m_sigDigit;
	string m_chr;
public:
	VariableStepParser(ifstream* reader, int position, string chr="");

	~VariableStepParser(void);
	wigPosition* nextPosition ();	
	wigPosition* nextChrom(string curChrom);
	int getDigit();
};

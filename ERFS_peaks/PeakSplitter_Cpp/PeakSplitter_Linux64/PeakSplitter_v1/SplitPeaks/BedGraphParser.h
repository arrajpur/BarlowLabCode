#pragma once
#include "Shared.h"
#include "Parser.h"
#include "wigPosition.h"

class BedGraphParser :
	public Parser
{
public:
	BedGraphParser(ifstream* bufReader, int position);
	~BedGraphParser(void);
	wigPosition* nextPosition();
	wigPosition* nextChrom(string curChrom);	
};

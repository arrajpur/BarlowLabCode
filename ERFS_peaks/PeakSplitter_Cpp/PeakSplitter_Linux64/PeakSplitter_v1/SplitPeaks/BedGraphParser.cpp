#include "BedGraphParser.h"



BedGraphParser::~BedGraphParser(void)
{
}

BedGraphParser::BedGraphParser(ifstream* bufReader, int position):Parser(bufReader,"BED")
{
	m_CurPosition = position;
	string line;

	m_ResetPoint = m_CurPosition;	
	getline(*m_bufRead, line, '\n');
	m_CurPosition += line.size() + NEWLINE_LEN;
	vector<string> myelements;
	Parser::Tokenize(line, myelements, " \t");

    if(myelements.size()!= 4) 
	{
		string err = "Invalid BedGraph file format in line starting with \"" + line + "\"\n";
    	throw err;
    }

    //return to be before the first line of signal
    m_bufRead->seekg(m_ResetPoint);
	m_CurPosition = m_ResetPoint;
}

	//create hash tables,  hold base_index as key and tag number as value

wigPosition* BedGraphParser::nextPosition()
{
	string line;

	m_ResetPoint = m_CurPosition;	
	getline(*m_bufRead, line, '\n');
	m_CurPosition += line.size() + NEWLINE_LEN;
	if(line.empty())
		return NULL;
	vector<string> myelements;
	Parser::Tokenize(line, myelements, " \t");
	if(myelements.size() !=4) 
	{
		string err = "Invalid BedGraph file format in line starting with \"" + line + "\"\n";
    	throw err;
    }
	string chr = "";
	int start = 0;
	int end = 0;
	float score = 0;
	try
	{
		chr = myelements[0];
		start = Parser::AtoI(myelements[1]);
		end = Parser::AtoI(myelements[2]);
		score = Parser::AtoF(myelements[3]);
	}
	catch(...)
	{
		string err = "Invalid BedGraph file format in line starting with \"" + line + "\"\n";
    	throw err;
	}
	wigPosition* pos = new wigPosition(chr,start,end,score);
	return pos;
}


wigPosition* BedGraphParser::nextChrom(string currentChrom)
{
	wigPosition* pos = this->nextPosition();
	while(pos != NULL && pos->getChr() == currentChrom)
	{
		delete pos;
		pos = this->nextPosition();
	}
	return pos;
}

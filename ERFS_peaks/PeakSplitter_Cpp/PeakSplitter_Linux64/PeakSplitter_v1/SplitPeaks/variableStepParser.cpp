#include "variableStepParser.h"
#include <cmath>


VariableStepParser::~VariableStepParser(void)
{
}


VariableStepParser::VariableStepParser(ifstream* reader, int position, string chr):Parser(reader,"WIG")
{
	m_CurPosition = position;
	m_space = 1;
	string line;
	
	m_ResetPoint = m_CurPosition;	
	getline(*m_bufRead, line, '\n');
	m_CurPosition += line.size() + NEWLINE_LEN;

	string upperCaseLine = Parser::ToUpper(line);
	if((int)upperCaseLine.find("VARIABLESTEP") >= 0)
	{
		vector<string> myelements;
		Parser::Tokenize(line, myelements, " \t");
		m_space=1;
		for(int i=0;i<myelements.size();i++)
		{
			vector<string> tokens;
			Parser::Tokenize(line, tokens, "=");
			string uppercaseToken = Parser::ToUpper(myelements[i]);
			if((int)uppercaseToken.find("CHROM") >= 0)
			{
				Parser::Tokenize(myelements[i], tokens, "=");
				m_chr = tokens[1];
				if(Parser::ToUpper(m_chr) == "CHRMT" && Parser::ToUpper(chr) == "CHRM")
				{
					m_chr = chr;
				}
			}
			else if((int)uppercaseToken.find("SPAN") >=0)
			{
				Parser::Tokenize(myelements[i], tokens, "=");
				try
				{
					m_space = Parser::AtoI(tokens[1]);
				}
				catch(...)
				{
					string err = "Invalid variableStep wig header line \"" + line + "\"\n";
					throw err;
				}
			}
		}
		m_ResetPoint = m_CurPosition;
	}

	m_ResetPoint = m_CurPosition;
	getline(*m_bufRead, line, '\n');
	m_CurPosition += line.size() + NEWLINE_LEN;

	vector<string> myelements;
	Parser::Tokenize(line, myelements, " \t");
    if(myelements.size() !=2) 
	{
		string err = "Invalid variableStep wig file format in line starting with \"" + line + "\"\n";
    	throw err;
    }

	try
	{
		int signal = Parser::AtoI(myelements[0]);
		m_sigDigit=signal%10;
	}
	catch(...)
	{
		string err = "Invalid variableStep wig file format in line starting with \"" + line + "\"\n";
		throw err;
	}
    //return to be before the first line of signal
	Lineback();

	if(m_chr.empty())
	{
		m_chr = chr;
	}
	if(Parser::ToUpper(chr) == "CHRMT" && Parser::ToUpper(m_chr) == "CHRM")
	{
		m_chr = chr;
	}
}


//create hash tables,  hold base_index as key and tag number as value
wigPosition* VariableStepParser::nextPosition ()
{
	m_ResetPoint = m_CurPosition;
	string line;
	getline(*m_bufRead, line, '\n');
	m_CurPosition += line.size() + NEWLINE_LEN;	
	if(line.empty())
	{
		return NULL;
	}
	wigPosition* pos=NULL;
	vector<string> myelements;
	Parser::Tokenize(line, myelements, " \t");

	if((int)line.find("chrom")>=0)
	{
		//this is a chromosome defenition line
		m_space=1;
		for(int i=0;i<myelements.size();i++)
		{
			string uppercaseToken = Parser::ToUpper(myelements[i]);
			if((int)uppercaseToken.find("CHROM") >= 0)
			{
				vector<string> tokens;
				Parser::Tokenize(myelements[i], tokens, "=");
				if(tokens.size() < 1) 
				{
					string err = "Invalid variableStep wig header line: \"" + line + "\"\n";
    				throw err;
				}
				m_chr = tokens[1];
			}
			else if((int)uppercaseToken.find("SPAN") >= 0)
			{
				vector<string> tokens;
				Parser::Tokenize(myelements[i], tokens, "=");
				if(tokens.size() < 1) 
				{
					string err = "Invalid variableStep wig header line: \"" + line + "\"\n";
    				throw err;
				}
				try
				{
					m_space = Parser::AtoI(tokens[1]);
				}
				catch(...)
				{
					string err = "Invalid variableStep wig header line: \"" + line + "\"\n";
					throw err;
				}
			}
		}
		m_ResetPoint = m_CurPosition;
		getline(*m_bufRead, line, '\n');
		m_CurPosition += line.size() + NEWLINE_LEN;		
		Parser::Tokenize(line, myelements, " \t");
		if(myelements.size() != 2) 
		{
			string err = "Invalid variableStep wig file format in line starting with \"" + line + "\"\n";
			throw err;
		}
	}
	int start = 0;
	int end = 0;
	float score = 0;
	try
	{
		start = Parser::AtoI(myelements[0]);
		end = start + m_space-1;
		score = Parser::AtoF(myelements[1]);
	}
	catch(...)
	{
		string err = "the first field should be an integer in line " + line + "\n";
		throw err;
	}
	
	pos = new wigPosition(m_chr,start,end,score);
	return pos;
}



wigPosition* VariableStepParser::nextChrom(string currentChrom)
{
	wigPosition* pos = this->nextPosition();
	while(pos != NULL && (pos->getChr() == currentChrom))
	{
		delete pos;
		pos = this->nextPosition();
	}
	return pos;
}

int VariableStepParser::getDigit()
{
	return m_sigDigit;
}

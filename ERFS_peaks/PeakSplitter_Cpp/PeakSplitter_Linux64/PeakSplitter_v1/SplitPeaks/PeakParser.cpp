#include "PeakParser.h"
#include "Parser.h";


PeakParser::~PeakParser(void)
{
}


PeakParser::PeakParser(ifstream* file)
{

        /*      Sets up a file reader to read the file passed on the command
                line one character at a time */

        /* Filter FileReader through a Buffered read to read a line at a
           time */

        m_bufRead = file;
        long firstPeak = file->tellg();
        long offset = 0;
        // Read first line
        string line;
		getline(*m_bufRead, line, '\n');
        //check if first line starts with the word track
        if((int)line.find("track") >= 0)
		{
        	firstPeak = m_bufRead->tellg();
			getline(*m_bufRead, line, '\n');
        }
        vector<string> myelements;
		Parser::Tokenize(line, myelements, " \t");

        if(myelements.size() < 3)
		{
			string err = "Invalid Peak file format in line starting with \"" + line + "\"\n";
        	throw err;
        }
        else
		{
        	while(!line.empty())
			{
				vector<string> tokens;
        		Parser::Tokenize(line, tokens, " \t");
				if(tokens.size() < 3) 
				{
					string err = "Invalid Peak file format in line starting with \"" + line + "\"\n";
    				throw err;
				}
				//if not contain key
				string chrom = tokens[0];				
				if(chrom.substr(0,3) == "chr")
					chrom = chrom;
				else if(chrom.substr(0,3) == "CHR")
					chrom = "chr" + chrom.substr(3,chrom.length());
				else{chrom = "chr" + chrom;}

				if (m_chrInFile.find(chrom) == m_chrInFile.end())
				{
					m_chrInFile[chrom] = offset;
				}				
        		
        		offset = m_bufRead->tellg();
        		getline(*m_bufRead, line, '\n');
        	}
        }

        //go to the start of the file (must clear the EOF flag first)
		m_bufRead->clear();
        m_bufRead->seekg(0);
        current = NULL;
}

Position* PeakParser::nextPosition()
{
	string line;
	getline(*m_bufRead, line, '\n');
	if(line.empty())
	{
		return NULL;
	}
	else
	{
		vector<string> myelements;
		Parser::Tokenize(line, myelements, " \t");
		if(myelements.size() < 3) 
		{
			string err = "Invalid Peak file format in line starting with \"" + line + "\"\n";
			throw err;
		}
        
		string chr = "";
		int start= -1;
		int end = -1;
		try
		{
			chr = myelements[0];
			start = Parser::AtoI(myelements[1]);
			end = Parser::AtoI(myelements[2]);
		}
		catch(...)
		{
			string err = "Invalid Peak file format in line starting with \"" + line + "\"\n";
			throw err;
		}
		Position* g = new Position(start,end,chr);
		current = g;
		return g;
		
	}
}

Position* PeakParser::getCur()
{
	return current;
}
void PeakParser::moveToChr(string chr)
{	
	if(m_chrInFile.find(chr) == m_chrInFile.end())
	{
		string err = "No such element\n";
		throw err;
	}
	m_bufRead->seekg(m_chrInFile[chr]);	
}

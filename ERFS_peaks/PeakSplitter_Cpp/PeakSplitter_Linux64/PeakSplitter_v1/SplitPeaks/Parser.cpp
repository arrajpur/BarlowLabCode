#include "Parser.h"

Parser::~Parser(void)
{
}

Parser::Parser(ifstream* reader,string type)
{
	m_CurPosition = 0;
	m_bufRead = reader;
	m_ResetPoint = reader->tellg();
	m_wigType = "";
	m_LineDelimiter = '\n';
    string line;

	getline(*m_bufRead, line, m_LineDelimiter);
	m_CurPosition += line.size() + NEWLINE_LEN;
    while(!line.empty())
	{
    	if(type == "WIG")
    	{
			string uppercaseLine = Parser::ToUpper(line);
    		if((int)uppercaseLine.find("VARIABLESTEP") >= 0)
			{
				m_bufRead->seekg(m_ResetPoint);
				m_CurPosition = m_ResetPoint;
        		break;
        	}
    		else if((int)uppercaseLine.find("FIXEDSTEP") >= 0)
    		{
				string err = "Wig fixedStep format is not supported\n";
    			throw err;
    		}
    		else if((int)line.find("=") >= 0 || (int)line.find("#") >= 0)
    		{
    			m_ResetPoint = m_CurPosition;
    			getline(*m_bufRead, line, m_LineDelimiter);
				m_CurPosition += line.size() + NEWLINE_LEN;
    			continue;
    		}
    		else
    		{
				string err = "The variableStep definition is missing in the wig file\n";
    			throw err;
    		}
    	}
    	else //BED format
    	{
    		 if((int)line.find("=") >= 0 || (int)line.find("#") >= 0)
    		{
    			m_ResetPoint = m_CurPosition;
    			getline(*m_bufRead, line, m_LineDelimiter);
				m_CurPosition += line.size() + NEWLINE_LEN;
    			continue;
    		}
    		else
    		{
    			vector<string> myelements;
				Tokenize(line, myelements, " \t");
    			if(myelements.size() == 4)
    			{
    				try
					{
						AtoI(myelements[1]);
						AtoI(myelements[2]);
            		}
    				catch(exception& e)
					{
						string err = "Invalid BedGraph file format in line starting with \"" + line + "\"\n";
    					throw err;
    				}
    			}
    			else
    			{
					string err = "Invalid BedGraph file format in line starting with \"" + line + "\"\n";
    				throw err;
    			}
    			m_bufRead->seekg(m_ResetPoint);
				m_CurPosition = m_ResetPoint;
    			break;
    		}
    	}
    }
}



void Parser::close ()
{
	m_bufRead->close();
}


void Parser::Lineback()
 {
	 m_bufRead->seekg(m_ResetPoint);	 
	 m_CurPosition = m_ResetPoint;
 }

string Parser::getFileFormat(ifstream* reader, int* position)
 {
	 (*position) = 0;
	 string wigType = "";
	// find the first line of data or variable step chromosome definition
	for(int i=0; i<20; i++)
	{
		string line;
		getline(*reader, line, '\n');
		(*position) += line.length() + NEWLINE_LEN;
		if(!line.empty())
		{
			vector<string> myelements;
			Parser::Tokenize(line, myelements, " \t");

    		for(int i=0;i<myelements.size();i++)
    		{
				if((int)Parser::ToUpper(myelements[i]).find("TYPE") >= 0)
    			{
					wigType = myelements[i].substr(myelements[i].find("=") + 1,myelements[i].length());
    				return wigType;
    			}
    		}
		}
	}
	return wigType;
 }

void Parser::Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters)
{
	tokens.clear();
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}




string Parser::ToUpper(const string& str)
{
	string out;
	for(int i=0;i<str.length();i++)
	{
		out+=toupper(str[i]);
	}
	return out;
}

//this method throws exception in case the string is not a number
int Parser::AtoI(const string& str)
{
	int res = atoi(str.c_str());
	if((res == 0) && (str[0] != '0'))
	{
		string err= "Invalid number " + str;
		throw err;
	}
	return res;
}

float Parser::AtoF(const string& str)
{
	float res = atof(str.c_str());
	if((res == 0) && (str[0] != '0'))
	{
		string err= "Invalid number " + str;
		throw err;
	}
	return res;
}

string Parser::Trim(string& str)
{
	return trim((char*)str.c_str());
}

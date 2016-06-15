#pragma once
#include "Shared.h"
#include "Position.h"

class Parser
{
protected:
	ifstream* m_bufRead;
	string m_wigType;
	int m_ResetPoint;
	int m_CurPosition;
	char m_LineDelimiter;
public:

	Parser(ifstream* reader,string type);
	virtual ~Parser(void);

	virtual Position* nextPosition() = 0;
	virtual Position* nextChrom(string currentChrom) = 0;
	void close ();
	void Lineback();	
	static string getFileFormat(ifstream* reader,int* position);
	static void Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters);
	static string ToUpper(const string& str);
	static int AtoI(const string& str);
	static float AtoF(const string& str);
	static string Trim(string& str);
};

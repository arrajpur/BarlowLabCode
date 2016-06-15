#pragma once
#include "Shared.h"
#include "Position.h"

class PeakParser
{
private:
	ifstream* m_bufRead;
	map<string, long> m_chrInFile;
	Position* current;
public:
	PeakParser(ifstream* file);
	~PeakParser(void);
	Position* nextPosition();
	Position* getCur();
	void moveToChr(string chr);
};

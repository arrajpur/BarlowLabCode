#pragma once
#include "Shared.h"

class Position
{
private:
	int m_Start;
	int m_End;
	string m_Chromosome;
public:	
	Position(){};
	Position(int start, int end, string chrom);
	~Position(void);
	string getChr();
	int getStart();
	int getEnd();
	int contains(Position& p);
};

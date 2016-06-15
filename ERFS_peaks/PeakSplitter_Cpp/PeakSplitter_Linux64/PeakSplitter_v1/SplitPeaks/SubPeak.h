#pragma once
#include "Shared.h"

class SubPeak
{
public:
	string m_chr;
	int m_start;
	int m_end;
	float m_SummitTags;
	int m_summitStart;

int m_summitEnd;
public:
	SubPeak(string chr, int start, int end, float summit, int summitStartPos,int summitEndPosition);
	~SubPeak(void);	
	void Print(ofstream* fw);
	bool operator< (const SubPeak& rhs) const;
};

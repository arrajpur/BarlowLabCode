#include "wigPosition.h"



wigPosition::~wigPosition(void)
{
}

wigPosition::wigPosition(string chrom,int start, int end, float score):Position(start,end,chrom)
{		
	m_score = score;
}
float wigPosition::getScore()
{
	return m_score;
}


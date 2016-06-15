#include "Position.h"



Position::~Position(void)
{
}
Position::Position(int start, int end, string chrom) 
{
	m_Start = start;
	m_End = end;
	if(chrom.substr(0,3) == "chr")
		m_Chromosome = chrom;
	else if(chrom.substr(0,3) == "CHR")
		m_Chromosome = "chr" + chrom.substr(3,chrom.length());
	else{m_Chromosome = "chr" +chrom;}
}
string Position::getChr() 
{
	return m_Chromosome;
}
int Position::getStart() 
{
	return m_Start;
	}
int Position::getEnd()
{
	return m_End;
}
int Position::contains(Position& p) 
{
	if( (this->m_Start >= p.m_Start && this->m_Start <= p.m_End ) || (this->m_End >= p.m_Start && this->m_End <= p.m_End) )
	{
		//overlap
		return 0;
	}
	else if (this->m_End < p.m_Start)
	{
		//before
		return -1;
	}
	else 
	{
		//after
		return 1;
	}	
}
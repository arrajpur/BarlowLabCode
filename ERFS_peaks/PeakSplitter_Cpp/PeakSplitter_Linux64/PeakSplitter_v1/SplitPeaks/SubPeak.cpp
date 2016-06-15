#include "SubPeak.h"

SubPeak::~SubPeak(void)
{
}


SubPeak::SubPeak(string chr, int start, int end, float summit, int summitStartPos,int summitEndPosition)
{
	m_chr = chr;
	m_start = start;
	m_end = end;
	m_SummitTags = summit;
	m_summitStart = summitStartPos;
	m_summitEnd = summitEndPosition;
}

bool SubPeak::operator< (const SubPeak& other) const
 {
	 if(this->m_SummitTags < other.m_SummitTags)
		 return  true;
	 return false;
}


void SubPeak::Print(ofstream* fw)
{
	int summitPosition = (this->m_summitStart + this->m_summitEnd)/2;
	*fw << this->m_chr << "\t" << this->m_start << "\t" << this->m_end << "\t" << this->m_SummitTags << "\t" << summitPosition << "\n";
}
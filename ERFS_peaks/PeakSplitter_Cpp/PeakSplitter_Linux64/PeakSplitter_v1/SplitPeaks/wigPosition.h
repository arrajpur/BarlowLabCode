#pragma once
#include "Position.h"

class wigPosition :
	public Position
{
private:
	float m_score;
public:	
	wigPosition(){};
	wigPosition(string chrom,int start, int end, float score);
	~wigPosition(void);
	float getScore();
};

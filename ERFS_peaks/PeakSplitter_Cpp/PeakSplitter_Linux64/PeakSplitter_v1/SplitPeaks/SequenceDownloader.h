#include "Shared.h"
#include <strstream>
using namespace std;

#pragma once

class SequenceDownloader
{
public:
	SequenceDownloader(void);
	~SequenceDownloader(void);
	strstream* GetSequence(string seq);
};

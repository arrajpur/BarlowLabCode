#pragma once
#include "Shared.h"
#include "SubPeak.h"
#include "Parser.h"
#include "PeakParser.h"
#include "variableStepParser.h"
#include "BedGraphParser.h"
#include "zlib.h"

class Split
{
private:
	float m_bd;
	multiset<SubPeak> m_SortedList;
	string m_OutDir;
	string m_PeakFileName;
	string m_PeakFileNoExtension;
	string m_PeakFileExtension;
	int m_numOfseq;
	int m_cutoff;
	string m_SubpeakFileName;
	ofstream* m_fw;
	void splitPeaks(string peakFile, string wigFile,float bd);
	void splitPeaksWigDirectory(string peakFile, string wigFolder,float bd);
	void splitPeak(Position* peak, Parser* wig,ofstream* fw);
	string Unzip(string inFilePath);
	string getRelatedChrFile(string chrString, string dir);
	list<wigPosition>* getOverlapList(Position* peak, Parser* wig);
	void AddToSortedSet(SubPeak* subPeak);
	Position* nextChrom(Parser* wig, PeakParser* peak, Position* curWig);

public:
	Split(void);
	~Split(void);
	void init(string peakFile, string wigFile,float bd, int cutoff,string outDir,int numSeq);
	string getFatsaName();
	void getPeakFasta(string dasURL, int seqLength);
};

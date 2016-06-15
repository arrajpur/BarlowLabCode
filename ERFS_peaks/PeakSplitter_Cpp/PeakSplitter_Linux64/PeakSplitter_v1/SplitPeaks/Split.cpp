#include "Split.h"
#include "SequenceDownloader.h"
#include "unzip.h"

static bool is_directory (string& strDir);
static bool FileExists(string& strFilename);
static void DeleteFile(string& fileName);
Split::Split(void)
{
}

Split::~Split(void)
{
	if(m_fw != NULL)
	{
		m_fw->close();
		delete m_fw;
	}
}

void Split::init(string peakFile, string wigFile,float bd, int cutoff,string outDir,int numSeq)
{
	m_numOfseq = numSeq;
	m_cutoff=cutoff;

	if(!is_directory(outDir))
	{
		string err = outDir + " is not a directory\n";
		throw err;
	}
	if(!FileExists(peakFile))
	{
		string err = peakFile + " does not exist\n";
		throw err;
	}
    if(!FileExists(wigFile))
	{
		string err = wigFile + " does not exist\n";
		throw err;
	}

	if(outDir[outDir.length() - 1] == DIR_SEP)
	{
		m_OutDir = outDir;
	}
	else
	{
		m_OutDir = outDir + DIR_SEP;
	}

	m_PeakFileName = peakFile.substr((int)peakFile.find_last_of(DIR_SEP) + 1,peakFile.length());

	//keep output file names
    //find extension
	//check if peakFile name contain the full path, if so separate between the path and the name

	int pos = (int)m_PeakFileName.find_first_of('.');
    if(pos > 0)
    {
    	m_PeakFileNoExtension = m_PeakFileName.substr(0, pos);
    	m_PeakFileExtension = m_PeakFileName.substr(pos);
    }
    else
    {
    	m_PeakFileNoExtension = m_PeakFileName;
    	m_PeakFileExtension = "";
    }
    m_SubpeakFileName = m_OutDir + m_PeakFileNoExtension+".subpeaks"+m_PeakFileExtension;
	m_fw = new ofstream(m_SubpeakFileName.c_str());
	*m_fw << "Chromosome\tStart\tEnd\tHeight\tSummitPosition\n";

	if(is_directory(wigFile))
	{
		splitPeaksWigDirectory(peakFile,wigFile,bd);
	}
	else
	{
		splitPeaks(peakFile,wigFile,bd);
	}

}
void Split::splitPeaks(string peakFile, string wigFile,float bd)
{
	m_bd = bd;
	
	bool isZip=false;

	if(!FileExists(peakFile))
	{
		string err = "cannot find file " + peakFile + "\n";
		throw  err;
	}
	ifstream* in = new ifstream(peakFile.c_str(),ios::binary);

	PeakParser* peak = new PeakParser(in);

	if((wigFile.substr(wigFile.length() - 3,wigFile.length() - 1)  == ".gz") ||
		 (wigFile.substr(wigFile.length() - 3,wigFile.length() - 1)  == "zip" ))
	{
		   string unzipFileName = this->Unzip(wigFile);
		   wigFile = unzipFileName;
		   isZip=true;
	}

	if(!FileExists(wigFile))
	{
		string err = "cannot find file " + wigFile +"\n";
		throw  err;
	}
	ifstream* inWig = new ifstream(wigFile.c_str(),ios::binary);
	int position = 0;
	string wigType = Parser::getFileFormat(inWig,&position);
	inWig->seekg(position);
	Parser* wig = NULL;
	if(wigType == "wiggle_0")
	{
		wig = new VariableStepParser(inWig, position);
	}
	else if(wigType == "bedGraph")
	{
		 wig = new BedGraphParser(inWig, position);
	}
	else
	{
		if(wigType == "")
		{
			string err = "Can't guess the wig file's type, track type definition is missing\n";
			throw err;
		}
		else
		{
			string err = "unknown wig file type" + wigType+"\n";
			throw err;
		}
	}

	Position* curwig = wig->nextPosition();
	Position* curPeak = NULL;
	string curChr;
	cout << "Processing..." << endl;
	while(curwig!=NULL)
	{
		if(curwig->getChr() != curChr)
		{
			try
			{
				peak->moveToChr(curwig->getChr());
				cout << "Start processing " << curwig->getChr() << " peaks" << endl;
			}
			catch(...)
			{				
				curwig = nextChrom(wig,peak,curwig);
			}
			if(curwig == NULL)
			{
				break;
			}
			curChr = curwig->getChr();
			if(curPeak != NULL)
			{
				delete curPeak;
			}
			curPeak = peak->nextPosition();
		}
		while(curPeak != NULL && curwig != NULL)
		{
			if(curPeak->getChr() != curChr)
			{
				cout << "End processing " << curChr << " peaks" << endl;				
				curwig = nextChrom(wig, peak, curwig);
				if(curwig == NULL)
				{
					break;
				}
				curChr = curwig->getChr();
				delete curPeak;
				curPeak = peak->nextPosition();
			}
			splitPeak(curPeak,wig,m_fw);
			delete curPeak;
			curPeak = peak->nextPosition();
		}
		if(curPeak == NULL)
		{
			cout << "End processing " << curChr << " peaks" << endl;			
			curwig = nextChrom(wig, peak, curwig);
			if(curwig == NULL)
			{
				break;
			}
			curChr = curwig->getChr();
			delete curPeak;
			curPeak = peak->nextPosition();
		}
	}	
	if(isZip)
	{
		DeleteFile(wigFile);
	}
	delete in;
	delete peak;
	delete wig;
	delete inWig;	
}

void Split::splitPeaksWigDirectory(string peakFile, string wigFolder,float bd)
{
	m_bd = bd;

	bool isZip = false;

	if(!FileExists(wigFolder))
	{
		string err = "wig folder " + wigFolder + " was not found\n";
		throw err;
	}

	ifstream* in = new ifstream(peakFile.c_str(),ios::binary);
	PeakParser* peak = new PeakParser(in);

	string wigFileName = "";
	ifstream* inWig = NULL;
	//loop over all peaks in file
	Position* curPeak = peak->nextPosition();
	string curChr = "";
	string prevChr = "";
	Parser* wig=NULL;

	while(curPeak!=NULL)
	{
		prevChr = curChr;
		curChr = curPeak->getChr();
		//read new wig file
		if(curChr != prevChr)
		{
			if(inWig!=NULL)
			{
				cout <<  "End processing " << prevChr << " peaks" <<endl;
				inWig->close();
				if(isZip)
				{
					DeleteFile(wigFileName);
					isZip=false;
				}
			}
			cout << "Start processing " << curChr << " peaks" << endl;
			wigFileName = getRelatedChrFile(curChr,wigFolder);
			if((wigFileName.substr(wigFileName.length() - 3,3)  == ".gz") ||
				(wigFileName.substr(wigFileName.length() - 4,4)  == ".zip" ))
			{
			   string unzipFileName = this->Unzip(wigFileName);
			   wigFileName = unzipFileName;
			   isZip=true;
			}
			inWig = new ifstream(wigFileName.c_str(),ios::binary);
			int position = 0;
			string wigType = Parser::getFileFormat(inWig,&position);
			inWig->seekg(position);
			if(wigType == "wiggle_0")
			{
				wig = new VariableStepParser(inWig,position,curChr);
			}
			else if(wigType == "bedGraph")
			{
				 wig = new BedGraphParser(inWig,position);
			}
			else
			{
				if(wigType == "")
				{
					string err= "Can't guess the wig file's type, track type definition is missing\n";
					throw err;
				}
				else
				{
					string err = "unknown wig file type" + wigType+"\n";
					throw err;
				}
			}
		}
		splitPeak(curPeak,wig,m_fw);
		curPeak = peak->nextPosition();
	}
	if(!curChr.empty())
		cout <<  "End processing " << curChr << " peaks" <<endl;
	//close the last file
	if(inWig != NULL)
		inWig->close();
	if(isZip){
		DeleteFile(wigFileName);
	}
	if(in != NULL)
		delete in;
	if(peak != NULL)
		delete peak;
	if(inWig != NULL)
		delete inWig;
}

string Split::getRelatedChrFile(string chrString, string dirName)
{
	//TODO iteate through all files in dir and return the first file to contain string chrString
	FILEINFO* file_info;
	NODE* file_list = load_dir_list(dirName.c_str(),"n");
	 for (file_info  = (FILEINFO *) file_list-> next;
         file_info != (FILEINFO *) file_list;
         file_info  = file_info-> next)
      {
			string fileName = file_info->dir.file_name;
			int res = (int)fileName.find(chrString);
			if((int)fileName.find(chrString) >= 0)
			{
				free_dir_list (file_list);
				return dirName + DIR_SEP + fileName;
			}
      }
    free_dir_list (file_list);
	string err = "wig file for choromosome " + chrString + " is missing\nMake sure that the file name contains the string \"" + chrString + "\" before the chromosome number\n";
	throw err;
};

void Split::splitPeak(Position* peak, Parser* wig,ofstream* fw)
{
	wig->Lineback();
	list<wigPosition>* wigPos = getOverlapList(peak,wig);
	//check if wigPos is not empty, in case there are not signal in wig file to specific peak
	if(wigPos->empty())
	{
		cout <<  "no signal in wig file was found for peak "
				<< peak->getChr() << " " << peak->getStart() << " "
				<< peak->getEnd() << endl;
		cout << "continue to next peak" << endl;
		delete wigPos;
		return;
	}

	//convert the list to array
	vector<wigPosition> wigPosArr(wigPos->size());
	int i=0;
	for(list<wigPosition>::iterator itr = wigPos->begin();
        itr != wigPos->end();
        itr++)
	{
		wigPosArr[i] = *itr;
		i++;
	}
	delete wigPos;
	bool wayUp = true;
	bool foundMaxima1 = false;
	bool foundLocalValley = false;
	int localMaxima1=0;
	int localMaxima2=0;
	int localValley=0;
	int subPeakStart=0;

	if(wigPosArr.size() == 1 && wigPosArr[0].getScore() >= m_cutoff)
	{
		//no need to split peak if there only one line in wig file for that peak
		SubPeak* subPeak = new SubPeak(peak->getChr(),
		peak->getStart(),
		peak->getEnd(),
		wigPosArr[0].getScore(),
		peak->getStart(),
		peak->getEnd());
		subPeak->Print(fw);
		AddToSortedSet(subPeak);
		delete subPeak;
		return;
	}

	for(int i=0;i<wigPosArr.size();i++)
	{
		if(i==0)
		{
			//first position
			localMaxima1 = 0;
			subPeakStart = 0;
			if(wigPosArr[1].getScore() > wigPosArr[0].getScore())
			{
				wayUp = true;
			}
			else
			{
				wayUp = false;
	//add this
				foundMaxima1=true;
			}
			continue;
		}

		if(wayUp)
		{
			if(wigPosArr[i].getScore() >= wigPosArr[i-1].getScore())
			{
				continue;
			}
			else if(wigPosArr[i].getScore() < wigPosArr[i-1].getScore())
			{
				wayUp = false;
				if(!foundMaxima1)
				{
					localMaxima1 = i-1;
					foundMaxima1 = true;
				}
				else
				{
					localMaxima2 = i-1;
					float lowerSummitScore = 0;
					//check who is higher
					if(wigPosArr[localMaxima1].getScore() >= wigPosArr[localMaxima2].getScore())
					{
						lowerSummitScore = wigPosArr[localMaxima2].getScore();
					}
					else
					{
						lowerSummitScore = wigPosArr[localMaxima1].getScore();
					}

					if(lowerSummitScore * m_bd > wigPosArr[localValley].getScore())
					{						
						//significant decrease split peaks
						int summitStart = wigPosArr[localMaxima1].getStart();
						int summitEnd = wigPosArr[localMaxima1].getEnd();
						int k=localMaxima1 - 1;
						while(k>0)
						{
							if(wigPosArr[k].getScore() == wigPosArr[localMaxima1].getScore()){
								summitStart = wigPosArr[k].getStart();
								k--;
							}
							else
								break;
						}
						if(wigPosArr[localMaxima1].getScore() >= m_cutoff)
						{
							SubPeak* subPeak = new SubPeak(peak->getChr(),
								wigPosArr[subPeakStart].getStart(),
								wigPosArr[localValley].getStart(),
								wigPosArr[localMaxima1].getScore(),
								summitStart,
								summitEnd);
							subPeak->Print(fw);
							AddToSortedSet(subPeak);
							delete subPeak;
						}
						//start new sub peak where read count = 0
						for(int j=localValley+1; j< wigPosArr.size();j++)
						{
							if(wigPosArr[j].getScore()>0){
								subPeakStart = j;
								break;
							}
						}
						localMaxima1 = localMaxima2;
						foundLocalValley = false;
					}
					else //no split
					{
						if(wigPosArr[localMaxima2].getScore() >= wigPosArr[localMaxima1].getScore())
						{
							localMaxima1 = localMaxima2;
							foundLocalValley = false;
						}
						continue;
					}
				}
			}
		}
		else //way down
		{
			if(wigPosArr[i].getScore() < wigPosArr[i-1].getScore())
			{
				//check if signal = 0
//					if(wigPosArr[i].getScore()==0){
//							localValley = i;
//					}
				continue;
			}
			else if(wigPosArr[i].getScore() > wigPosArr[i-1].getScore())
			{
				if(!foundLocalValley || wigPosArr[i-1].getScore() < wigPosArr[localValley].getScore())
				{
					//set only if local valley is lower than the current local valley
					localValley = i-1;
					foundLocalValley = true;
				}
				wayUp = true;
			}
		}
	}
	//handle last case
	if(wayUp)
	{
		if(!foundMaxima1 || wigPosArr[wigPosArr.size() - 1].getScore() > wigPosArr[localMaxima1].getScore())
		{
			localMaxima1 = wigPosArr.size() - 1;
		}
	}
	int summitStart = wigPosArr[localMaxima1].getStart();
	int summitEnd = wigPosArr[localMaxima1].getEnd();
	int k=localMaxima1 -1;
	while(k>0)
	{
		if(wigPosArr[k].getScore() == wigPosArr[localMaxima1].getScore()){
			summitStart = wigPosArr[k].getStart();
			k--;
		}
		else
			break;
	}

	if(wigPosArr[localMaxima1].getScore() >= m_cutoff)
	{
		SubPeak* subPeak = new SubPeak(peak->getChr(),
			wigPosArr[subPeakStart].getStart(),
			wigPosArr[wigPosArr.size() - 1].getEnd(),
			wigPosArr[localMaxima1].getScore(),
			summitStart,
			summitEnd);
		subPeak->Print(fw);
		AddToSortedSet(subPeak);
		delete subPeak;
	}
}

list<wigPosition>* Split::getOverlapList(Position* peak, Parser* wig)
{
	//get arraylist and fill holes with zero
	wigPosition* prev = NULL;
	list<wigPosition>* wigSignal = new list<wigPosition>();
	wigPosition* p = (wigPosition*)wig->nextPosition();
	while(p!=NULL && p->getChr() == peak->getChr())
	{
		if(p->contains(*peak)==0) //overlap
		{
			//check if there is a hole
			if(prev!=NULL)
			{
				if(p->getStart()!=prev->getEnd()+1 && p->getStart()!=prev->getEnd())
				{
						//there is a hole
						wigPosition hole(p->getChr(),prev->getEnd()+1,p->getStart()-1,0);
						wigSignal->push_back(hole);
				}
			}
			//check is the first signal is larder than 0.0
			if(p->getScore()>0.0)
			{
				wigSignal->push_back(*p);
			}
			//update prev only when we reached to a region in wig that overlaps the peak
			delete prev;
			prev=p;
			p = (wigPosition*) wig->nextPosition();
		}
		else if (p->contains(*peak)==1)
		{
			//signal is after the peak
			wig->Lineback();
			break;
		}
		else
		{
			//signal before peak
			delete p;
			p = (wigPosition*) wig->nextPosition();
			continue;
		}
	}

	return wigSignal;
}


string Split::Unzip(string inFilePath)
{
	string outFilePath="";
	string parentPath = strip_file_name((char*)inFilePath.c_str());

	char* buf = new char[102400];
    int len;
	int readBytes = 0;

    if(inFilePath.substr(inFilePath.length() - 3,3) == ".gz")
	{		
		char temp[1000];		
		inFilePath.copy(temp,inFilePath.length(),0);			
		outFilePath = strip_extension(temp);

		gzFile file;
		z_off_t pos;

		file = gzopen((char*)inFilePath.c_str(), "rb");
		if (file == NULL) {
			fprintf(stderr, "gzopen error\n");
			exit(1);
		}
		ofstream out(outFilePath.c_str());
		int readBytes = 0;
		while ((len = gzread(file,buf,102400)) > 0)
		{
			if(len == 0)
				break;
	 		readBytes += len;
			out.write(buf, len);
		}
		 gzclose(file);
		 out.close();
	}
	else
	{
		unzFile file = unzOpen((char*)inFilePath.c_str());
		if(unzGoToFirstFile(file)!= UNZ_OK)
		{
			string err = "Failed to read zip file\n";
			throw err;

		}

		do
		{
			if(unzOpenCurrentFile(file) != UNZ_OK)
			{
				string err = "Failed to read zip file\n";
				throw err;
			}
			unz_file_info fileInfo;
			char fileName[1024];
			char extra[1024];
			char comment[1024];
			unzGetCurrentFileInfo(file,&fileInfo,fileName,1024,extra,1024,comment,1024);

	        outFilePath = fileName;
	        if(!parentPath.empty())
	        {
	        	outFilePath = parentPath+ DIR_SEP + fileName;
	        }
			ofstream out(outFilePath.c_str());
			while ((len = unzReadCurrentFile(file,buf,102400)) > 0)
			{
				if(len == 0)
					break;
 				readBytes += len;
				out.write(buf, len);
			}
			out.close();
		}while(unzGoToNextFile(file) != UNZ_END_OF_LIST_OF_FILE);
		unzClose(file);
	}
    return outFilePath;
}

void Split::AddToSortedSet(SubPeak* subPeak)
{
	m_SortedList.insert(*subPeak);
	if(m_SortedList.size() > m_numOfseq)
	{
		m_SortedList.erase(m_SortedList.begin());
	}
}

void Split::getPeakFasta(string dasURL, int seqLength)
{
	cout << "Starting to fetch subpeaks sequences\n";
	string fastaOut = m_OutDir+m_PeakFileNoExtension+".bestSubPeaks.fa";
	ofstream fasta(fastaOut.c_str());

	//go over the sorted peak array, and get the sequence for each binding site
	int count=0;
	std::multiset<SubPeak>::reverse_iterator itr;
	//the list is sorted in ascending order but we want the fasta file
	//to have the sub peaks in decending order
	for(itr = m_SortedList.rbegin();itr != m_SortedList.rend();itr ++)
	{
		count++;
		if(count%10==0)
		{
			cout << count << " sequences have been fetched\n";
		}
		int summitPos = (itr->m_summitStart+itr->m_summitEnd)/2;
		int start = summitPos-(seqLength/2);
		int end = summitPos+(seqLength/2);
		string chr="";
		if(Parser::ToUpper(itr->m_chr).substr(0,3) == "CHR")
		{
			chr=Parser::ToUpper(itr->m_chr).substr(3,itr->m_chr.length());
		}		
		else
		{
			chr=Parser::ToUpper(itr->m_chr);
		}

		if(chr == "M")
		{
			chr = "MT";
		}
		char startbuf[10];
		char endbuf[10];
		sprintf(startbuf, "%d", start);
		sprintf(endbuf, "%d", end);
		string url = dasURL+ "/sequence?segment=" +chr +":"+ startbuf +","+ endbuf;
		SequenceDownloader downloader;
		strstream* rcvSeq = NULL;
		try
		{
			rcvSeq = downloader.GetSequence(url);
		}
		catch(...)
		{
			cout << "Error occured: Couldn't reach \"" << dasURL << "\" check your internet connection" <<endl;
			fasta.close();
			return;
		}
		string inputLine;
		string seq = "";
		bool inSeq = false;
		getline(*rcvSeq,inputLine,'\n');		
		while (!inputLine.empty())
		{
			if(inputLine.substr(0,11) == "</SEQUENCE>")
			{				
				break;
			}
			if(inSeq)
			{
				seq = seq+ Parser::Trim(inputLine);
			}
			else if(inputLine.substr(0,9) == "<SEQUENCE")
			{
				inSeq=true;
				getline(*rcvSeq,seq,'\n');
			}
			getline(*rcvSeq,inputLine,'\n');			
		}
		if(seq == "")
		{
			count--;
			cout << "Couldn't get sequence " << itr->m_chr << ":" << start << "-" << end <<  " from " << dasURL << "\n";
		}
		else
		{
			fasta << ">" << itr->m_chr << ":" << start << "-" << end << "\n" << seq << "\n";
		}
		delete rcvSeq;
	}
	cout << count << " sequences have been fetched\n";
	fasta.close();
}

string Split::getFatsaName()
{
	return m_PeakFileNoExtension+".bestSubPeaks.fa";
}

Position* Split::nextChrom(Parser* wig, PeakParser* peak, Position* curwig)
{
	while(curwig != NULL)
	{		
		Position* pos = wig->nextChrom(curwig->getChr());
		delete curwig;
		curwig = pos;
		if(curwig == NULL)
			return NULL;
		try
		{
			peak->moveToChr(curwig->getChr());
			cout << "Start processing " << curwig->getChr() << " peaks" << endl;
			break;
		}
		catch (...)
		{
			cout << "No peaks were found in chromosome " << curwig->getChr() << endl;
		}			
	}
	return curwig;
}

static bool is_directory (string& dir)
{
    return file_is_directory(dir.c_str());
}

bool FileExists(string& fileName)
{
	return file_exists(fileName.c_str());
}

void DeleteFile(string& fileName)
{
	file_delete(fileName.c_str());
}

// SplitPeaks.cpp : Defines the entry point for the console application.
//


#include "tclap/CmdLine.h"
using namespace TCLAP;
#include "Shared.h"
#include "Split.h"

int main(int argc, char* argv[])
{
	try
	{		
		// Define the command line object.
		CmdLine cmd("This tool split ChIP-SEQ enrichment areas to individual binding sites", ' ', "0.1");

		// Define a value argument and add it to the command line.
		ValueArg<int> length("l","length","length of sequence to fetch (default 60)",false,60,"int",cmd);
                ValueArg<int> numSeq("n","numSeq","number of best peak sequences to fetch (default 300)",false,300,"int",cmd);
                ValueArg<string> url("u","url","Das url (default is for human \"http://www.ensembl.org/das/Homo_sapiens.GRCh37.reference\")",false,"http://www.ensembl.org/das/Homo_sapiens.GRCh37.reference","string",cmd);
                SwitchArg fetch("f","fetch","whether to fetch subpeaks sequences or not",cmd, true);
                ValueArg<float> bd("v","valley","float value to determine the valley depth required for peak separation",false,0.6,"float",cmd);
                ValueArg<int> cutoff("c","cutoff","height cutoff (default 5)",false,5,"int",cmd);
                ValueArg<string> outDir("o","outDir","output folder",true,"","string",cmd);
                ValueArg<string> wigFile("w","wigFile","input wig file or folder",true,"","string",cmd);
                ValueArg<string> peakFile("p","peakFile","input peak file",true,"","string",cmd);

		//match the arguments, and handle the un-matched
		cmd.parse( argc, argv );

		if(!peakFile.isSet() || !wigFile.isSet() || !outDir.isSet())
		{
			string help = cmd.getMessage();
			cout << help << "\n";
			return -1;
		}
		else
		{
			Split* s = new Split();
			s->init(peakFile.getValue(),wigFile.getValue(),bd.getValue(),cutoff.getValue(),outDir.getValue(),numSeq.getValue());
			if(fetch.getValue())
			{
				s->getPeakFasta(url.getValue(), length.getValue());
			}
			delete s;
			cout << "program ended successfully" << endl;
		}
	}
	catch (ArgException &e)  // catch any exceptions
	{
		cerr << "error occurred: " << e.error() << " for arg " << e.argId() << endl;
	}	
	catch (exception e)
	{
		cerr << "error occurred: " << e.what() << endl;
	}
	catch(string s)
	{
		cerr << "error occurred: " << s.c_str() << endl;
	}	
}

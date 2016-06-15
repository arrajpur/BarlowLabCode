#include "SequenceDownloader.h"
#include "Parser.h"
#ifdef WIN32
#include <iostream>
#include "Urlmon.h"
#include <fstream>
#include <strstream>
using namespace std;

#else
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <netdb.h>
#include <iostream>
using namespace std;
#define RCVBUFSIZE 1024   /* Size of receive buffer */

#endif
SequenceDownloader::SequenceDownloader(void)
{
}

SequenceDownloader::~SequenceDownloader(void)
{
}
strstream* SequenceDownloader::GetSequence(string queryURL)
{
	strstream* memStream = new strstream();

	#ifdef WIN32
	//just in case
	DeleteFile("tmp_seq.xml");
	int res = URLDownloadToFileA(NULL,queryURL.c_str(),"tmp_seq.xml", 0,NULL);
	if(res != 0)
	{
		string err = "Failed to download file\n";
		throw err;
	}
	ifstream is("tmp_seq.xml");
	is.seekg(0, ios::end);
	int size = is.tellg();
	is.seekg(0);

	char* data = new char[size];
	is.read(data,size);
	memStream->write(data,size);
	delete data;
	is.close();
	DeleteFile("tmp_seq.xml");
	#else

    int sock;                        /* Socket descriptor */
    struct sockaddr_in ServAddr;	 /* server address */
    unsigned short ServPort = 80;    /* server port */
    string servIP;                   /* Server IP address (dotted quad) */
    string queryString;              /* String to send to echo server */
    char rcvBuffer[RCVBUFSIZE];      /* Buffer for received string */
    unsigned int sndStringLen;       /* Length of string to send*/
    int bytesRcvd;				     /* Bytes read in single recv()*/    

	vector<string> vec;
	Parser::Tokenize(queryURL,vec,"/");
	if(vec.size() >= 2)
	{
		servIP = vec[1];
	}
	else
	{
		string err = "Invalid URL\n"; 
		throw err;
	}
    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
        string err = "socket() failed\n";
		throw err;
	}
    /* Construct the server address structure */
    memset(&ServAddr, 0, sizeof(ServAddr));     /* Zero out structure */
    ServAddr.sin_family = AF_INET;     /* Internet address family */
    {
		struct in_addr a;
		struct hostent *h=gethostbyname(servIP.c_str());
		if(h == NULL)
		{
			string err = "Unknown host\n";
			throw err;
		}
		memcpy(&a, h->h_addr_list[0], sizeof(a));

		servIP=inet_ntoa(a);
    }
    ServAddr.sin_addr.s_addr = inet_addr(servIP.c_str());   /* Server IP address */

    ServAddr.sin_port = htons(ServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
	{
		string err = "connect() failed\n";
		throw err;
	}
	queryString = "GET " + queryURL + "\r\nHTTP/1.1\r\n";
    sndStringLen = queryString.length();          /* Determine input length */

    /* Send the string to the server */
    if (send(sock, queryString.c_str(), sndStringLen, 0) != sndStringLen)
	{
		string err = "send() sent a different number of bytes than expected\n";
		throw err;
	}

    /* Receive the same string back from the server */
    do
    {
        /* Receive up to the buffer size (minus 1 to leave space for
           a null terminator) bytes from the sender */
        bytesRcvd = recv(sock, rcvBuffer, RCVBUFSIZE - 1, 0);
		if(bytesRcvd < 0)
		{
			string err = "recv() failed or connection closed prematurely\n";
			throw err;
		}
        rcvBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
		memStream->write(rcvBuffer,bytesRcvd);
    } while( bytesRcvd > 0 );

    close(sock);
#endif
	return memStream;
}//-- end main --//


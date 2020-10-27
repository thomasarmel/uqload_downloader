#include <iostream>
#include <iomanip>
#include "Downloader.h"

using namespace std;

int downloadCallback(void* p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		cerr << "Usage: " << argv[0] << "https://uqload.com/embed-xxxxxxx.html movie.mp4" << endl;
		return -1;
	}
	string pageInit = string(argv[1]), outputFile=string(argv[2]);
	Downloader *uqDownloader=new Downloader(pageInit, outputFile);
	uqDownloader->setDownloadCallback(downloadCallback);
	try
	{
		uqDownloader->download();
	}
	catch (const string& e)
	{
		cerr << e << endl;
	}
	return 0;
}

int downloadCallback(void* p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	if (dltotal != 0.0)
	{
		cout << setprecision(3) << ((double)dlnow / (double)dltotal) * 100 << " %\r" << flush;
	}
	return 0;
}


/**
https://uqload.com/embed-h63yfu9dkw1r.html
test.mp4
*/
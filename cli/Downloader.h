#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <iostream>
#include <string>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <io.h>
#define uqdCheckAccess(pathname, mode) _access(pathname, mode)
#else
#include <unistd.h>
#include <cassert>
#define uqdCheckAccess(pathname, mode) access(pathname, mode)
error_t fopen_s(FILE** f, const char* name, const char* mode);
#endif
#include <curl/curl.h>
#include "DownloaderListener.h"

class Downloader
{
public:
	Downloader();
	Downloader(std::string uqUrl, std::string outputFile, DownloaderListener *downloaderListener=nullptr);
	~Downloader();
	void setUqUrl(std::string uqUrl);
	void setOutputFile(std::string outputFile);
    void setListener(DownloaderListener *downloaderListener);
	//void setDownloadCallback(void* callback);
	void download();

private:
	static size_t writefunc(void *curl, size_t size, std::size_t nmemb, std::string *s);
    static int downloadCallback(void* p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
	std::string m_uqUrl, m_outputFile;
	CURL* m_curl;
	CURLcode m_res;
	FILE* m_fp;
	//void* m_downloadCallback;
    DownloaderListener *m_downloaderListener = nullptr;
};

#endif // DOWNLOADER_H
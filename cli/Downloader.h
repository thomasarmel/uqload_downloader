#pragma once

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

class Downloader
{
public:
	Downloader();
	Downloader(std::string uqUrl, std::string outputFile);
	~Downloader();
	void setUqUrl(std::string uqUrl);
	void setOutputFile(std::string outputFile);
	void setDownloadCallback(void* callback);
	void download();

private:
	static size_t writefunc(void *curl, size_t size, std::size_t nmemb, std::string *s);
	std::string m_uqUrl, m_outputFile;
	CURL* m_curl;
	CURLcode m_res;
	FILE* m_fp;
	void* m_downloadCallback;
};

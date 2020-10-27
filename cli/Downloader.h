#pragma once

#include <iostream>
#include <string>
#include <io.h>
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


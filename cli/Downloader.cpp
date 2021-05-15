#include "Downloader.h"

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
error_t fopen_s(FILE** f, const char* name, const char* mode)
{ // https://stackoverflow.com/questions/1513209/is-there-a-way-to-use-fopen-s-with-gcc-or-at-least-create-a-define-about-it
	error_t ret = 0;
	assert(f);
	*f = fopen(name, mode);
	if (!*f)
	{
		ret = errno;
	}
	return ret;
}
#endif

using namespace std;

Downloader::Downloader()
{
	m_fp = nullptr;
	m_downloadCallback = nullptr;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	m_curl = curl_easy_init();
	curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(m_curl, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt(m_curl, CURLOPT_COOKIEJAR, "");
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
}

Downloader::Downloader(std::string uqUrl, std::string outputFile) : Downloader()
{
	setUqUrl(uqUrl);
	setOutputFile(outputFile);
}

void Downloader::setUqUrl(std::string uqUrl)
{
	m_uqUrl = uqUrl;
}

void Downloader::setOutputFile(std::string outputFile)
{
	m_outputFile = outputFile;
}

void Downloader::setDownloadCallback(void* callback)
{
	if (callback != nullptr)
	{
		curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, false);
		m_downloadCallback = callback;
	}
}

Downloader::~Downloader()
{
	if (m_curl)
	{
		curl_easy_cleanup(m_curl);
		curl_global_cleanup();
	}
	if (m_fp != nullptr)
	{
		fclose(m_fp);
	}
}

size_t Downloader::writefunc(void* ptr, size_t size, size_t nmemb, std::string* s)
{
	s->append(static_cast<char*>(ptr), size * nmemb);
	return size * nmemb;
}

void Downloader::download()
{
	if (m_uqUrl == "" || m_outputFile == "")
	{
		throw string("Error: please specify URL and output file");
	}
	if (!m_curl)
	{
		throw string("Error: curl init");
	}
	if (uqdCheckAccess(m_outputFile.c_str(), 0) == 0)
	{
		throw string("Error: " + m_outputFile + " already exists");
	}
	fopen_s(&m_fp, m_outputFile.c_str(), "wb");
	string strHTML;
	curl_easy_setopt(m_curl, CURLOPT_URL, m_uqUrl.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &Downloader::writefunc);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &strHTML);
	m_res = curl_easy_perform(m_curl);
	if (m_res != CURLE_OK)
	{
		throw string("Error: curl_easy_perform() failed: " + string(curl_easy_strerror(m_res)));
	}
	size_t posBegVid = strHTML.find("sources: [\"https://");
	size_t posEndVid = strHTML.find("/v.mp4\"]");
	if (posBegVid == std::string::npos || posEndVid == std::string::npos)
	{
		throw string("Video URL not found");
	}
	string videoURL = strHTML.substr(posBegVid + 11, posEndVid - posBegVid - 5);

	if (m_downloadCallback != nullptr)
	{
		curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION, m_downloadCallback);
	}

	curl_easy_setopt(m_curl, CURLOPT_URL, videoURL.c_str());
	curl_easy_setopt(m_curl, CURLOPT_REFERER, m_uqUrl.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, m_fp);
	m_res = curl_easy_perform(m_curl);
	if (m_res != CURLE_OK)
	{
		throw string("Error: curl_easy_perform() failed: " + string(curl_easy_strerror(m_res)));
	}
}

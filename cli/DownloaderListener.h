#ifndef UQLOAD_DOWNLOADER_DOWNLOADERLISTENER_H
#define UQLOAD_DOWNLOADER_DOWNLOADERLISTENER_H

#include <curl/curl.h>
#include "Downloader.h"

class DownloaderListener
{
protected:
    friend class Downloader;
    virtual int
    downloadCallback(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) = 0;
};


#endif //UQLOAD_DOWNLOADER_DOWNLOADERLISTENER_H

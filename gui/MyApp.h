#ifndef UQLOAD_DOWNLOADER_MYAPP_H
#define UQLOAD_DOWNLOADER_MYAPP_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include <wx/filepicker.h>
#include <wx/uri.h>
#include "MainFrame.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

DECLARE_APP(MyApp);

#endif //UQLOAD_DOWNLOADER_MYAPP_H

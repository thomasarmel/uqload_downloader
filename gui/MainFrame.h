#ifndef UQLOAD_DOWNLOADER_MAINFRAME_H
#define UQLOAD_DOWNLOADER_MAINFRAME_H

#include <thread>
#include "MyApp.h"
#include "../cli/Downloader.h"

class MainFrame : public wxFrame, public DownloaderListener
{
public:
    MainFrame(const wxString& title);
    ~MainFrame();

    int downloadCallback(void* p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

private:
    void onStartDownloadClicked(wxCommandEvent& event);
    //friend static int staticDownloadCallback(MainFrame *mainFrameInstance, void* p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

    wxPanel *mainPanel;
    wxStaticText *titleLabel;
    wxStaticText *URLLabel;
    wxTextCtrl *URLTextCtrl;
    wxStaticText *fileDestLabel;
    wxFilePickerCtrl *fileDestPicker;
    wxButton *startDownloadButton;
    wxStaticText *downloadProgressLabel;
    wxGauge *downloadProgressGauge;

    wxString URLTextCtrlHint;

    Downloader *uqDownloader = nullptr;

    DECLARE_EVENT_TABLE()

    enum
    {
        ID_URLTEXTCTRL = wxID_HIGHEST + 1,
        ID_DESTFILEPICKER,
        ID_STARTDOWNLOADBUTTON,
        ID_DOWNLOADPROGRESSGAUGE,
    };
};


#endif //UQLOAD_DOWNLOADER_MAINFRAME_H

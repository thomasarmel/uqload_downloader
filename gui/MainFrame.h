#ifndef UQLOAD_DOWNLOADER_MAINFRAME_H
#define UQLOAD_DOWNLOADER_MAINFRAME_H

#include <thread>
#include <atomic>
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
    void onStopDownloadClicked(wxCommandEvent& event);
    void onClose(wxCloseEvent &event);
    void startDownload();

    wxPanel *m_mainPanel;
    wxStaticText *m_titleLabel;
    wxStaticText *URLLabel;
    wxTextCtrl *URLTextCtrl;
    wxStaticText *fileDestLabel;
    wxFilePickerCtrl *fileDestPicker;
    wxButton *startDownloadButton;
    wxButton *stopDownloadButton;
    wxStaticText *downloadProgressLabel;
    wxGauge *downloadProgressGauge;

    wxString URLTextCtrlHint, fileName;

    Downloader *uqDownloader = nullptr;
    std::thread *asyncDownloadThread = nullptr; // replace with unique_ptr ?

    std::atomic<bool> shouldStopDownload = ATOMIC_VAR_INIT(false);

    DECLARE_EVENT_TABLE()

    enum
    {
        ID_URLTEXTCTRL = wxID_HIGHEST + 1,
        ID_DESTFILEPICKER,
        ID_STARTDOWNLOADBUTTON,
        ID_STOPDOWNLOADBUTTON,
        ID_DOWNLOADPROGRESSGAUGE,
    };
};


#endif //UQLOAD_DOWNLOADER_MAINFRAME_H

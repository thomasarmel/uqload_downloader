#ifndef UQLOAD_DOWNLOADER_MAINFRAME_H
#define UQLOAD_DOWNLOADER_MAINFRAME_H

#include <thread>
#include <atomic>
#include "MyApp.h"
#include "../cli/Downloader.h"

class MainFrame : public wxFrame, public DownloaderListener
{
public:
    MainFrame(const wxString &title);

    ~MainFrame();

private:
    int downloadCallback(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

    void onStartDownloadClicked(wxCommandEvent &event);

    void onStopDownloadClicked(wxCommandEvent &event);

    void onClose(wxCloseEvent &event);

    void startDownload();

    wxPanel *m_mainPanel;
    wxStaticText *m_titleLabel;
    wxStaticText *m_URLLabel;
    wxTextCtrl *m_URLTextCtrl;
    wxStaticText *m_fileDestLabel;
    wxFilePickerCtrl *m_fileDestPicker;
    wxButton *m_startDownloadButton;
    wxButton *m_stopDownloadButton;
    wxStaticText *m_downloadProgressLabel;
    wxGauge *m_downloadProgressGauge;

    wxString m_URLTextCtrlHint, m_fileName;

    Downloader *m_uqDownloader = nullptr;
    std::thread *m_asyncDownloadThread = nullptr; // replace with unique_ptr ?

    std::atomic<bool> m_shouldStopDownload = ATOMIC_VAR_INIT(false);

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

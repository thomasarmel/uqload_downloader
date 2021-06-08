#include "MainFrame.h"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON(ID_STARTDOWNLOADBUTTON, MainFrame::onStartDownloadClicked)
                EVT_BUTTON(ID_STOPDOWNLOADBUTTON, MainFrame::onStopDownloadClicked)
                EVT_CLOSE(MainFrame::onClose)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 500),
                                                      (!wxRESIZE_BORDER) | wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX |
                                                      wxMINIMIZE_BOX)
{
    SetIcon(wxICON(ApplicationIcon));
    wxFont font = GetFont();
    font.SetSymbolicSize(wxFONTSIZE_LARGE);
    SetFont(font);
    m_mainPanel = new wxPanel(this);
    m_titleLabel = new wxStaticText(m_mainPanel, wxID_ANY, wxT("UQLoad Downloader"), wxPoint(100, 20));
    font = m_titleLabel->GetFont();
    font.MakeBold();
    font.SetSymbolicSize(wxFONTSIZE_XX_LARGE);
    m_titleLabel->SetFont(font);
    m_URLLabel = new wxStaticText(m_mainPanel, wxID_ANY, wxT("URL:"), wxPoint(20, 100));
    m_URLTextCtrl = new wxTextCtrl(m_mainPanel, ID_URLTEXTCTRL, wxEmptyString, wxPoint(20, 130), wxSize(400, -1),
                                   wxTE_AUTO_URL);
    m_URLTextCtrlHint = wxT("https://uqload.com/embed-xxxxxxx.html");
    m_URLTextCtrl->SetHint(m_URLTextCtrlHint);
    m_fileDestLabel = new wxStaticText(m_mainPanel, wxID_ANY, wxT("Destination file:"), wxPoint(20, 200));
    m_fileDestPicker = new wxFilePickerCtrl(m_mainPanel, ID_DESTFILEPICKER, wxEmptyString,
                                            wxT("Please indicate a non-existing file for saving your movie"),
                                            _T("MP4 file (*.mp4)|*.mp4|All files (*.*)|*.*"), wxPoint(20, 230),
                                            wxSize(400, -1),
                                            wxFLP_SAVE | wxFLP_OVERWRITE_PROMPT /*| wxFLP_CHANGE_DIR*/ |
                                            wxFLP_USE_TEXTCTRL);
    m_startDownloadButton = new wxButton(m_mainPanel, ID_STARTDOWNLOADBUTTON, wxT("START"), wxPoint(20, 300),
                                         wxDefaultSize);
    m_stopDownloadButton = new wxButton(m_mainPanel, ID_STOPDOWNLOADBUTTON, wxT("STOP"), wxPoint(150, 300),
                                        wxDefaultSize);
    font = m_startDownloadButton->GetFont();
    font.MakeBold();
    m_startDownloadButton->SetFont(font);
    m_stopDownloadButton->SetFont(font);
    m_stopDownloadButton->Disable();
    m_downloadProgressLabel = new wxStaticText(m_mainPanel, wxID_ANY, wxT("Download Progress:"), wxPoint(20, 370));
    m_downloadProgressGauge = new wxGauge(m_mainPanel, ID_DOWNLOADPROGRESSGAUGE, 100, wxPoint(20, 400),
                                          wxSize(400, 35));
    m_downloadProgressGauge->SetForegroundColour(*wxGREEN);
    m_uqDownloader = new Downloader();
}

MainFrame::~MainFrame()
{
}

void MainFrame::onClose(wxCloseEvent &event)
{
    if (m_asyncDownloadThread != nullptr)
    {
        delete m_asyncDownloadThread;
    }
    delete m_uqDownloader; // slow
    Destroy();
}

void MainFrame::onStartDownloadClicked(wxCommandEvent &event)
{
    m_fileName = m_fileDestPicker->GetPath();
    wxString URL = m_URLTextCtrl->GetValue();
    if (m_fileName.IsEmpty())
    {
        wxMessageBox(wxT("Please specify a file where movie will be saved."), wxT("File name error"), wxICON_WARNING);
        return;
    }
    if (URL.IsEmpty() || URL.IsSameAs(m_URLTextCtrlHint))
    {
        wxMessageBox(wxT("Please specify a valid UQLoad URL."), wxT("URL error"), wxICON_WARNING);
        return;
    }
    wxURI checkURL(URL);
    if (!checkURL.HasScheme() || !checkURL.HasServer() || !checkURL.HasPath())
    {
        wxMessageBox(wxT("Please specify a valid UQLoad URL."), wxT("URL error"), wxICON_WARNING);
        return;
    }

    wxFileName checkFileName(m_fileName);
    if (!checkFileName.IsOk() || checkFileName.IsDir())
    {
        wxMessageBox(wxT("Please specify a valid file name."), wxT("File name error"), wxICON_WARNING);
        return;
    }
    if (checkFileName.FileExists() && !wxRemoveFile(m_fileName))
    {
        wxMessageBox(wxT("Can't replace specified file content."), wxT("File permissions error"), wxICON_WARNING);
        return;
    }
    //m_uqDownloader=new Downloader(URL.ToStdString(), m_fileName.ToStdString(), this);
    m_uqDownloader->setUqUrl(URL.ToStdString());
    m_uqDownloader->setOutputFile(m_fileName.ToStdString());
    m_uqDownloader->setListener(this);
    m_shouldStopDownload.store(false);
    try
    {
        if (m_asyncDownloadThread != nullptr)
        {
            delete m_asyncDownloadThread;
            m_asyncDownloadThread = nullptr;
        }
        m_asyncDownloadThread = new std::thread(&MainFrame::startDownload, this);
        m_asyncDownloadThread->detach();

    }
    catch (const std::string &e)
    {
        wxMessageBox(wxString(e), wxT("Download error"), wxICON_WARNING);
        return;
    }
}

int MainFrame::downloadCallback(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    if (dltotal == 0.0)
    {
        return 0;
    }
    m_downloadProgressGauge->SetValue((int) (dlnow * 100 / dltotal));
    if (m_shouldStopDownload.load())
    {
        return 1;
    }
    return 0;
}

void MainFrame::startDownload()
{
    if (m_uqDownloader == nullptr)
    {
        wxMessageBox(wxT("Pointer on downloader is null."), wxT("Pointer error"), wxICON_ERROR);
        return;
    }
    m_downloadProgressGauge->SetValue(0);
    m_downloadProgressGauge->SetForegroundColour(*wxGREEN);
    m_startDownloadButton->Disable();
    m_stopDownloadButton->Enable();
    try
    {
        m_uqDownloader->download();
    }
    catch (const std::string &e)
    {
        m_shouldStopDownload.store(true);
    }
    m_startDownloadButton->Enable();
    m_stopDownloadButton->Disable();
    if (!m_shouldStopDownload.load())
    {
        m_downloadProgressGauge->SetForegroundColour(*wxBLUE);
        wxMessageBox(wxT("Your movie has been downloaded."), wxT("Download done"), wxICON_INFORMATION);
    } else
    {
        m_downloadProgressGauge->SetForegroundColour(*wxRED);
        m_downloadProgressGauge->SetValue(0);
        wxRemoveFile(m_fileName);
    }
}

void MainFrame::onStopDownloadClicked(wxCommandEvent &event)
{
    m_shouldStopDownload.store(true);
}
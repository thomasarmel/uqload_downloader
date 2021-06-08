#include "MainFrame.h"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON(ID_STARTDOWNLOADBUTTON, MainFrame::onStartDownloadClicked)
                EVT_BUTTON(ID_STOPDOWNLOADBUTTON, MainFrame::onStopDownloadClicked)
                EVT_CLOSE(MainFrame::onClose)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 500), (!wxRESIZE_BORDER) | wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxMINIMIZE_BOX)
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
    URLLabel = new wxStaticText(m_mainPanel, wxID_ANY, wxT("URL:"), wxPoint(20, 100));
    URLTextCtrl = new wxTextCtrl(m_mainPanel, ID_URLTEXTCTRL, wxEmptyString, wxPoint(20, 130), wxSize(400, -1), wxTE_AUTO_URL);
    URLTextCtrlHint = wxT("https://uqload.com/embed-xxxxxxx.html");
    URLTextCtrl->SetHint(URLTextCtrlHint);
    fileDestLabel = new wxStaticText(m_mainPanel, wxID_ANY, wxT("Destination file:"), wxPoint(20, 200));
    fileDestPicker = new wxFilePickerCtrl(m_mainPanel, ID_DESTFILEPICKER, wxEmptyString, wxT("Please indicate a non-existing file for saving your movie"), _T("MP4 file (*.mp4)|*.mp4|All files (*.*)|*.*"), wxPoint(20, 230), wxSize(400, -1), wxFLP_SAVE | wxFLP_OVERWRITE_PROMPT /*| wxFLP_CHANGE_DIR*/ | wxFLP_USE_TEXTCTRL);
    startDownloadButton = new wxButton(m_mainPanel, ID_STARTDOWNLOADBUTTON, wxT("START"), wxPoint(20, 300), wxDefaultSize);
    stopDownloadButton = new wxButton(m_mainPanel, ID_STOPDOWNLOADBUTTON, wxT("STOP"), wxPoint(150, 300), wxDefaultSize);
    font = startDownloadButton->GetFont();
    font.MakeBold();
    startDownloadButton->SetFont(font);
    stopDownloadButton->SetFont(font);
    stopDownloadButton->Disable();
    downloadProgressLabel = new wxStaticText(m_mainPanel, wxID_ANY, wxT("Download Progress:"), wxPoint(20, 370));
    downloadProgressGauge = new wxGauge(m_mainPanel, ID_DOWNLOADPROGRESSGAUGE, 100, wxPoint(20, 400), wxSize(400, 35));
    downloadProgressGauge->SetForegroundColour(*wxGREEN);
    uqDownloader=new Downloader();
}

MainFrame::~MainFrame()
{
}

void MainFrame::onClose(wxCloseEvent &event)
{
    if(asyncDownloadThread != nullptr)
    {
        delete asyncDownloadThread;
    }
    delete uqDownloader; // slow
    Destroy();
}

void MainFrame::onStartDownloadClicked(wxCommandEvent& event)
{
    fileName = fileDestPicker->GetPath();
    wxString URL = URLTextCtrl->GetValue();
    if(fileName.IsEmpty())
    {
        wxMessageBox(wxT("Please specify a file where movie will be saved."), wxT("File name error"), wxICON_WARNING);
        return;
    }
    if(URL.IsEmpty() || URL.IsSameAs(URLTextCtrlHint))
    {
        wxMessageBox(wxT("Please specify a valid UQLoad URL."), wxT("URL error"), wxICON_WARNING);
        return;
    }
    wxURI checkURL(URL);
    if(!checkURL.HasScheme() || !checkURL.HasServer() || !checkURL.HasPath())
    {
        wxMessageBox(wxT("Please specify a valid UQLoad URL."), wxT("URL error"), wxICON_WARNING);
        return;
    }

    wxFileName checkFileName(fileName);
    if(!checkFileName.IsOk() || checkFileName.IsDir())
    {
        wxMessageBox(wxT("Please specify a valid file name."), wxT("File name error"), wxICON_WARNING);
        return;
    }
    if(checkFileName.FileExists() && !wxRemoveFile(fileName))
    {
        wxMessageBox(wxT("Can't replace specified file content."), wxT("File permissions error"), wxICON_WARNING);
        return;
    }
    //uqDownloader=new Downloader(URL.ToStdString(), fileName.ToStdString(), this);
    uqDownloader->setUqUrl(URL.ToStdString());
    uqDownloader->setOutputFile(fileName.ToStdString());
    uqDownloader->setListener(this);
    shouldStopDownload.store(false);
    try
    {
        if(asyncDownloadThread != nullptr)
        {
            delete asyncDownloadThread;
            asyncDownloadThread= nullptr;
        }
        asyncDownloadThread=new std::thread(&MainFrame::startDownload, this);
        asyncDownloadThread->detach();

    }
    catch (const std::string& e)
    {
        wxMessageBox(wxString(e), wxT("Download error"), wxICON_WARNING);
        return;
    }
}

int MainFrame::downloadCallback(void* p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    if (dltotal == 0.0)
    {
        return 0;
    }
    downloadProgressGauge->SetValue((int)(dlnow * 100/dltotal));
    if(shouldStopDownload.load())
    {
        return 1;
    }
    return 0;
}

void MainFrame::startDownload()
{
    if(uqDownloader == nullptr)
    {
        wxMessageBox(wxT("Pointer on downloader is null."), wxT("Pointer error"), wxICON_ERROR);
        return;
    }
    downloadProgressGauge->SetValue(0);
    downloadProgressGauge->SetForegroundColour(*wxGREEN);
    startDownloadButton->Disable();
    stopDownloadButton->Enable();
    try
    {
        uqDownloader->download();
    }
    catch (const std::string& e)
    {
        shouldStopDownload.store(true);
    }
    startDownloadButton->Enable();
    stopDownloadButton->Disable();
    if(!shouldStopDownload.load())
    {
        downloadProgressGauge->SetForegroundColour(*wxBLUE);
        wxMessageBox(wxT("Your movie has been downloaded."), wxT("Download done"), wxICON_INFORMATION);
    }
    else
    {
        downloadProgressGauge->SetForegroundColour(*wxRED);
        downloadProgressGauge->SetValue(0);
        wxRemoveFile(fileName);
    }
}

void MainFrame::onStopDownloadClicked(wxCommandEvent& event)
{
    shouldStopDownload.store(true);
}
#include "MainFrame.h"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON(ID_STARTDOWNLOADBUTTON, MainFrame::onStartDownloadClicked)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 500), (!wxRESIZE_BORDER) | wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxMINIMIZE_BOX)
{
    wxFont font = GetFont();
    font.SetSymbolicSize(wxFONTSIZE_LARGE);
    SetFont(font);
    mainPanel = new wxPanel(this);
    titleLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("UQLoad Downloader"), wxPoint(100, 20));
    font = titleLabel->GetFont();
    font.MakeBold();
    font.SetSymbolicSize(wxFONTSIZE_XX_LARGE);
    titleLabel->SetFont(font);
    URLLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("URL:"), wxPoint(20, 100));
    URLTextCtrl = new wxTextCtrl(mainPanel, ID_URLTEXTCTRL, wxEmptyString, wxPoint(20, 130), wxSize(400, -1), wxTE_AUTO_URL);
    URLTextCtrlHint = wxT("https://uqload.com/embed-xxxxxxx.html");
    URLTextCtrl->SetHint(URLTextCtrlHint);
    fileDestLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Destination file:"), wxPoint(20, 200));
    fileDestPicker = new wxFilePickerCtrl(mainPanel, ID_DESTFILEPICKER, wxEmptyString, wxT("Please indicate a non-existing file for saving your movie"), _T("MP4 file (*.mp4)|*.mp4|All files (*.*)|*.*"), wxPoint(20, 230), wxSize(400, -1), wxFLP_SAVE | wxFLP_OVERWRITE_PROMPT /*| wxFLP_CHANGE_DIR*/ | wxFLP_USE_TEXTCTRL);
    startDownloadButton = new wxButton(mainPanel, ID_STARTDOWNLOADBUTTON, wxT("START"), wxPoint(20, 300), wxDefaultSize);
    font = startDownloadButton->GetFont();
    font.MakeBold();
    startDownloadButton->SetFont(font);
    downloadProgressLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Download Progress:"), wxPoint(20, 370));
    downloadProgressGauge = new wxGauge(mainPanel, ID_DOWNLOADPROGRESSGAUGE, 100, wxPoint(20, 400), wxSize(400, 35));
}

MainFrame::~MainFrame()
{}

void MainFrame::onStartDownloadClicked(wxCommandEvent& event)
{
    wxString fileName = fileDestPicker->GetPath();
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
    uqDownloader=new Downloader(URL.ToStdString(), fileName.ToStdString(), this);
    try
    {
        downloadProgressGauge->SetValue(0);
        std::thread *asyncDownloadThread=new std::thread(&Downloader::download, uqDownloader);
        //uqDownloader->download();
        asyncDownloadThread->detach();
        wxMessageBox(wxT("Your movie has been downloaded."), wxT("Download done"), wxICON_INFORMATION);

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
    return 0;
}

/*static int staticDownloadCallback(MainFrame *mainFrameInstance, void* p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    return mainFrameInstance->downloadCallback(p, dltotal, dlnow, ultotal, ulnow);
}*/
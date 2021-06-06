#include "MyApp.h"

IMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MainFrame *frm=new MainFrame(_T("UQLoad Downloader"));
    frm->Show();
    return true;
}
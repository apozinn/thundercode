#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <wx/fs_mem.h>
#include <wx/textdlg.h>
#include <wx/sysopt.h>
#include <wx/splitter.h>
#include <wx/socket.h>
#include <wx/aboutdlg.h>
#include <wx/utils.h>
#include <wx/nativewin.h>
#include <wx/process.h>
#include <wx/filefn.h> 
#include <wx/infobar.h>
#include <wx/log.h>
#include <wx/stc/stc.h>
#include "wx/image.h"
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/mstream.h"
#include "wx/wfstream.h"
#include "wx/quantize.h"
#include "wx/stopwatch.h"
#include <wx/accel.h>
#include <wx/sizer.h>
#include <string>
#include <wx/dirdlg.h>
#include <iostream>
#include <filesystem>

#if wxUSE_CLIPBOARD
#include "wx/dataobj.h"
#include "wx/clipbrd.h"
#endif 

#ifdef __WXMSW__
#include "wx/msw/private.h"
#endif

#include "./app.h"
#include "./components/paintButton.cpp"
#include "./components/tabs.cpp"
#include "./components/navigation.cpp"
#include "./components/sideNavigation.cpp"

//code container component
#include "./components/codeContainer/code.hpp"
#include "./components/codeContainer/code.cpp"

class MyApp: public wxApp {
    virtual bool OnInit();
};

class MainFrame: public wxFrame {
    public:
        MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnNewFile(wxCommandEvent& event);
        void OnOpenFolder(wxCommandEvent& event);
        void OnOpenFile(wxCommandEvent& event);

        void OnPrev(wxCommandEvent& event);
    private:
        wxPanel* code_editor_comp;
        wxButton* hidde_menutools;
        Navigation* navigation_comp;
        SideNavigation* side_navigation_comp;
        Tabs* tabs_container;

        CodeContainer* codeContainer;
        wxDECLARE_NO_COPY_CLASS(MainFrame);
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit() {
    MainFrame *frame = new MainFrame(
     _("ThunderCode"), wxPoint(50, 50),  wxSize(500,400));
    frame->Show(true);
    SetTopWindow(frame);
    return true;
} 

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame( NULL, -1, title, pos, size )
{
    if(wxFile::Exists("./icons/settings.png")) icons_dir = "./icons/";
    else if(wxFile::Exists("../icons/settings.png")) icons_dir = "../icons/";
    else wxLogWarning("Can't find icons dir!");

    //global_main
    wxSplitterWindow *global_main = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    global_main->SetSashInvisible(true);

     //bottom
    wxPanel *status_bar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    status_bar->SetBackgroundColour(wxColor(45,120,210));

    //barra de navegação(esquerda)
    wxPanel *global_navigation = new wxPanel(global_main, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    //side navigation menu
    side_navigation_comp = new SideNavigation(global_navigation);

    //code box(right)
    wxPanel *code_content = new wxPanel(global_main, ID_CODE_BLOCK, wxDefaultPosition, wxDefaultSize);

    //tabs
    tabs_container = new Tabs(code_content);

    //navigation_files
    navigation_comp = new Navigation(global_navigation, tabs_container);

    //global_navigation sizer
    wxBoxSizer *global_navigation_sizer = new wxBoxSizer(wxHORIZONTAL);
    global_navigation_sizer->Add(side_navigation_comp, 3, wxEXPAND);
    global_navigation_sizer->Add(navigation_comp, 16, wxEXPAND);
    global_navigation->SetSizerAndFit(global_navigation_sizer);

    //code container
    codeContainer = new CodeContainer(code_content, ID_CODE_CONTAINER);

    //code_content boxSizer
    wxBoxSizer *code_content_sizer = new wxBoxSizer(wxVERTICAL);
    code_content_sizer->Add(tabs_container, 0, wxEXPAND);
    code_content_sizer->Add(codeContainer, 1, wxEXPAND);
    code_content->SetSizerAndFit(code_content_sizer);

    //global_main sizer
    wxBoxSizer *global_main_sizer = new wxBoxSizer(wxHORIZONTAL);
    global_main_sizer->Add(global_navigation, 2, wxEXPAND);
    global_main_sizer->Add(code_content, 13, wxEXPAND);

    global_main->SetMinimumPaneSize(300);
    global_main->SplitVertically(global_navigation, code_content, 1);
    global_main->SetSizerAndFit(global_main_sizer);

    //global sizer
    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(global_main, 30, wxEXPAND);
    main_sizer->Add(status_bar, 1, wxEXPAND);

    this->SetSizerAndFit(main_sizer);

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_NEW_FILE, _("&New File\tCtrl-N"));
    menuFile->Append(ID_OPEN_FOLDER, _("&Open folder\tCtrl-o-k"));
    menuFile->Append(wxID_OPEN, _("&Open file"));
    menuFile->Append(wxID_SAVE, _("&Save"));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, _("&Exit"));

    wxMenu *menuEdit = new wxMenu;
    menuEdit->Append(wxID_ANY, _("&Edit"));

    wxMenu *menuSelection = new wxMenu;
    menuSelection->Append(wxID_ANY, _("&Selection"));

    wxMenu *menuFind = new wxMenu;
    menuFind->Append(wxID_ANY, _("&Find"));

    wxMenu *menuView = new wxMenu;
    menuView->Append(wxID_ANY, _("&View"));

    wxMenu *menuTools = new wxMenu;
    menuTools->Append(wxID_ANY, _("&Tools"));

    wxMenu *menuPreference = new wxMenu;
    menuPreference->Append(wxID_ANY, _("&Preferences"));

    wxMenu *menuTerminal = new wxMenu;
    menuTerminal->Append(wxID_ANY, _("&Terminal"));

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, _("&About"));

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _("&File"));
    menuBar->Append(menuEdit, _("&Edit"));
    menuBar->Append(menuSelection, _("&Selection"));
    menuBar->Append(menuFind, _("&Find"));
    menuBar->Append(menuView, _("&View"));
    menuBar->Append(menuTools, _("&Tools"));
    menuBar->Append(menuTerminal, _("&Terminal"));
    menuBar->Append(menuPreference, _("&Preferences"));
    menuBar->Append(menuHelp, _("&Help"));
    SetMenuBar(menuBar);

    wxAcceleratorEntry shiftReturn(wxACCEL_ALT, wxACCEL_NORMAL, wxID_BACKWARD);
    this->SetAcceleratorTable(wxAcceleratorTable(1, &shiftReturn));
    this->Bind(wxEVT_MENU, &MainFrame::OnPrev, this, wxID_BACKWARD);

    Connect(ID_NEW_FILE, wxEVT_MENU,
     wxCommandEventHandler(MainFrame::OnNewFile));
    Connect(wxID_ABOUT, wxEVT_MENU,
     wxCommandEventHandler(MainFrame::OnAbout));
    Connect(ID_OPEN_FOLDER, wxEVT_MENU,
     wxCommandEventHandler(MainFrame::OnOpenFolder));
    Connect(wxID_OPEN, wxEVT_MENU,
     wxCommandEventHandler(MainFrame::OnOpenFile));
    Connect(wxID_SAVE, wxEVT_MENU,
     wxCommandEventHandler(CodeContainer::OnSave));
    Maximize();

    this->SetOwnForegroundColour(wxColour(*wxWHITE));
    this->SetThemeEnabled(true);
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(TRUE);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxMessageBox(_("Code editor for all you need"), _("ThunderCode"), 
        wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnNewFile(wxCommandEvent& WXUNUSED(event)) {
    tabs_container->AddTab("Untitled", project_path+"/");
}

void MainFrame::OnOpenFolder(wxCommandEvent& WXUNUSED(event)) {
    wxDirDialog* dlg = new wxDirDialog( NULL, "Choose project directory", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    dlg->ShowModal();
    wxString path = dlg->GetPath();

    if(path.size()) {
        project_path = path;
        if(tabs_container) {
            tabs_container->ClearAllTabs();
        }
        navigation_comp->Update();
    }
}

void MainFrame::OnOpenFile(wxCommandEvent& WXUNUSED(event)) {}
void MainFrame::OnPrev(wxCommandEvent& WXUNUSED(event)) {}
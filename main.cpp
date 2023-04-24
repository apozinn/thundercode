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
#include <wx/sashwin.h>
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

#include "./app.hpp"
#include "./defs.hpp"

#include "./components/codeContainer/code.cpp"
#include "./components/filesTree/files.cpp"
#include "./components/sideNavigation/side.cpp"
#include "./components/statusBar/status.cpp"
#include "./components/tabs/tabs.cpp"
#include "./members/menuBar.cpp"
#include "./members/emptyWindow.cpp"

class ThunderCode: public wxApp {
    virtual bool OnInit();
};

class MainFrame: public wxFrame {
    wxBoxSizer* sizer;
    wxPanel* main_container;
    StatusBar* status_bar;
    wxSplitterWindow* main_splitter;
    SideNavigation* side_navigation;
    FilesTree* files_tree;
    wxPanel* main_code;
    Tabs* tabs_container;
    MenuBar* menu_bar;
    EmptyWindow* empty_window;
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnNewFile(wxCommandEvent& event);
    void OnOpenFolder(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnHiddeFilesTree(wxCommandEvent& event);
    void OnHiddeSideNav(wxCommandEvent& event);
    void OnHiddeMenuBar(wxCommandEvent& event);
    void OnSashPaint( wxPaintEvent& event );
    void OnSashPosChange( wxSplitterEvent& event );
private:
    wxDECLARE_NO_COPY_CLASS(MainFrame);
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(wxID_SAVE, CodeContainer::OnSave)
    EVT_MENU(ID_NEW_FILE, MainFrame::OnNewFile)
    EVT_MENU(ID_OPEN_FOLDER, MainFrame::OnOpenFolder)
    EVT_MENU(ID_OPEN_FILE, MainFrame::OnOpenFile)
    EVT_MENU(ID_HIDDE_FILES_TREE, MainFrame::OnHiddeFilesTree)
    EVT_MENU(ID_HIDDE_SIDE_NAV, MainFrame::OnHiddeSideNav)
    EVT_MENU(ID_HIDDE_MENU_BAR, MainFrame::OnHiddeMenuBar)
wxEND_EVENT_TABLE()

IMPLEMENT_APP(ThunderCode)

bool ThunderCode::OnInit() {
    MainFrame* frame = new MainFrame(_("ThunderCode"), wxPoint(50, 50),  wxSize(900, 800));
    frame->Show(true);
    SetTopWindow(frame);
    return true;
} 

MainFrame::MainFrame(
    const wxString& title, const wxPoint& pos, const wxSize& size
) : wxFrame( NULL, ID_MAIN_FRAME, title, pos, size)
{
    if(wxFile::Exists("./icons/settings.png")) icons_dir = "./icons/";
    else if(wxFile::Exists("../icons/settings.png")) icons_dir = "../icons/";
    else wxLogWarning("Can't find icons dir!");

    sizer = new wxBoxSizer(wxVERTICAL);
    main_container = new wxPanel(this, ID_MAIN_CONTAINER);
    wxBoxSizer* main_container_sizer = new wxBoxSizer(wxHORIZONTAL);

    side_navigation = new SideNavigation(main_container, ID_SIDE_NAVIGATION);
    main_container_sizer->Add(side_navigation, 0, wxEXPAND);
    
    main_splitter = new wxSplitterWindow(main_container, ID_MAIN_SPLITTER);
    main_splitter->SetBackgroundStyle(wxBG_STYLE_PAINT);
    main_splitter->Bind(wxEVT_PAINT, &MainFrame::OnSashPaint, this);
    main_splitter->Bind(wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, &MainFrame::OnSashPosChange, this);

    main_container_sizer->Add(main_splitter, 1, wxEXPAND);
    wxBoxSizer* main_splitter_sizer = new wxBoxSizer(wxHORIZONTAL);
    
    files_tree = new FilesTree(main_splitter, ID_FILES_TREE);
    main_splitter_sizer->Add(files_tree, 0, wxEXPAND);
    
    main_code = new wxPanel(main_splitter, ID_MAIN_CODE);
    main_code->SetBackgroundColour(wxColor(55, 55, 55));
    main_splitter_sizer->Add(main_code, 1, wxEXPAND);
    wxBoxSizer* main_code_sizer = new wxBoxSizer(wxVERTICAL);

    tabs_container = new Tabs(main_code, ID_TABS);
    tabs_container->DestroyChildren();
    empty_window = new EmptyWindow(main_code, ID_EMPYT_WINDOW);

    main_code_sizer->Add(tabs_container, 0, wxEXPAND);
    main_code_sizer->Add(empty_window, 1, wxEXPAND);

    if(project_path.size()) {
        empty_window->Hide();
    } else {}
    
    main_code_sizer->Layout();
    main_code->SetSizerAndFit(main_code_sizer);
    main_splitter->SetSizerAndFit(main_splitter_sizer);
    main_splitter->SetMinimumPaneSize(250);
    main_splitter->SplitVertically(files_tree, main_code, 1);
    
    status_bar = new StatusBar(this, ID_STATUS_BAR);
    main_container->SetSizerAndFit(main_container_sizer);
    sizer->Add(main_container, 1, wxEXPAND);
    sizer->Add(status_bar, 0, wxEXPAND);

    menu_bar = new MenuBar(ID_MENU_BAR);
    SetMenuBar(menu_bar);
    SetTitle("ThunderCode");

    this->SetSizerAndFit(sizer);
    this->SetOwnForegroundColour(wxColour(*wxWHITE));
    this->SetThemeEnabled(true);
    Maximize();
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(TRUE);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxMessageBox(_("Code editor for all you need"), _("ThunderCode"), 
        wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnNewFile(wxCommandEvent& WXUNUSED(event)) {
    tabs_container->AddTab("Untitled", project_path);
}

void MainFrame::OnOpenFolder(wxCommandEvent& WXUNUSED(event)) {
    wxDirDialog* dlg = new wxDirDialog( NULL, "Choose project directory", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    dlg->ShowModal();
    wxString path = dlg->GetPath();
    if(path.size()) {
        project_path = path+"/";
        project_name = wxFileNameFromPath(path);
        if(tabs_container) {
            tabs_container->ClearAllTabs();
        }
        files_tree->Update();
    }
}

void MainFrame::OnOpenFile(wxCommandEvent& WXUNUSED(event)) {}

void MainFrame::OnHiddeFilesTree(wxCommandEvent& WXUNUSED(event)) {
    if(main_splitter) {
        if(files_tree) {
            if(files_tree->IsShown()) {
                files_tree->Hide();
                main_splitter->SetMinimumPaneSize(0);
                main_splitter->SetSashInvisible(true);
            } else {
                files_tree->Show(); 
                main_splitter->SetMinimumPaneSize(250);
                main_splitter->SetSashInvisible(false);
            }
            wxSizer* ms_sizer = main_splitter->GetSizer();
            if(main_splitter) {
                main_splitter->Refresh();
                ms_sizer->Layout();
            }
        }
    }
}

void MainFrame::OnHiddeSideNav(wxCommandEvent& WXUNUSED(event)) {}

void MainFrame::OnHiddeMenuBar(wxCommandEvent& WXUNUSED(event)) {}

void MainFrame::OnSashPaint( wxPaintEvent& event )
{
    wxPaintDC myDC(main_splitter);
    myDC.SetBrush(wxColour(45, 45, 45));
    myDC.SetPen( *wxTRANSPARENT_PEN );

    if( main_splitter->GetSplitMode() == wxSPLIT_VERTICAL) {
        myDC.DrawRectangle(
            main_splitter->GetSashPosition(),
            0,
            main_splitter->GetSashSize(),
            main_splitter->GetSize().GetHeight()
        );
    } else {
        myDC.DrawRectangle(
            0,
            main_splitter->GetSashPosition(),
            main_splitter->GetSize().GetWidth(),
            main_splitter->GetSashSize()
        );
    }
}

void MainFrame::OnSashPosChange( wxSplitterEvent& event ) { main_splitter->Refresh(); }
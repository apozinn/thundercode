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
#include <wx/stdpaths.h>
#include <wx/infobar.h>
#include <wx/log.h>
#include <wx/config.h>
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
    wxSplitterWindow* main_splitter;
    SideNavigation* side_navigation;
    FilesTree* files_tree;
    wxPanel* main_code;
    Tabs* tabs_container;
    MenuBar* menu_bar;
    EmptyWindow* empty_window;
    wxPanel* side_container;
public:
    StatusBar* status_bar;
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnNewFile(wxCommandEvent& event);
    void OnOpenFolder(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnHiddeFilesTree(wxCommandEvent& event);
    void OnHiddeSideNav(wxCommandEvent& event);
    void OnHiddeMenuBar(wxCommandEvent& event);
    void OnHiddeStatusBar(wxCommandEvent& event);
    void OnHiddeTabs(wxCommandEvent& event);
    void OnSashPaint(wxPaintEvent& event);
    void OnSashPosChange(wxSplitterEvent& event);
    void CloseAllFiles(wxCommandEvent& event);
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
    EVT_MENU(ID_HIDDE_STATUS_BAR, MainFrame::OnHiddeStatusBar)
    EVT_MENU(ID_HIDDE_TABS, MainFrame::OnHiddeTabs)
    EVT_MENU(ID_FOCUS_MODE, SideNavigation::OnFocusMode)
    EVT_MENU(ID_CLOSE_ALL_FILES, MainFrame::CloseAllFiles)
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
) : wxFrame(NULL, ID_MAIN_FRAME, title, pos, size)
{
    SetMinSize(size);
    wxInitAllImageHandlers();
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

    side_container = new wxPanel(main_splitter, ID_SIDE_CONTAINER);
    side_container->SetBackgroundColour(wxColor(45, 45, 45));
    wxBoxSizer* side_ctn_sizer = new wxBoxSizer(wxVERTICAL);

    main_splitter_sizer->Add(side_container, 0, wxEXPAND);
    
    files_tree = new FilesTree(side_container, ID_FILES_TREE);
    side_ctn_sizer->Add(files_tree, 1, wxEXPAND | wxLEFT, 5);

    side_container->SetSizerAndFit(side_ctn_sizer);
    
    main_code = new wxPanel(main_splitter, ID_MAIN_CODE);
    main_code->SetBackgroundColour(wxColor(55, 55, 55));
    main_splitter_sizer->Add(main_code, 1, wxEXPAND);
    wxBoxSizer* main_code_sizer = new wxBoxSizer(wxVERTICAL);

    tabs_container = new Tabs(main_code, ID_TABS);
    tabs_container->DestroyChildren();
    empty_window = new EmptyWindow(main_code, ID_EMPYT_WINDOW);

    main_code_sizer->Add(tabs_container, 0, wxEXPAND);
    main_code_sizer->Add(empty_window, 1, wxEXPAND);
    
    main_code_sizer->Layout();
    main_code->SetSizerAndFit(main_code_sizer);
    main_splitter->SetSizerAndFit(main_splitter_sizer);
    main_splitter->SetMinimumPaneSize(250);
    main_splitter->SplitVertically(side_container, main_code, 1);
    
    status_bar = new StatusBar(this, ID_STATUS_BAR);
    main_container->SetSizerAndFit(main_container_sizer);
    sizer->Add(main_container, 1, wxEXPAND);
    sizer->Add(status_bar, 0, wxEXPAND);

    menu_bar = new MenuBar();
    SetMenuBar(menu_bar);
    SetTitle("ThunderCode");

    this->SetSizerAndFit(sizer);
    this->SetOwnForegroundColour(wxColour(*wxWHITE));
    this->SetThemeEnabled(true);
    Maximize();

    wxConfig *config = new wxConfig("ThunderCode");
    wxString str;
    if(config->Read("workspace", &str) ) {
        wxString last_workspace = config->Read("workspace", str);
        project_path = last_workspace;

        project_name = wxFileNameFromPath(project_path.substr(0, project_path.size()-1));
        if(tabs_container) {
            tabs_container->ClearAllTabs();
        }
        files_tree->Update();
        SetTitle("ThunderCode - "+project_name);
    }

    wxAcceleratorEntry entries[1];
    entries[0].Set(wxACCEL_ALT, WXK_ALT, ID_HIDDE_MENU_BAR);
    wxAcceleratorTable accel(1, entries);
    this->SetAcceleratorTable(accel);
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

        wxConfig *config = new wxConfig("ThunderCode");
        config->Write("workspace", project_path);
        delete config;

        SetTitle("ThunderCode - "+project_name);
    }
}

void MainFrame::OnOpenFile(wxCommandEvent& WXUNUSED(event)) {
    wxFileDialog* dlg = new wxFileDialog(NULL, "Choose a file", "", "", "", wxFD_DEFAULT_STYLE | wxFD_FILE_MUST_EXIST);
    dlg->ShowModal();
    wxString path = dlg->GetPath();
    if(path.size()) {
        files_tree->OpenFile(path);
    }
}

void MainFrame::OnHiddeFilesTree(wxCommandEvent& WXUNUSED(event)) {
    if(main_splitter->IsSplit()) {
        main_splitter->Unsplit(side_container);
    } else {
        main_splitter->SplitVertically(side_container, main_code, 1);
    }
}

void MainFrame::OnHiddeSideNav(wxCommandEvent& WXUNUSED(event)) {
    if(side_navigation) {
        if(side_navigation->IsShown()) {
            side_navigation->Hide();
        } else side_navigation->Show();

        main_container->GetSizer()->Layout();
        main_container->Update();
    }
}

void MainFrame::OnHiddeMenuBar(wxCommandEvent& WXUNUSED(event)) {
    if(menu_bar) {
        if(menu_bar->IsShown()) {
            menu_bar->Hide();
        } else menu_bar->Show();
    }
}

void MainFrame::OnHiddeStatusBar(wxCommandEvent& WXUNUSED(event)) {
    if(status_bar) {
        if(status_bar->IsShown()) {
            status_bar->Hide();
        } else status_bar->Show();

        this->GetSizer()->Layout();
        this->Update();
    }
}

void MainFrame::OnHiddeTabs(wxCommandEvent& WXUNUSED(event)) {
    if(tabs_container) {
        if(tabs_container->IsShown()) {
            tabs_container->Hide();
        } else tabs_container->Show();

        main_code->GetSizer()->Layout();
        main_code->Update();
    }
}

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

void MainFrame::CloseAllFiles(wxCommandEvent& WXUNUSED(event)) {
    if(tabs_container) {
        for(auto&& tab : tabs_container->GetChildren()) {
            wxString tab_path = tab->GetName();
            if(FindWindowByLabel(tab_path+"_codeContainer")) {
                FindWindowByLabel(tab_path+"_codeContainer")->Destroy();
            }
            if(FindWindowByLabel(tab_path+"_imageContainer")) {
                FindWindowByLabel(tab_path+"_imageContainer")->Destroy();
            }
            
            tab->Destroy();
        }

        tabs_container->Hide();
        FindWindowById(ID_EMPYT_WINDOW)->Show();
        status_bar->ClearLabels();

        if(files_tree) {
            files_tree->selectedFile->SetBackgroundColour(wxColor(45, 45, 45));
            files_tree->SetSelectedFile(new wxPanel(files_tree, wxID_ANY, wxDefaultPosition, wxSize(0, 0)));
        }
    }
}
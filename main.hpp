#pragma once
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
#include "./members/openFolderLink.cpp"
#include "./members/controlPanel.cpp"

class ThunderCode: public wxApp {
    virtual bool OnInit();
};

IMPLEMENT_APP(ThunderCode)
DECLARE_APP(ThunderCode)

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
    OpenFolderLink* open_folder_link;
    ControlPanel* control_panel;
public:
    StatusBar* status_bar;
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnNewFile(wxCommandEvent& event);
    void OnOpenFolderMenu(wxCommandEvent& event);
    void OnOpenFolderClick(wxMouseEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnHiddeFilesTree(wxCommandEvent& event);
    void OnHiddeSideNav(wxCommandEvent& event);
    void OnHiddeMenuBar(wxCommandEvent& event);
    void OnHiddeStatusBar(wxCommandEvent& event);
    void OnHiddeTabs(wxCommandEvent& event);
    void OnSashPaint(wxPaintEvent& event);
    void OnSashPosChange(wxSplitterEvent& event);
    void CloseAllFiles(wxCommandEvent& event);
    void OpenFolderDialog();
    void ToggleControlPanel(wxCommandEvent& event);
private:
    wxDECLARE_NO_COPY_CLASS(MainFrame);
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(wxID_SAVE, CodeContainer::OnSave)
    EVT_MENU(ID_NEW_FILE, MainFrame::OnNewFile)
    EVT_MENU(ID_OPEN_FOLDER, MainFrame::OnOpenFolderMenu)
    EVT_MENU(ID_OPEN_FILE, MainFrame::OnOpenFile)
    EVT_MENU(ID_HIDDE_FILES_TREE, MainFrame::OnHiddeFilesTree)
    EVT_MENU(ID_HIDDE_SIDE_NAV, MainFrame::OnHiddeSideNav)
    EVT_MENU(ID_HIDDE_MENU_BAR, MainFrame::OnHiddeMenuBar)
    EVT_MENU(ID_HIDDE_STATUS_BAR, MainFrame::OnHiddeStatusBar)
    EVT_MENU(ID_HIDDE_TABS, MainFrame::OnHiddeTabs)
    EVT_MENU(ID_FOCUS_MODE, SideNavigation::OnFocusMode)
    EVT_MENU(ID_CLOSE_ALL_FILES, MainFrame::CloseAllFiles)
    EVT_MENU(ID_TOGGLE_CONTROL_PANEL, MainFrame::ToggleControlPanel)
wxEND_EVENT_TABLE()
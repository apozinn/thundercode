#include "main.hpp"

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
    wxBoxSizer* main_code_sizer = new wxBoxSizer(wxVERTICAL);

    tabs = new Tabs(main_code, ID_TABS);
    empty_window = new EmptyWindow(main_code, ID_EMPYT_WINDOW);

    main_code_sizer->Add(tabs, 0, wxEXPAND);
    main_code_sizer->Add(empty_window, 1, wxEXPAND);
    
    main_code->SetSizerAndFit(main_code_sizer);
    main_splitter_sizer->Add(main_code, 1, wxEXPAND);
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
        if(tabs) {
            tabs->CloseAll();
        }
        files_tree->Update();
        SetTitle("ThunderCode - "+project_name);
    } else {
        if(auto pjt_ctn = FindWindowById(ID_PROJECT_FILES_CTN)) {
            open_folder_link = new OpenFolderLink(pjt_ctn, ID_OPEN_FOLDER_LINK);
            open_folder_link->Bind(wxEVT_LEFT_UP, &MainFrame::OnOpenFolderClick, this);
            for(auto&& children : open_folder_link->GetChildren()) 
                children->Bind(wxEVT_LEFT_UP, &MainFrame::OnOpenFolderClick, this);
            pjt_ctn->GetSizer()->Add(open_folder_link, 1, wxEXPAND);
        }
    }

    wxAcceleratorEntry entries[2];
    entries[0].Set(wxACCEL_ALT, WXK_ALT, ID_HIDDE_MENU_BAR);
    entries[1].Set(wxACCEL_CTRL, WXK_SHIFT, ID_TOGGLE_CONTROL_PANEL);
    entries[1].FromString("Ctrl+Shift+P");
    wxAcceleratorTable accel(2, entries);
    this->SetAcceleratorTable(accel);
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(TRUE);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxMessageBox(_("Code editor for all you need"), _("ThunderCode"), 
        wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OpenFolderDialog() {
    wxDirDialog* dlg = new wxDirDialog( NULL, "Choose project directory", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    dlg->ShowModal();
    wxString path = dlg->GetPath();
    if(path.size()) {
        project_path = path+"/";
        project_name = wxFileNameFromPath(path);
        if(tabs) {
            tabs->CloseAll();
        }
        files_tree->Update();

        wxConfig *config = new wxConfig("ThunderCode");
        config->Write("workspace", project_path);
        delete config;

        SetTitle("ThunderCode - "+project_name);
    }
}

void MainFrame::OnNewFile(wxCommandEvent& WXUNUSED(event)) {
    tabs->Add("Untitled", project_path);
}

void MainFrame::OnOpenFolderMenu(wxCommandEvent& WXUNUSED(event)) {
    OpenFolderDialog();
}

void MainFrame::OnOpenFolderClick(wxMouseEvent& event) {
    OpenFolderDialog();
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
    if(tabs) {
        if(tabs->IsShown()) {
            tabs->Hide();
        } else tabs->Show();

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
    if(tabs) {
        for(auto&& tab : tabs->GetChildren()) {
            wxString tab_path = tab->GetName();
            if(FindWindowByLabel(tab_path+"_codeContainer")) {
                FindWindowByLabel(tab_path+"_codeContainer")->Destroy();
            }
            if(FindWindowByLabel(tab_path+"_imageContainer")) {
                FindWindowByLabel(tab_path+"_imageContainer")->Destroy();
            }
            
            tab->Destroy();
        }

        tabs->Hide();
        FindWindowById(ID_EMPYT_WINDOW)->Show();
        status_bar->ClearLabels();

        if(files_tree) {
            files_tree->selectedFile->SetBackgroundColour(wxColor(45, 45, 45));
            files_tree->SetSelectedFile(new wxPanel(files_tree, wxID_ANY, wxDefaultPosition, wxSize(0, 0)));
        }
    }
}

void MainFrame::ToggleControlPanel(wxCommandEvent& event) {
    if(FindWindowById(ID_CONTROL_PANEL)) {
        control_panel->Destroy();
    } else {
        control_panel = new ControlPanel(this, ID_CONTROL_PANEL);
    }
}
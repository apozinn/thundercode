#include "./files.hpp"
#include "../codeContainer/code.hpp"
#include "../tabs/tabs.hpp"

FilesTree::FilesTree(wxWindow* parent, wxWindowID ID) : wxPanel(parent, ID) 
{
    this->SetBackgroundColour(wxColor(45, 45, 45));
    sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* top_content = new wxPanel(this);
    wxBoxSizer* top_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* top_ctn_pjt = new wxStaticText(top_content, wxID_ANY, "EXPLORATOR");
    top_ctn_pjt->SetFont(fontWithOtherSize(top_ctn_pjt, 15));
    top_ctn_pjt->SetForegroundColour(wxColor(200, 200, 200));
    top_ctn_sizer->Add(top_ctn_pjt, 1, wxEXPAND | wxTOP, 4);

    wxStaticText* top_ctn_menu = new wxStaticText(top_content, ID_FILES_TREE_TOP_MENU, "...");
    top_ctn_sizer->Add(top_ctn_menu, 0, wxEXPAND);
    top_content->SetSizerAndFit(top_ctn_sizer);
    sizer->Add(top_content, 0, wxEXPAND | wxALL, 8);

    wxPanel* project_files = new wxPanel(this, ID_PROJECT_FILES);
    wxBoxSizer* pjt_files_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* project_tools = new wxPanel(project_files, ID_PROJECT_TOOLS_BAR);
    pjt_files_sizer->Add(project_tools, 0, wxEXPAND | wxLEFT, 8);
    wxBoxSizer* pjt_tools_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(wxBitmap(icons_dir+"dir_arrow.png", wxBITMAP_TYPE_PNG).ConvertToImage().Rotate90(true), -1));
    wxStaticBitmap* pjt_arrow = new wxStaticBitmap(project_tools, ID_PJT_TOOLS_ARROW, wxBitmapBundle::FromBitmaps(bitmaps));
    pjt_tools_sizer->Add(pjt_arrow, 0, wxEXPAND | wxBOTTOM, 2);

    wxStaticText* pjt_name = new wxStaticText(project_tools, ID_PJT_TOOLS_PJTNAME, project_name);
    pjt_name->Connect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(FilesTree::ToggleDir));
    pjt_name->SetFont(fontWithOtherSize(pjt_name, 16));
    pjt_tools_sizer->Add(pjt_name, 1, wxEXPAND | wxLEFT, 4);
    project_tools->SetSizerAndFit(pjt_tools_sizer);

    project_files_ctn = new wxScrolled<wxPanel>(project_files, ID_PROJECT_FILES_CTN);
    pjt_files_sizer->Add(project_files_ctn, 1, wxEXPAND | wxLEFT, 3);
    auto pjt_files_ctn_sizer = new wxBoxSizer(wxVERTICAL);
    project_files_ctn->SetSizerAndFit(pjt_files_ctn_sizer);

    project_files->SetSizerAndFit(pjt_files_sizer);
    sizer->Add(project_files, 1, wxEXPAND);

    this->SetSizerAndFit(sizer);
    if(!project_path.size()) pjt_arrow->Hide();
}

void FilesTree::Update() {
    project_files_ctn->DestroyChildren();
    project_files_ctn->SetName(project_path);
    FindWindowById(ID_PJT_TOOLS_PJTNAME)->SetLabel(project_name);
    FindWindowById(ID_PJT_TOOLS_ARROW)->Show();

    fileManager->ListChildrens(
        project_path.ToStdString(), [&](const std::string &path, const std::string &type, const std::string &name
    ) {
        if(type == "dir") {
            this->CreateDir(project_files_ctn, name, path);
        } else {
            this->CreateFile(project_files_ctn, name , path);
        }
    });

    project_files_ctn->FitInside();
    project_files_ctn->SetScrollRate(20, 20);
}

void FilesTree::CreateFile(
    wxWindow* parent, wxString name, wxString path
) {
    if(FindWindowByName(path)) return;
    auto parent_sizer = parent->GetSizer();
    if(!parent_sizer) {
        auto new_sizer = new wxBoxSizer(wxVERTICAL);
        parent->SetSizerAndFit(new_sizer);
        parent_sizer = new_sizer;
    }

    wxPanel* file_container = new wxPanel(parent);
    file_container->SetName(path);
    wxBoxSizer* file_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxImagePanel* file_icon = new wxImagePanel(file_container, icons_dir+"pass.png", wxBITMAP_TYPE_PNG, 17);
    file_ctn_sizer->Add(file_icon, 0, wxEXPAND | wxALL, 2);

    wxStaticText* file_name = new wxStaticText(file_container, wxID_ANY, name);
    file_name->SetFont(fontWithOtherSize(file_name, 18));
    file_name->Connect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(FilesTree::OnFileSelect));
    file_ctn_sizer->Add(file_name, 0, wxEXPAND | wxLEFT | wxTOP, 2);
    file_container->SetSizerAndFit(file_ctn_sizer);

    if(parent->GetParent()->GetName() != project_path && parent->GetName() != project_path) parent->Hide();
    parent_sizer->Add(file_container, 0, wxEXPAND | wxLEFT, 5);
    parent_sizer->Layout();
}

void FilesTree::CreateDir(
    wxWindow* parent, wxString name, wxString path
) {
    if(!parent || FindWindowByName(path)) return;
    auto parent_sizer = parent->GetSizer();
    if(!parent_sizer) {
        wxBoxSizer* n_s = new wxBoxSizer(wxVERTICAL);
        parent->SetSizerAndFit(n_s);
        parent_sizer = n_s;
    }

    wxPanel* container = new wxPanel(parent);
    wxBoxSizer* ctn_sizer = new wxBoxSizer(wxVERTICAL);
    container->SetSizerAndFit(ctn_sizer);

    // props container
    wxPanel* props = new wxPanel(container);
    props->SetName(path);
    wxBoxSizer* props_sizer = new wxBoxSizer(wxHORIZONTAL);
    props->SetSizerAndFit(props_sizer);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(icons_dir+"dir_arrow.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap* dir_arrow = new wxStaticBitmap(props, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
    props_sizer->Add(dir_arrow, 0, wxEXPAND | wxTOP, 2);

    wxStaticText* dir_name = new wxStaticText(props, wxID_ANY, name);
    dir_name->SetFont(fontWithOtherSize(dir_name, 18));
    dir_name->Connect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(FilesTree::ToggleDir));
    props_sizer->Add(dir_name, 0, wxEXPAND | wxLEFT, 4);
    ctn_sizer->Add(props, 0, wxEXPAND | wxALL, 2);

    // childrens container
    wxPanel* childrens_ctn = new wxPanel(container);
    wxBoxSizer* childrens_sizer = new wxBoxSizer(wxVERTICAL);

    fileManager->ListChildrens(
        path.ToStdString(), [&](const std::string &path, const std::string &type, const std::string &name
    ) {
        if(type == "dir") {
            this->CreateDir(container, name, path);
        } else {
            this->CreateFile(container, name, path);
        }
    });

    container->Update();
    ctn_sizer->Layout();

    parent_sizer->Add(container, 0, wxEXPAND | wxLEFT, 5);
    parent->Update();
    parent_sizer->Layout();
}

void FilesTree::OnFileSelect(wxMouseEvent& event) {
    auto codeContainer = ((CodeContainer*)FindWindowById(ID_CODE_CONTAINER));
    auto tabsContainer = ((Tabs*)FindWindowById(ID_TABS));
    wxObject* obj = event.GetEventObject();
    auto this_file = ((wxWindow*)obj)->GetParent();

    if(this_file && tabsContainer && codeContainer) {
        wxString file_path = this_file->GetName();
        tabsContainer->AddTab(wxFileNameFromPath(file_path.substr(0, file_path.size())), file_path);
        if(!codeContainer->IsShown()) FindWindowById(ID_EMPYT_WINDOW)->Destroy();
        codeContainer->LoadNewFile(file_path.substr(0, file_path.size()));
    }
}

void FilesTree::ToggleDir(wxMouseEvent& event) {
    wxObject* obj = event.GetEventObject();
    auto dir_container = ((wxWindow*)obj)->GetGrandParent();

    if(dir_container) {
        auto dir_arrow_ctn = ((wxStaticBitmap*)dir_container->GetChildren()[0]->GetChildren()[0]);
        auto dir_content = dir_container->GetChildren()[1];

        if(dir_content && dir_arrow_ctn) {
            auto arrow_bit = dir_arrow_ctn->GetBitmap();
            wxVector<wxBitmap> bitmaps;

            if(dir_content->IsShown()) {
                dir_content->Hide();
                bitmaps.push_back(wxBitmap(arrow_bit.ConvertToImage().Rotate90(false), -1));
            } else {
                dir_content->Show();
                bitmaps.push_back(wxBitmap(arrow_bit.ConvertToImage().Rotate90(true), -1));
            }
            dir_arrow_ctn->SetBitmap(wxBitmapBundle::FromBitmaps(bitmaps));
        }

        auto dir_ctn_sizer = dir_container->GetSizer();
        dir_container->Update();
        dir_container->Refresh();
        dir_container->Layout();
        dir_ctn_sizer->Layout();
    }
}
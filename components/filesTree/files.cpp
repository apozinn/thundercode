#include "./files.hpp"
#include "../codeContainer/code.hpp"
#include "../tabs/tabs.hpp"

FilesTree::FilesTree(wxWindow* parent, wxWindowID ID) : wxPanel(parent, ID) 
{
    this->SetBackgroundColour(wxColor(45, 45, 45));
    sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* aaa = new wxPanel(this, ID_TRIPLE_A);
    wxBoxSizer* aaa_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* top_content = new wxPanel(aaa);
    wxBoxSizer* top_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* top_ctn_pjt = new wxStaticText(top_content, wxID_ANY, "EXPLORATOR");
    top_ctn_pjt->SetFont(fontWithOtherSize(top_ctn_pjt, 15));
    top_ctn_pjt->SetForegroundColour(wxColor(200, 200, 200));
    top_ctn_sizer->Add(top_ctn_pjt, 1, wxEXPAND | wxTOP, 4);

    wxStaticText* top_ctn_menu = new wxStaticText(top_content, ID_FILES_TREE_TOP_MENU, "...");
    top_ctn_sizer->Add(top_ctn_menu, 0, wxEXPAND);
    top_content->SetSizerAndFit(top_ctn_sizer);
    aaa_sizer->Add(top_content, 0, wxEXPAND | wxTOP | wxBOTTOM, 8);

    wxPanel* project_files = new wxPanel(aaa, ID_PROJECT_FILES);
    wxBoxSizer* pjt_files_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* project_tools = new wxPanel(project_files, ID_PROJECT_TOOLS_BAR);
    pjt_files_sizer->Add(project_tools, 0, wxEXPAND);
    wxBoxSizer* pjt_tools_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(icons_dir+"dir_arrow.png", wxBITMAP_TYPE_PNG));
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
    aaa_sizer->Add(project_files, 1, wxEXPAND);

    aaa->SetSizerAndFit(aaa_sizer);

    sizer->Add(aaa, 1, wxEXPAND | wxLEFT, 5);
    this->SetSizerAndFit(sizer);
    if(!project_path.size()) pjt_arrow->Hide();
}

void FilesTree::Update() {
    project_files_ctn->DestroyChildren();
    project_files_ctn->SetName(project_path);
    FindWindowById(ID_PJT_TOOLS_PJTNAME)->SetLabel(project_name);
    FindWindowById(ID_PJT_TOOLS_ARROW)->Show();

    this->Create(project_path.ToStdString(), project_files_ctn);

    project_files_ctn->FitInside();
    project_files_ctn->SetScrollRate(20, 20);
}

void FilesTree::CreateFile(
    wxWindow* parent, wxString name, wxString path
) {
    if(!parent || FindWindowByName(path)) return;
    auto parent_sizer = parent->GetSizer();
    if(!parent_sizer) {
        auto new_sizer = new wxBoxSizer(wxVERTICAL);
        parent->SetSizerAndFit(new_sizer);
        parent_sizer = new_sizer;
    }

    wxPanel* file_container = new wxPanel(parent);
    file_container->SetName(path);
    wxBoxSizer* file_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(icons_dir+"pass.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap* file_icon = new wxStaticBitmap(file_container, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
    file_ctn_sizer->Add(file_icon, 0, wxEXPAND);

    wxStaticText* file_name = new wxStaticText(file_container, wxID_ANY, name);
    file_name->SetFont(fontWithOtherSize(file_name, 18));
    file_name->Connect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(FilesTree::OnFileSelect));
    file_ctn_sizer->Add(file_name, 0, wxEXPAND | wxLEFT | wxTOP, 2);

    file_container->SetSizerAndFit(file_ctn_sizer);
    parent_sizer->Add(file_container, 0, wxEXPAND | wxLEFT, 5);
    parent_sizer->Layout();
    parent->Update();
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

    wxPanel* dir_container = new wxPanel(parent);
    dir_container->SetName(path);
    wxBoxSizer* dir_ctn_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* dir_props = new wxPanel(dir_container);
    wxBoxSizer* props_sizer = new wxBoxSizer(wxHORIZONTAL);
    dir_ctn_sizer->Add(dir_props, 0, wxEXPAND);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(icons_dir+"dir_arrow.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap* dir_arrow = new wxStaticBitmap(dir_props, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
    props_sizer->Add(dir_arrow, 0, wxEXPAND | wxTOP, 2);

    wxStaticText* dir_name = new wxStaticText(dir_props, wxID_ANY, name);
    dir_name->SetFont(fontWithOtherSize(dir_name, 18));
    dir_name->Connect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(FilesTree::ToggleDir));
    props_sizer->Add(dir_name, 0, wxEXPAND | wxLEFT, 4);
    dir_props->SetSizerAndFit(props_sizer);

    wxPanel* dir_childrens = new wxPanel(dir_container);
    wxBoxSizer* dir_childrens_sizer = new wxBoxSizer(wxVERTICAL);
    dir_childrens->SetSizerAndFit(dir_childrens_sizer);
    dir_ctn_sizer->Add(dir_childrens, 0, wxEXPAND);

    dir_container->SetSizerAndFit(dir_ctn_sizer);
    dir_childrens->Hide();

    parent_sizer->Add(dir_container, 0, wxEXPAND | wxLEFT, 5);
    parent_sizer->Layout();
    parent->Update();
}

void FilesTree::OnFileSelect(wxMouseEvent& event) {
    wxObject* obj = event.GetEventObject();
    auto this_file = ((wxWindow*)obj)->GetParent();
    wxString file_path = this_file->GetName();
    auto main_code = FindWindowById(ID_MAIN_CODE);

    if(!main_code->GetId()) return;

    auto tabsContainer = ((Tabs*)FindWindowById(ID_TABS));
    auto codeContainer = ((CodeContainer*)FindWindowByLabel(file_path+"_codeContainer"));

    for(auto&& other_ct : main_code->GetChildren()) {
        if(other_ct->GetId() != ID_TABS) other_ct->Hide();
    }

    {
        size_t last_dot;
        last_dot = file_path.find_last_of(".");
        if(last_dot) {
            std::string file_ext = file_path.ToStdString().substr(last_dot+1);
            if(file_ext.size()) {
                if(file_ext == "png") {
                    wxImagePanel* img_file = new wxImagePanel(main_code, file_path, wxBITMAP_TYPE_ANY);
                    img_file->SetLabel(file_path+"_imgContainer");
                    main_code->GetSizer()->Add(img_file, 0, wxALIGN_CENTER);

                    auto tab_size_comp = ((wxStaticText*)FindWindowById(ID_STTSBAR_TAB_SIZE));
                    auto file_ext_comp = ((wxStaticText*)FindWindowById(ID_STTSBAR_FILE_EXT));

                    if(tab_size_comp) tab_size_comp->SetLabel("Tab Size: 4");
                    if(file_ext_comp) file_ext_comp->SetLabel(file_ext);
                } else {
                    if(!codeContainer) {
                        codeContainer = new CodeContainer(main_code, wxID_ANY, file_path);
                        main_code->GetSizer()->Add(codeContainer, 1, wxEXPAND);
                    }

                    if(this_file && codeContainer) {
                        if(!codeContainer->IsShown()) {
                            codeContainer->Show();
                        }
                        codeContainer->LoadNewFile(file_path.substr(0, file_path.size()));
                    }
                }

                if(tabsContainer) {
                    tabsContainer->AddTab(wxFileNameFromPath(file_path.substr(0, file_path.size())), file_path);
                }

                main_code->GetSizer()->Layout();
                main_code->Update();
            }
        }
    }
}

void FilesTree::ToggleDir(wxMouseEvent& event) {
    wxObject* obj = event.GetEventObject();
    auto dir_container = ((wxWindow*)obj)->GetGrandParent();

    if(dir_container) {
        auto dir_arrow_ctn = ((wxStaticBitmap*)dir_container->GetChildren()[0]->GetChildren()[0]);
        auto dir_childrens = dir_container->GetChildren()[1];
        wxString path = dir_container->GetName();

        fileManager->ListChildrens(
            path.ToStdString(), [&](const std::string &path, const std::string &type, const std::string &name
        ) {
            if(type == "dir") {
                this->CreateDir(dir_childrens, name, path);                
            } else {
                this->CreateFile(dir_childrens, name, path);
            }

            auto next_parent = dir_childrens;
            bool has_next_parent = true;
            while(has_next_parent) {
                if(next_parent->GetId() == ID_PROJECT_FILES_CTN) has_next_parent = false;
                next_parent->SetSize(next_parent->GetSize().GetWidth(), next_parent->GetSize().GetHeight()+21);
                next_parent->SetMinSize(wxSize(next_parent->GetSize().GetWidth(), next_parent->GetSize().GetHeight()+21));
                if(next_parent->GetParent()) {
                    next_parent = next_parent->GetParent();
                } else has_next_parent = false;
            }
        });

        if(dir_childrens && dir_arrow_ctn) {
            auto arrow_bit = dir_arrow_ctn->GetBitmap();
            wxVector<wxBitmap> bitmaps;

            if(dir_childrens->IsShown()) {
                dir_childrens->Hide();
                bitmaps.push_back(wxBitmap(arrow_bit.ConvertToImage().Rotate90(false), -1));

                dir_container->SetSize(dir_container->GetSize().GetWidth(), 18);
                dir_container->SetMinSize(wxSize(dir_container->GetSize().GetWidth(), 18));
                dir_childrens->SetSize(dir_childrens->GetSize().GetWidth(), 18);
                dir_childrens->SetMinSize(wxSize(dir_childrens->GetSize().GetWidth(), 18));
            } else {
                dir_childrens->Show();
                bitmaps.push_back(wxBitmap(arrow_bit.ConvertToImage().Rotate90(true), -1));
            }

            dir_arrow_ctn->SetBitmap(wxBitmapBundle::FromBitmaps(bitmaps));
            auto dir_ctn_sizer = dir_container->GetSizer();
            dir_container->Update();
            dir_container->Layout();
            dir_ctn_sizer->Layout();

            dir_container->GetParent()->GetSizer()->Layout();
            dir_container->GetParent()->Layout();
            dir_container->GetParent()->Update();
        }
    }
}
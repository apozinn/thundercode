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
    sizer->Add(top_content, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 8);

    wxPanel* project_files = new wxPanel(this, ID_PROJECT_FILES);
    wxBoxSizer* pjt_files_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* project_tools = new wxPanel(project_files, ID_PROJECT_TOOLS_BAR);
    pjt_files_sizer->Add(project_tools, 0, wxEXPAND | wxLEFT, 8);
    wxBoxSizer* pjt_tools_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(wxBitmap(icons_dir+"dir_arrow.png", wxBITMAP_TYPE_PNG).ConvertToImage().Rotate90(true), -1));
    wxStaticBitmap* pjt_arrow = new wxStaticBitmap(project_tools, ID_PJT_TOOLS_ARROW, wxBitmapBundle::FromBitmaps(bitmaps));
    pjt_tools_sizer->Add(pjt_arrow, 0, wxEXPAND | wxTOP, 4);

    wxStaticText* pjt_name = new wxStaticText(project_tools, ID_PJT_TOOLS_PJTNAME, project_name);
    pjt_name->Connect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(FilesTree::ToggleDir));
    pjt_name->SetFont(fontWithOtherSize(pjt_name, 17));
    pjt_tools_sizer->Add(pjt_name, 1, wxEXPAND | wxLEFT, 4);
    project_tools->SetSizerAndFit(pjt_tools_sizer);

    project_files_ctn = new wxScrolled<wxPanel>(project_files, ID_PROJECT_FILES_CTN);
    pjt_files_sizer->Add(project_files_ctn, 1, wxEXPAND | wxLEFT, 3);
    wxBoxSizer* pjt_files_ctn_sizer = new wxBoxSizer(wxVERTICAL);
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

	fileManager->listFiles(project_path.ToStdString(), [&](const std::string &path) {
		std::string new_path = path.substr(project_path.size());
		wxFileName path_props = wxFileName::DirName(new_path);

		for(auto& dir_name : path_props.GetDirs()) {
			wxFileName dir_props = wxFileName::DirName(path.substr(0, path.find(dir_name)+dir_name.size()));
			wxString dir_fullpath = dir_props.GetAbsolutePath();
			wxString parent_path = dir_fullpath.substr(0, dir_fullpath.size()-dir_name.size()-1);

			if(dir_props.DirExists()) {
				auto dir_parent = FindWindowByName(parent_path);
				if(dir_parent) {
					if(parent_path == project_path) {
						this->CreateDir(dir_parent, dir_name, dir_fullpath);
					} else this->CreateDir(dir_parent->GetChildren()[1], dir_name, dir_fullpath);
				}
			}
		}

		for(auto& file_name : path_props.GetDirs()) {
			wxFileName file_props = wxFileName::DirName(path.substr(0, path.find(file_name)+file_name.size()));
			wxString file_fullpath = file_props.GetAbsolutePath();
			wxString parent_path = file_fullpath.substr(0, file_fullpath.size()-file_name.size()-1);

			if(file_props.FileExists()) {
				auto file_parent = FindWindowByName(parent_path);
				if(file_parent) {
					if(parent_path == project_path) {
						this->CreateFile(file_parent, file_name, file_fullpath);
					} else this->CreateFile(file_parent->GetChildren()[1], file_name, file_fullpath);
				}
			}
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
    	auto new_sizer = new wxBoxSizer(wxVERTICAL);
    	parent->SetSizerAndFit(new_sizer);
    	parent_sizer = new_sizer;
    }

	wxPanel* dir_container = new wxPanel(parent);
	dir_container->SetName(path);
	wxBoxSizer* dir_ctn_sizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* dir_props = new wxPanel(dir_container);
    dir_props->SetName(path);
    wxBoxSizer* dir_props_sizer = new wxBoxSizer(wxHORIZONTAL);
    dir_ctn_sizer->Add(dir_props, 0, wxEXPAND | wxALL, 2);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(icons_dir+"dir_arrow.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap* dir_arrow = new wxStaticBitmap(dir_props, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
    dir_props_sizer->Add(dir_arrow, 0, wxEXPAND | wxTOP, 4);

    wxStaticText* dir_name = new wxStaticText(dir_props, wxID_ANY, name);
    dir_name->SetFont(fontWithOtherSize(dir_name, 18));
    dir_name->Connect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(FilesTree::ToggleDir));
    dir_props_sizer->Add(dir_name, 0, wxEXPAND | wxLEFT, 4);

    dir_props->SetSizerAndFit(dir_props_sizer);

	wxPanel* dir_childrens = new wxPanel(dir_container);
	wxBoxSizer* dir_childrens_sizer = new wxBoxSizer(wxVERTICAL);
	dir_childrens->SetSizerAndFit(dir_childrens_sizer);
	dir_ctn_sizer->Add(dir_childrens, 1, wxEXPAND);
	dir_container->SetSizerAndFit(dir_ctn_sizer);

	if(parent->GetParent()->GetName() != project_path && parent->GetName() != project_path) parent->Hide();
	parent_sizer->Add(dir_container, 1, wxEXPAND | wxLEFT, 5);
	parent_sizer->Layout();
}

void FilesTree::OnFileSelect(wxMouseEvent& event) {
    auto codeContainer = ((CodeContainer*)FindWindowById(ID_CODE_CONTAINER));
    auto tabsContainer = ((Tabs*)FindWindowById(ID_TABS));
    wxObject* obj = event.GetEventObject();
    auto this_file = ((wxWindow*)obj)->GetParent();

    if(this_file && tabsContainer && codeContainer) {
        wxString file_path = this_file->GetName();
        tabsContainer->AddTab(wxFileNameFromPath(file_path.substr(0, file_path.size()-1)), file_path);
        if(!codeContainer->IsShown()) FindWindowById(ID_EMPYT_WINDOW)->Destroy();
        codeContainer->LoadNewFile(file_path.substr(0, file_path.size()-1));
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
#include <vector>
#include <wx/filename.h>
#include <wx/scrolwin.h>
#include "../app.h"
#include "../utils/randoms.hpp"
#include "../utils/fileManager.hpp"
#include "../members/imagePanel.cpp"

class Navigation : public wxPanel {
	wxPanel* Navigation_cmp;
	wxSizer* sizer;
	wxImagePanel* file_icon;
	wxPanel* file_container;
	wxSizer* files_tree_sizer;
	wxStaticText* project_name_comp;
	wxScrolled<wxPanel>* files_tree;
public:
	FileManager* fileManager = new FileManager();
	Navigation(wxPanel* parent) : wxPanel(parent, ID_NAVIGATION_COMP) {
		Navigation_cmp = this;
		this->SetBackgroundColour(wxColor(45, 45, 45));
		sizer = new wxBoxSizer(wxVERTICAL);

		wxPanel* top_nav = new wxPanel(this, wxID_ANY);
		wxBoxSizer* top_sizer = new wxBoxSizer(wxHORIZONTAL);

		wxStaticText* tc_name = new wxStaticText(top_nav, wxID_ANY, "THUNDERCODE");
		wxFont font = tc_name->GetFont(); 
		font.SetPixelSize(wxSize(14, 14));
		tc_name->SetFont(font);

		wxStaticText* top_nav_dots = new wxStaticText(top_nav, wxID_ANY, "...");

		top_sizer->Add(tc_name, 17, wxALIGN_CENTER);
		top_sizer->Add(top_nav_dots, 1, wxALIGN_CENTER);
		top_nav->SetSizerAndFit(top_sizer);

		sizer->Add(top_nav, 0, wxALL, 10);

		wxPanel* project_tools = new wxPanel(this, wxID_ANY);
		wxBoxSizer* project_tools_sizer = new wxBoxSizer(wxHORIZONTAL);
		project_tools->SetBackgroundColour(wxColor(55, 55, 55));

		project_name_comp = new wxStaticText(project_tools, ID_PROJECT_NAME, project_name);
		font.SetPixelSize(wxSize(16, 16));
		project_name_comp->SetFont(font);

		project_tools_sizer->Add(project_name_comp, 1, wxALIGN_CENTER | wxALL, 3);
		project_tools->SetSizerAndFit(project_tools_sizer);

		files_tree = new wxScrolled<wxPanel>(this, ID_FILES_TREE_COMP);
		files_tree->SetBackgroundColour(wxColor(45, 45, 45));

		sizer->Add(project_tools, 0, wxEXPAND);
		sizer->Add(files_tree, 1, wxEXPAND);
		if(project_path.size()) Update();
		this->SetSizerAndFit(sizer);
	}

	void Update() {
		wxString dir;
		if(wxFile::Exists("./icons/settings.png")) dir = "./icons/";
		else if(wxFile::Exists("../icons/settings.png")) dir = "../icons/";

		files_tree->Destroy();
		files_tree = new wxScrolled<wxPanel>(Navigation_cmp, ID_FILES_TREE_COMP);
		files_tree->SetBackgroundColour(wxColor(45, 45, 45));
		sizer->Add(files_tree, 1, wxEXPAND);

		files_tree->SetName("files_tree");

		project_path = wxGetCwd();

		project_name = wxFileNameFromPath(project_path);
		project_name_comp->SetLabel(stringToWxString(project_name));
		files_tree_sizer = new wxBoxSizer(wxVERTICAL);

		wxArrayString created_project_dirs = wxArrayString();

		fileManager->listFiles(project_path.ToStdString()+"/", [&](const std::string &path) {
			int id_divisor = path.find(project_name);
		    wxString project_begin = path.substr(id_divisor+project_name.size()+1);
		    wxFileName dirname = wxFileName::DirName(project_begin);
		    wxArrayString path_dirs = dirname.GetDirs();

			for(auto& dir_name : path_dirs) {
				int this_dir_div = path.find(dir_name);
				wxString dir_path = path.substr(0, this_dir_div+dir_name.size()+1);
				wxFileName this_dir_props = wxFileName::DirName(dir_path);

				if(!this_dir_props.HasExt() && this_dir_props.DirExists()) 
				{
					if(created_project_dirs.Index(dir_path) < 0) {
						created_project_dirs.Add(dir_path);

						if(!path_dirs.Index(dir_name)) {
							create_dir_panel(files_tree->GetName(), dir_name, dir_path);	
						} else {
							wxString dir_parent = dir_path.substr(0, dir_path.find(dir_name));
							auto parent_panel = FindWindowByName(dir_parent);
							if(parent_panel) {
								// create_dir_panel(parent_panel->GetName(), dir_name, dir_path);
							}
						}
					}
				}
			}

			wxString f_n = wxFileNameFromPath(stringToWxString(path));
			file_container = new wxPanel(files_tree);
			file_container->SetName(path);
			wxBoxSizer* this_sizer = new wxBoxSizer(wxHORIZONTAL);

			int raiz = project_begin.find("/") > 100 ? true : false;
		    if(raiz) {
				file_icon = new wxImagePanel(file_container, dir+"file-icon.png", wxBITMAP_TYPE_PNG);
				wxStaticText* file_name = new wxStaticText(file_container, wxID_ANY, f_n);

				wxFont font = file_name->GetFont(); 
				font.SetPixelSize(wxSize(16, 16));
				file_name->SetFont(font);

				this_sizer->Add(file_icon, 1, wxEXPAND);
				this_sizer->Add(file_name, 10, wxEXPAND | wxTOP, 3);

				file_container->SetSizerAndFit(this_sizer);
				files_tree_sizer->Add(file_container, 0, wxEXPAND | wxALL, 3);
				file_container->Lower();
		    } else {}
		});

		files_tree->SetSizerAndFit(files_tree_sizer);
		files_tree->FitInside();
		files_tree->SetScrollRate(10, 10);
	}

	void create_dir_panel(wxString parent_name, wxString name, wxString path) {
		auto parent = FindWindowByName(parent_name);
		if(parent) {
			wxPanel* dir_container = new wxPanel(parent);
			dir_container->SetName(path);
			wxBoxSizer* this_main_sizer = new wxBoxSizer(wxVERTICAL);

			wxPanel* dir_ctn_props = new wxPanel(dir_container);
			wxBoxSizer* dir_ctn_props_sizer = new wxBoxSizer(wxHORIZONTAL);

			file_icon = new wxImagePanel(dir_ctn_props, icons_dir+"dir_arrow.png", wxBITMAP_TYPE_PNG);
			wxStaticText* file_name = new wxStaticText(dir_ctn_props, wxID_ANY, name);
			wxFont font = file_name->GetFont(); 
			font.SetPixelSize(wxSize(16, 16));
			file_name->SetFont(font);

			//dir_ctn_props sizer
			dir_ctn_props_sizer->Add(file_icon, 1, wxEXPAND | wxTOP, 5);
			dir_ctn_props_sizer->Add(file_name, 15, wxEXPAND | wxTOP, 3);
			dir_ctn_props->SetSizerAndFit(dir_ctn_props_sizer);

			//dir_container sizer
			this_main_sizer->Add(dir_ctn_props, 0, wxEXPAND);
			dir_container->SetSizerAndFit(this_main_sizer);

			wxSizer* parent_sizer = parent->GetSizer();
			files_tree_sizer->Add(dir_container, 0, wxEXPAND | wxALL, 3);

			dir_container->Raise();
		}
	}
	void OnHover(wxMouseEvent& event);
	void OnHoverEnd(wxMouseEvent& event);
	wxDECLARE_NO_COPY_CLASS(Navigation);
};

void Navigation::OnHover(wxMouseEvent& event) {
	wxObject* obj = event.GetEventObject();
    auto this_pth = ((Navigation*)obj)->GetParent();

    if(this_pth) {
	    for(wxWindowList::iterator it = this_pth->GetChildren().begin(); 
	    	it != this_pth->GetChildren().end(); it++) 
	    {
	    	wxPanel* a_pth = dynamic_cast<wxPanel *>( *it );
	    	if(a_pth) a_pth->SetBackgroundColour(wxColor(90, 90, 90));
	    }
    }
}

void Navigation::OnHoverEnd(wxMouseEvent& event) {
	wxObject* obj = event.GetEventObject();
    auto this_pth = ((Navigation*)obj)->GetParent();

    if(this_pth) {
	    for(wxWindowList::iterator it = this_pth->GetChildren().begin(); 
	    	it != this_pth->GetChildren().end(); it++) 
	    {
	    	wxPanel* a_pth = dynamic_cast<wxPanel *>( *it );
	    	if(a_pth) a_pth->SetBackgroundColour(wxColor(45, 45, 45));
	    }
    }
}
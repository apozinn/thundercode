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

		project_name = wxFileNameFromPath(project_path);
		project_name_comp->SetLabel(stringToWxString(project_name));
		wxBoxSizer* files_tree_sizer = new wxBoxSizer(wxVERTICAL);

		wxArrayString project_dirs = wxArrayString();

		fileManager->listFiles(project_path.ToStdString()+"/", [&](const std::string &path) {
			int id_divisor = path.find(project_name);
		    wxString project_begin = path.substr(id_divisor+project_name.size()+1);

		    wxFileName dirname = wxFileName::DirName(project_begin);

			for(auto& dir_ : dirname.GetDirs()) {
				wxFileName dir_p(dir_);
				if(!dir_p.HasExt()) {
					// project_dirs.Add(dir_);
					bool finded;

					for(wxWindowList::iterator it = files_tree->GetChildren().begin(); 
						it != files_tree->GetChildren().end(); it++) 
					{
						wxPanel* a_tab = dynamic_cast<wxPanel *>( *it );
						if(a_tab){
							if(a_tab->GetName() == dir_) finded = true;
					    }
					}

					if(!finded) {
						file_container = new wxPanel(files_tree);
						file_container->SetName(dir_);
						wxBoxSizer* this_sizer = new wxBoxSizer(wxHORIZONTAL);

						file_icon = new wxImagePanel(file_container, dir+"dir_arrow.png", wxBITMAP_TYPE_PNG);
						wxStaticText* file_name = new wxStaticText(file_container, wxID_ANY, dir_);

						wxFont font = file_name->GetFont(); 
						font.SetPixelSize(wxSize(16, 16));
						file_name->SetFont(font);

						this_sizer->Add(file_icon, 1, wxEXPAND | wxTOP, 5);
						this_sizer->Add(file_name, 15, wxEXPAND | wxTOP, 3);

						file_container->SetSizerAndFit(this_sizer);
						files_tree_sizer->Add(file_container, 0, wxEXPAND | wxALL, 3);
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
private:
	wxStaticText* project_name_comp;
	wxScrolled<wxPanel>* files_tree;
	wxDECLARE_NO_COPY_CLASS(Navigation);
};
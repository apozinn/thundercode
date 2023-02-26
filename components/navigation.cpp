#include <vector>
#include <wx/filename.h>
#include <wx/scrolwin.h>
#include "../app.h"
#include "../utils/randoms.hpp"
#include "../utils/fileManager.hpp"

class Navigation : public wxPanel {
public:
	FileManager* fileManager = new FileManager();
	Navigation(wxPanel* parent) : wxPanel(parent, ID_NAVIGATION_COMP) {
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

		files_tree = new wxScrolled(this, ID_FILES_TREE_COMP);
		files_tree->SetBackgroundColour(wxColor(45, 45, 45));

		sizer->Add(project_tools, 0, wxEXPAND);
		sizer->Add(files_tree, 1, wxEXPAND);
		if(wxGetCwd().size()) Update();
		this->SetSizerAndFit(sizer);
	}

	void Update() {
		project_name = wxFileNameFromPath(wxGetCwd());
		project_name_comp->SetLabel(stringToWxString(project_name));

		wxBoxSizer* files_tree_sizer = new wxBoxSizer(wxVERTICAL);

		fileManager->listFiles(wxGetCwd().ToStdString()+"/", [&](const std::string &path) {
			wxString file_name = wxFileNameFromPath(stringToWxString(path));

			wxStaticText* ff = new wxStaticText(files_tree, wxID_ANY, file_name);
			files_tree_sizer->Add(ff, 0, wxALL, 2);
		});

		files_tree->SetSizerAndFit(files_tree_sizer);
	}
private:
	wxPanel* Navigation_comp;
	wxBoxSizer* sizer;
	wxStaticText* project_name_comp;
	wxScrolled<wxPanel>* files_tree;
	wxDECLARE_NO_COPY_CLASS(Navigation);
};
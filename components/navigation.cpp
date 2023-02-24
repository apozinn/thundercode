#include <vector>
#include "../app.h"
#include "../utils/randoms.hpp"

class Navigation : public wxPanel {
public:
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

		wxStaticText* project_name = new wxStaticText(project_tools, wxID_ANY, "project");
		font.SetPixelSize(wxSize(16, 16));
		project_name->SetFont(font);

		project_tools_sizer->Add(project_name, 1, wxALIGN_CENTER | wxALL, 3);
		project_tools->SetSizerAndFit(project_tools_sizer);

		sizer->Add(project_tools, 0, wxEXPAND);
		this->SetSizerAndFit(sizer);
	}
private:
	wxPanel* Navigation_comp;
	wxBoxSizer* sizer;
	wxDECLARE_NO_COPY_CLASS(Navigation);
};
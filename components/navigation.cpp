#include <vector>
#include <wx/filename.h>
#include <wx/scrolwin.h>
#include "../app.h"
#include "../utils/randoms.hpp"
#include "../utils/fileManager.hpp"

class wxImagePanel : public wxPanel {
	wxBitmap image;
    public:
        wxImagePanel(wxPanel* parent, wxString file, wxBitmapType format);
        
        void paintEvent(wxPaintEvent & evt);
        void paintNow();
        void render(wxDC& dc);
        
        DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
EVT_PAINT(wxImagePanel::paintEvent)
END_EVENT_TABLE()

wxImagePanel::wxImagePanel(
	wxFrame* parent, wxString file, wxBitmapType format
) : wxPanel(parent) {
    image.LoadFile(file, format);
}

void wxImagePanel::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void wxImagePanel::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void wxImagePanel::render(wxDC&  dc)
{
    dc.DrawBitmap( image, 0, 0, false );
}

class Navigation : public wxPanel {
	wxImagePanel* file_icon;
	wxPanel* file_container;
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

		files_tree = new wxScrolled<wxPanel>(this, ID_FILES_TREE_COMP);
		files_tree->SetBackgroundColour(wxColor(45, 45, 45));

		sizer->Add(project_tools, wxImagePanel0, wxEXPAND);
		sizer->Add(files_tree, 1, wxEXPAND);
		if(wxGetCwd().size()) Update();
		this->SetSizerAndFit(sizer);
	}

	void Update() {
		wxString dir;
		if(wxFile::Exists("./icons/settings.png")) dir = "./icons/";
		else if(wxFile::Exists("../icons/settings.png")) dir = "../icons/";

		project_name = wxFileNameFromPath(wxGetCwd());
		project_name_comp->SetLabel(stringToWxString(project_name));

		wxBoxSizer* files_tree_sizer = new wxBoxSizer(wxVERTICAL);

		fileManager->listFiles(wxGetCwd().ToStdString()+"/", [&](const std::string &path) {
			wxString f_n = wxFileNameFromPath(stringToWxString(path));

			file_container = new wxPanel(files_tree);
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			file_icon = new wxImagePanel(file_container, dir+"file-icon.png", wxBITMAP_TYPE_PNG);
			wxStaticText* file_name = new wxStaticText(files_tree, wxID_ANY, f_n);

			sizer->Add(file_icon, 1, wxEXPAND);
			sizer->Add(file_name, 35, wxEXPAND);

			file_container->SetSizerAndFit(sizer);
			files_tree_sizer->Add(file_container, 0, wxALL, 2);
		});

		files_tree->SetSizerAndFit(files_tree_sizer);
		files_tree->FitInside();
		files_tree->SetScrollRate(10, 10);
	}
private:
	wxPanel* Navigation_comp;
	wxBoxSizer* sizer;
	wxStaticText* project_name_comp;
	wxScrolled<wxPanel>* files_tree;
	wxDECLARE_NO_COPY_CLASS(Navigation);
};
#pragma once

class EmptyWindow : public wxPanel {
	wxBoxSizer* sizer;
public:
	EmptyWindow(wxWindow* parent, wxWindowID ID) : wxPanel(parent, ID) {
		this->SetBackgroundColour(wxColor(21, 21, 21));
		sizer = new wxBoxSizer(wxVERTICAL);
		wxString logo_dir;

		if(wxFile::Exists("./assets/logo_emptyWindow.png")) logo_dir = "./assets/logo_emptyWindow.png";
	    else if(wxFile::Exists("../assets/logo_emptyWindow.png")) logo_dir = "../assets/logo_emptyWindow.png";

		wxImagePanel* logo = new wxImagePanel(this, logo_dir, wxBITMAP_TYPE_PNG, 200);
		sizer->AddStretchSpacer();
		sizer->Add(logo, 0, wxALL | wxALIGN_CENTER, 10);
		sizer->AddStretchSpacer();

		this->SetSizerAndFit(sizer);
	}
};
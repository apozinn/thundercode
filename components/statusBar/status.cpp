#include "status.hpp"

StatusBar::StatusBar(wxFrame* parent, wxWindowID ID) : wxPanel(parent, ID) 
{
	this->SetBackgroundColour(wxColor(45,120,210));
	sizer = new wxBoxSizer(wxHORIZONTAL);

	//left container
	wxPanel* left_ctn = new wxPanel(this);
	wxBoxSizer* left_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxPanel* error_ctn = new wxPanel(left_ctn);
	wxBoxSizer* error_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxImagePanel* error_icon = new wxImagePanel(error_ctn, icons_dir+"cancel.png", wxBITMAP_TYPE_PNG, 15);
	error_ctn_sizer->Add(error_icon, 0, wxEXPAND | wxLEFT, 4);
	wxStaticText* error_count = new wxStaticText(error_ctn, wxID_ANY, "0");
	error_ctn_sizer->Add(error_count, 0, wxEXPAND | wxLEFT, 3);
	error_count->SetFont(fontWithOtherSize(error_count, 15));

	error_ctn->SetSizerAndFit(error_ctn_sizer);
	left_ctn_sizer->Add(error_ctn);
	sizer->Add(left_ctn, 1, wxEXPAND | wxTOP, 3);
	//

	//right container
	wxPanel* right_ctn = new wxPanel(this);
	wxBoxSizer* right_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxImagePanel* git_icon = new wxImagePanel(right_ctn, icons_dir+"git.png", wxBITMAP_TYPE_PNG, 15);
	right_ctn_sizer->Add(git_icon, 0, wxEXPAND | wxRIGHT, 4);

	right_ctn->SetSizerAndFit(right_ctn_sizer);
	sizer->Add(right_ctn, 0, wxEXPAND | wxTOP, 3);
	//

	this->SetSizerAndFit(sizer);
}
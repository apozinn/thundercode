#include "status.hpp"

StatusBar::StatusBar(wxFrame* parent, wxWindowID ID) : wxPanel(parent, ID) 
{
	this->SetBackgroundColour(wxColor(45,120,210));
	sizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* app_name = new wxStaticText(this, wxID_ANY, "ThunderCode");
	sizer->Add(app_name, 0, wxEXPAND | wxALL, 3);

	this->SetSizerAndFit(sizer);
}
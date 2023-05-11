#include "builder.hpp"

SidePage_Builder::SidePage_Builder(
	wxWindow* parent, wxWindowID ID
) : wxPanel(parent, ID) {
	this->SetBackgroundColour(wxColor(45, 45, 45));
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* under_construction = new wxStaticText(this, wxID_ANY, "Under Construction");

	sizer->Add(under_construction, 0, wxALIGN_CENTER);
	this->SetSizerAndFit(sizer);
}
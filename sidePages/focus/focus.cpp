#include "focus.hpp"

SidePage_Focus::SidePage_Focus(
	wxWindow* parent, wxWindowID ID
) : wxPanel(parent, ID) {
	this->SetBackgroundColour(wxColor(45, 45, 45));
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* title = new wxStaticText(this, wxID_ANY, "Focus Page");
	sizer->Add(title, 0, wxEXPAND);

	this->SetSizerAndFit(sizer);
}
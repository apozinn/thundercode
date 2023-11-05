#include "extensions.hpp"

SidePage_Extensions::SidePage_Extensions(
	wxWindow* parent, wxWindowID ID
) : wxPanel(parent, ID) {
	this->SetBackgroundColour(wxColor(45, 45, 45));
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizerAndFit(sizer);
}
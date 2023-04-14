#include "search.hpp"

SidePage_Search::SidePage_Search(
	wxWindow* parent, wxWindowID ID
) : wxPanel(parent, ID) {
	this->SetBackgroundColour(wxColor(45, 45, 45));
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxTextCtrl* input = new wxTextCtrl(this, wxID_ANY, "Search", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    input->SetDefaultStyle(wxTextAttr(*wxWHITE));
    input->SetBackgroundColour(wxColor(45, 45, 45));

    sizer->Add(input, 0, wxEXPAND);
    this->SetSizerAndFit(sizer);
}
#include "search.hpp"

#include "../../members/customRectangle.cpp"

SidePage_Search::SidePage_Search(
	wxWindow* parent, wxWindowID ID
) : wxPanel(parent, ID) {
	this->SetBackgroundColour(wxColor(45, 45, 45));
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* top_content = new wxPanel(this);
	wxBoxSizer* top_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxTextCtrl* input = new wxTextCtrl(top_content, wxID_ANY, "Search");
    input->SetMinSize(wxSize(input->GetSize().GetWidth(), 30));
    input->SetBackgroundColour(wxColor(55, 55, 55));
    top_ctn_sizer->Add(input, 1, wxEXPAND);

	top_content->SetSizerAndFit(top_ctn_sizer);

	sizer->Add(top_content, 0, wxEXPAND | wxTOP | wxBOTTOM, 8);
    this->SetSizerAndFit(sizer);
}
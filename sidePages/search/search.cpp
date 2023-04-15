#include "search.hpp"

SidePage_Search::SidePage_Search(
	wxWindow* parent, wxWindowID ID
) : wxPanel(parent, ID) {
	this->SetBackgroundColour(wxColor(45, 45, 45));
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* top_content = new wxPanel(this);
	wxBoxSizer* top_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxVector<wxBitmap> bitmaps_s;
    bitmaps_s.push_back(wxBitmap(icons_dir+"case_sensitive.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap* search_ico = new wxStaticBitmap(top_content, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps_s));
    top_ctn_sizer->Add(search_ico, 0, wxEXPAND | wxRIGHT, 4);

    wxVector<wxBitmap> bitmaps_se;
    bitmaps_se.push_back(wxBitmap(icons_dir+"selection.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap* in_selection_ico = new wxStaticBitmap(top_content, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps_se));
    top_ctn_sizer->Add(in_selection_ico, 0, wxEXPAND | wxRIGHT, 4);

	wxTextCtrl* input = new wxTextCtrl(top_content, wxID_ANY, "Search", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    input->SetDefaultStyle(wxTextAttr(*wxWHITE));
    input->SetBackgroundColour(wxColor(50, 50, 50));
    top_ctn_sizer->Add(input, 1, wxEXPAND);

	top_content->SetSizerAndFit(top_ctn_sizer);

	sizer->Add(top_content, 0, wxEXPAND | wxTOP | wxBOTTOM, 8);
    this->SetSizerAndFit(sizer);
}
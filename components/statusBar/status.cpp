#include "status.hpp"

StatusBar::StatusBar(wxFrame* parent, wxWindowID ID) : wxPanel(parent, ID) 
{
	this->SetBackgroundColour(wxColor(45,120,210));
	sizer = new wxBoxSizer(wxHORIZONTAL);

	wxPanel* left_ctn = new wxPanel(this);
	wxBoxSizer* left_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxPanel* error_ctn = new wxPanel(left_ctn);
	wxBoxSizer* error_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxVector<wxBitmap> bitmaps_e;
    bitmaps_e.push_back(wxBitmap(icons_dir+"cancel.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap* error_ico = new wxStaticBitmap(error_ctn, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps_e));
	error_ctn_sizer->Add(error_ico, 0, wxALIGN_CENTER);

	wxStaticText* error_count = new wxStaticText(error_ctn, wxID_ANY, "0");
	error_count->SetFont(fontWithOtherSize(error_count, 15));
	error_count->SetMinSize(wxSize(error_count->GetSize().GetWidth(), 15));
	error_ctn_sizer->Add(error_count, 0, wxALIGN_CENTER | wxLEFT, 3);

	error_ctn->SetSizerAndFit(error_ctn_sizer);

	left_ctn_sizer->Add(error_ctn);
	sizer->Add(left_ctn, 1, wxEXPAND | wxALL, 4);

	wxPanel* right_ctn = new wxPanel(this);
	wxBoxSizer* right_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxVector<wxBitmap> bitmaps_suggest;
    bitmaps_suggest.push_back(wxBitmap(icons_dir+"suggest.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap* suggest_ico = new wxStaticBitmap(right_ctn, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps_suggest));
	right_ctn_sizer->Add(suggest_ico, 0, wxALIGN_CENTER | wxRIGHT, 4);

	wxVector<wxBitmap> bitmaps_sup;
    bitmaps_sup.push_back(wxBitmap(icons_dir+"support.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap* sup_ico = new wxStaticBitmap(right_ctn, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps_sup));
	right_ctn_sizer->Add(sup_ico, 0, wxALIGN_CENTER);

	right_ctn->SetSizerAndFit(right_ctn_sizer);
	sizer->Add(right_ctn, 0, wxEXPAND | wxALL, 4);

	this->SetSizerAndFit(sizer);
}
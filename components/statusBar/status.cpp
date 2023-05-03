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
	left_ctn_sizer->SetSizeHints(left_ctn);

	left_ctn->SetSizerAndFit(left_ctn_sizer);
	sizer->Add(left_ctn, 1, wxEXPAND | wxALL, 3);

	wxPanel* right_ctn = new wxPanel(this);
	wxBoxSizer* right_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* code_locale = new wxStaticText(right_ctn, ID_STTSBAR_CODELOCALE, "");
	code_locale->SetFont(fontWithOtherSize(code_locale, 15));
	code_locale->SetMinSize(wxSize(110, 15));
	right_ctn_sizer->Add(code_locale, 0, wxALIGN_CENTER | wxRIGHT, 4);

	wxStaticText* tab_size = new wxStaticText(right_ctn, ID_STTSBAR_TAB_SIZE, "");
	tab_size->SetFont(fontWithOtherSize(tab_size, 15));
	tab_size->SetMinSize(wxSize(60, 15));
	right_ctn_sizer->Add(tab_size, 0, wxALIGN_CENTER | wxRIGHT, 4);

	wxStaticText* file_ext = new wxStaticText(right_ctn, ID_STTSBAR_FILE_EXT, "");
	file_ext->SetFont(fontWithOtherSize(file_ext, 15));
	file_ext->SetMinSize(wxSize(30, 15));
	right_ctn_sizer->Add(file_ext, 0, wxALIGN_CENTER | wxRIGHT, 4);

	wxVector<wxBitmap> bitmaps_suggest;
    bitmaps_suggest.push_back(wxBitmap(icons_dir+"suggest.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap* suggest_ico = new wxStaticBitmap(right_ctn, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps_suggest));
    suggest_ico->SetMinSize(wxSize(16, 16));
	right_ctn_sizer->Add(suggest_ico, 0, wxALIGN_CENTER | wxRIGHT, 4);

	wxVector<wxBitmap> bitmaps_sup;
    bitmaps_sup.push_back(wxBitmap(icons_dir+"support.png", wxBITMAP_TYPE_PNG));
    wxStaticBitmap* sup_ico = new wxStaticBitmap(right_ctn, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps_sup));
    sup_ico->SetMinSize(wxSize(16, 16));
	right_ctn_sizer->Add(sup_ico, 0, wxALIGN_CENTER);

	right_ctn_sizer->SetSizeHints(right_ctn);

	right_ctn->SetSizerAndFit(right_ctn_sizer);
	sizer->Add(right_ctn, 0, wxEXPAND | wxALL, 3);

	sizer->SetSizeHints(this);
	this->SetSizerAndFit(sizer);
}
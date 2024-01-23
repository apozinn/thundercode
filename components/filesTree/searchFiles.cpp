class SearchFiles : public wxPanel {
    json Themes = UserConfig().GetThemes();
public:
	SearchFiles(wxWindow* parent, wxWindowID ID, wxColor background_color) : wxPanel(parent, ID) {
		SetBackgroundColour(background_color);
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		wxStaticBitmap* search_icon = new wxStaticBitmap(
	    	this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(icons_dir+"search.png", wxBITMAP_TYPE_PNG))
	    );
	    sizer->Add(search_icon, 0, wxEXPAND | wxALL, 5);

	    wxStyledTextCtrl* search_input = new wxStyledTextCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	    search_input->SetSize(wxSize(100, 10));
	    search_input->SetMinSize(wxSize(100, 10));
	    sizer->Add(search_input, 0, wxEXPAND | wxALL, 5);

	    search_input->SetWrapMode(wxSTC_WRAP_NONE);
	    search_input->StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(background_color));
	    search_input->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor("#7d7d7e"));
	    search_input->StyleClearAll();

	    search_input->SetMarginWidth(0, 0);
	    search_input->SetMarginWidth(1, 0);
	    search_input->SetUseVerticalScrollBar(false);
	    search_input->SetUseHorizontalScrollBar(false);

	    search_input->SetCaretForeground(wxColour(wxT("WHITE")));
	    search_input->SetText("Search a file");
		SetSizerAndFit(sizer);
	}
};
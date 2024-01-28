class SearchFiles : public wxPanel {
    json Themes = UserConfig().GetThemes();
    wxStyledTextCtrl* search_input;
public:
	SearchFiles(wxWindow* parent, wxWindowID ID, wxColor background_color) : wxPanel(parent, ID) {
		SetBackgroundColour(background_color);
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		wxStaticBitmap* search_icon = new wxStaticBitmap(
	    	this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(icons_dir+"search_small_gray.png", wxBITMAP_TYPE_PNG))
	    );
	    sizer->Add(search_icon, 0, wxALIGN_CENTER | wxALL, 2);

	    search_input = new wxStyledTextCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	    search_input->SetSize(wxSize(GetSize().GetWidth(), GetSize().GetHeight()));
	    search_input->SetMinSize(wxSize(GetSize().GetWidth(), GetSize().GetHeight()));
	    sizer->Add(search_input, 1, wxALIGN_CENTER);

	    search_input->SetWrapMode(wxSTC_WRAP_NONE);
	    search_input->StyleSetBackground(wxSTC_STYLE_DEFAULT, background_color);
	    search_input->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor("#7d7d7e"));
	    search_input->StyleClearAll();

	    search_input->SetZoom(-1);

	    search_input->SetMarginWidth(0, 0);
	    search_input->SetMarginWidth(1, 0);
	    search_input->SetUseVerticalScrollBar(false);
	    search_input->SetUseHorizontalScrollBar(false);

	    search_input->SetCaretForeground(wxColour(wxT("WHITE")));
	    search_input->SetText("Search a file");
	    search_input->Bind(wxEVT_LEFT_UP, &SearchFiles::OnClick, this);
	    search_input->Bind(wxEVT_KILL_FOCUS, &SearchFiles::OnLostFocus, this);
		SetSizerAndFit(sizer);
	}
private:
	void OnClick(wxMouseEvent& event) {
	    search_input->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(*wxWHITE));
	    search_input->StyleClearAll();
		search_input->SetText("");
		event.Skip();
	}
	void OnLostFocus(wxEvent& event) {
	    search_input->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor("#7d7d7e"));
	    search_input->StyleClearAll();
		search_input->SetText("Search a file");
		event.Skip();
	}
};
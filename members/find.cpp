class Find : public wxPanel {
public:
	Find(wxWindow* parent, wxString defaultInput) : wxPanel(parent, ID_FIND_CONTAINER, wxPoint(parent->GetSize().GetWidth()-310, 10), wxSize(300, 51)) {
	    SetMinSize(wxSize(300, 51));
	    SetSize(wxSize(300, 51));
	    SetBackgroundColour(wxColor(36, 36, 36));

	    wxBoxSizer* find_sizer = new wxBoxSizer(wxVERTICAL);

	    wxStyledTextCtrl* input = new wxStyledTextCtrl(this);
	    input->SetSize(wxSize(200, 25));
	    input->SetPosition(wxPoint(10, 13));
	    input->SetText(defaultInput);

	    input->SetWrapMode(wxSTC_WRAP_NONE);

	    input->StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(36, 36, 36));
	    input->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));
	    input->StyleClearAll();

	    input->SetMarginWidth(0, 0);
	    input->SetMarginWidth(1, 0);
	    input->SetUseVerticalScrollBar(false);
	    input->SetUseHorizontalScrollBar(false);

	    input->SetCaretForeground(wxColour(wxT("WHITE")));

	    input->SetFocus();
	    input->GotoPos(input->GetLength());

	    input->SetSelectionStart(0);
	    input->SetSelectionEnd(input->GetLength());
	    input->SetHotspotSingleLine(true);

	    input->IndicatorFillRange(0, 2);

	    SetSizer(find_sizer);

	    wxAcceleratorEntry entries[1];
	    entries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, ID_EXIT_FIND_CONTAINER);
	    wxAcceleratorTable accel(1, entries);
	    SetAcceleratorTable(accel);
	}

	void Close(wxCommandEvent& event) {
		Destroy();
	}

	wxDECLARE_NO_COPY_CLASS(Find);
	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(Find, wxPanel)
	EVT_MENU(ID_EXIT_FIND_CONTAINER, Find::Close)
wxEND_EVENT_TABLE()
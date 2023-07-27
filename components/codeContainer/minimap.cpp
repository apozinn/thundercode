class MiniMap : public wxStyledTextCtrl {
public: 
	MiniMap(wxWindow* parent, wxString path) : wxStyledTextCtrl(parent) {
		LoadFile(path);
		SetZoom(-10);
		SetSize(wxSize(100, parent->GetSize().GetHeight()));
		SetMinSize(wxSize(100, parent->GetSize().GetHeight()));
		SetReadOnly(true);

		wxFont font = wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	    font.SetFaceName(wxT("Monospace"));

	    StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(55, 55, 55));
	    StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));
	    StyleSetFont(wxSTC_STYLE_DEFAULT, font);

	    StyleClearAll();
	    SetCaretForeground(wxColour(wxT("WHITE")));

	    SetMarginWidth(0, 0);
	    SetMarginType(0, wxSTC_MARGIN_NUMBER);
	    StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(128, 128, 128));
	    StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColor(55, 55, 55));

	    SetMarginWidth(1, 0);
	    SetMarginType(1, 0);
	    SetMarginMask(1, 0);
	    SetMarginSensitive(1, false);

		SetPosition(wxPoint(parent->GetSize().GetHeight()+230, 0));
	}
};
#pragma once

class Terminal : public wxPanel {
    wxString cmd = "ls";
    wxStyledTextCtrl* shell;
    wxBoxSizer* sizer;
public:
	Terminal(wxWindow* parent, wxWindowID ID) : wxPanel(parent, ID) {
		SetBackgroundColour(wxColor(55, 55, 55));
		sizer = new wxBoxSizer(wxVERTICAL);

	    /*wxArrayString output, errors;
        int code = wxExecute(cmd, output, errors, wxEXEC_SYNC);

        ShowOutput(cmd, output, "Output");
        ShowOutput(cmd, errors, "Errors");*/

		auto separator = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(this->GetSize().GetWidth(), 1), wxLI_HORIZONTAL);
		separator->SetBackgroundColour(wxColor(70, 70, 70));
		sizer->Add(separator, 0, wxEXPAND);

		shell = new wxStyledTextCtrl(this);
		shell->SetBackgroundColour(wxColor(55, 55, 55));

		wxFont font(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		font.SetFaceName(wxT("Monospace"));

		shell->StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(55, 55, 55));
	    shell->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));
	    shell->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
	    shell->StyleClearAll();

	    shell->SetMarginWidth(0, 0);
	    shell->SetMarginWidth(1, 0);
	    shell->SetMarginWidth(2, 0);
    	shell->SetCaretForeground(wxColour(wxT("WHITE")));
    	shell->SetUseHorizontalScrollBar(false);
		shell->AppendText("[samuel@fedora ThunderCode]$ ");

		sizer->Add(shell, 1, wxEXPAND | wxTOP, 5);
		SetSizerAndFit(sizer);
	}
private:
	void Paint(wxPaintEvent& event) {
		wxPaintDC this_dc(this);
	    this_dc.SetBrush(wxColour(70, 70, 70));
	    this_dc.SetPen(wxColour(70, 70, 70));
	}

	void ShowOutput(
		const wxString& cmd,
		const wxArrayString& output,
		const wxString& title
	) {
	    size_t count = output.GetCount();
	    if ( !count )
	        return;

	    for ( size_t n = 0; n < count; n++ )
	    {
	    	std::cout << output[n] << "\n";
	    }
	}
};